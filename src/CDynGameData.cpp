#include "CDynGameData.h"

#include <algorithm>

#include "common/CMath.h"

#include "CGameData.h"
#include "CSoundPlayer.h"
#include "CGameGraphicsInterface.h"
#include "CGameSystem.h"
#include "CPlayer.h"
#include "CGameStatisticsData.h"
#include "CIngameData.h"
#include "CLevelRuntimeData.h"
#include "timer.h"
#include "common/CSteam.h"

namespace
{
	const int KTooManyTries=100000;
	const char* KUnknownSoldier="Unknown";
	const int KSteamRateArrayLength=8;
	const int KSteamRates[KSteamRateArrayLength ]=
	{
		30,20,10,10,8,8,6,6
	};
};

CDynGameData::CDynGameData(CGameGraphicsInterface* aGGI, 
						   CGameData* aGameData, 
						   CSoundPlayer* aSP,
						   CGameSystem* aGS,
						   COptions* aOptions,
						   int aPlayerAmount):
iGameData(aGameData),
iGGI(aGGI),
iGameSystem(aGS),
iOptions(aOptions),
iSP(aSP),
iAllPlayersDead(false), 
iUserQuit(false), 
iShowMap(false), 
iNextLevel(0), 
iCompleteTime(0), 
iDrawFrames(false)
{
	ASSERTPTR( aGGI );
	ASSERTPTR( aGameData );
	ASSERTPTR( aSP );
	ASSERTPTR( aGS );
	iMouseOwner=NULL;

    for(int a=0;a<aPlayerAmount;a++)
	{
		iAnyPlayer.push_back(new CPlayer(this,aGGI,aGameData,aSP,aGS,aGS));
	}

	iStatistics = new CGameStatisticsData;
	iDarkBuf = new CGraphicsBuffer();
	NetMsgReset();
	iRuntimeData = new CLevelRuntimeData( aGameData, aSP, aGS, this );

	iGGI->AddObserver( this );
	GraphicsModeChanged(); // update these things for the first time
	iSP->SetDynData( this );
}

CDynGameData::~CDynGameData()
{
	iGGI->RemoveObserver( this );
	iSP->SetDynData( NULL );
	delete iDarkBuf;
	delete iRuntimeData;
	iRuntimeData = NULL;

	Reset();
	
	delete iStatistics;

	for(int a=0;a<iAnyPlayer.size();a++)
	{
		delete iAnyPlayer[a];
	}
	iAnyPlayer.clear();
}



void CDynGameData::Reset()
{
	if ( iRuntimeData )
	{
		delete iRuntimeData;
		iRuntimeData = new CLevelRuntimeData( iGameData, iSP, iGameSystem, this );
	}
	iAllPlayersDead=false;
    iLastFPSClockVal=0;
	iFPSIndex=0;
	for (int a=0;a<KFPSAVGAmount;a++)
		iFramesPerSecData[a] = 0;
	iUserQuit=false;
}

void CDynGameData::SetLocalPlayers(int aAmount)
{
    ASSERT(aAmount>=1);
	ASSERT(aAmount<=iAnyPlayer.size());

	int a;
	iLocalPlayer.clear();

	iLocalPlayer.reserve( aAmount );

	for (a=0;a<aAmount;a++)
		iLocalPlayer.push_back(iAnyPlayer[a]);
}

void CDynGameData::MapLocalPlayer(int aLocalIndex,int aAnyIndex)
{
	ASSERT(aLocalIndex>=0);
	ASSERT(aAnyIndex>=0);
	ASSERT(aLocalIndex<iLocalPlayer.size());
	ASSERT(aAnyIndex<iAnyPlayer.size());

	iLocalPlayer[aLocalIndex]=iAnyPlayer[aAnyIndex];
}

void CDynGameData::NetMsgAppendChar(char aChar)
{
	if (iNetworkMessageCursorPos<KNetMessageMaxLength)
	{
		iNetworkMessageBuffer[iNetworkMessageCursorPos]=aChar;
		iNetworkMessageCursorPos++;
	}
}

void CDynGameData::NetMsgReset()
{
	iNetworkMessageBuffer[0]=0;
	iNetworkMessageCursorPos=0;
}

void CDynGameData::NetMsgEraseChar()
{
	if (iNetworkMessageCursorPos>0) 
	{
		iNetworkMessageCursorPos--;
		iNetworkMessageBuffer[iNetworkMessageCursorPos]=0;
	}
}

const char* CDynGameData::NetMsg()
{
	return iNetworkMessageBuffer;
}

void CDynGameData::SetMessageWritingMode(bool aMode)
{
	iMessageWritingMode=aMode;
}

bool CDynGameData::CharCollision(const CRect<int>& aRect,CCharacter* aSelf)
{
		CCoord<float> left = aRect.TopLeft();
		CCoord<float> right = aRect.BottomRight();

		std::vector< CEnemy >::iterator low = 
			lower_bound(iRuntimeData->Enemies().begin(),iRuntimeData->Enemies().end(),left,SortOperator);
		std::vector< CEnemy >::iterator high = 
			upper_bound(iRuntimeData->Enemies().begin(),iRuntimeData->Enemies().end(),right,SortOperator);
		CPlayer* ptr2;

		while(low != high)
		{	
			if (static_cast<CCharacter*>(&(*low))!=aSelf)
				if (low->CollisionCheck(aRect))
					return true;
			low++;
		}

		for (int b=0;b<iAnyPlayer.size();b++) 
		{
			ptr2 = iAnyPlayer[b];
			if (static_cast<CCharacter*>(ptr2)!=aSelf&&
			    !ptr2->iDead&&
			    ptr2->CollisionCheck(aRect))
				return true;
		}

		return false;
}

void CDynGameData::SetupLevelStart()
{
	DEBUG0("CDynGameData::SetupLevelStart: Reset calls\n");
	Reset();

	/* Setup level*/
	if (iNetworkMode!=ENetworkModeClient)
	{
		iRuntimeData->Level().Load( iGameData->Options()->EpisodeList()->Episode( iEpisode, GameMode()->DMEpisodes() )->Level(iCurrentLevel) );

		iStatistics->LevelStats( iCurrentLevel ).iLevelInfo = iRuntimeData->Level().iGeneralLevelInfo;

		// initialize rest of the player later
		PlacePlayersForLevel();

		if ( iGameData->Options()->Data().iEnemiesOnGame || !GameMode()->DisableEnemiesPossible() ) 
		{
			iRuntimeData->InitEnemies();
		}

		iRuntimeData->PlaceGameCrates( GameMode()->DMCrates() );

		if (GameMode()->RandomPlacePlayers())
			for(int a=0; a<iLocalPlayer.size(); a++) 
				iLocalPlayer[a]->RandomPlace();
	} 

	iNextLevel=0;
	SetMessageWritingMode(false);
}

void CDynGameData::PreparePlayersForLevel() 
{
	const int startPositions = iRuntimeData->Level().StartPositions();

	for(int b = 0; b < iLocalPlayer.size(); b++)
	{
		CPlayer *ply = iLocalPlayer[b];
		ply->Reset(PlayerDrawRect(b));
		ply->Move( 0,0,0, PlayerDrawRect(b) );
		ply->iEnergy = KPlayerMaxEnergy+(ply->iShield*KShieldEnergyAmount);

		GameMode()->SetupPlayerLevel( ply );
	}
}

void CDynGameData::PlacePlayersForLevel()
{
	const int startPositions = iRuntimeData->Level().StartPositions();

	for(int b = 0; b < iLocalPlayer.size(); b++)
	{
		CCoord<int> pos, newPos;
		bool found = false;
		CPlayer *ply = iLocalPlayer[b];

		pos = iRuntimeData->Level().StartPosition(b % startPositions).Mul( KBlockSpriteSize );

		// place players
		int tries;
		for (tries = 0;tries < KTooManyTries && !found;tries++)
		{
			CCoord<int> add;
			bool tooClose = false;
			newPos = pos;

			// stupid algorithm:
			if ( tries > 0 && tries < 9 )
			{
				add.Set( (rand() % 3) - 1,(rand() % 3) - 1 );
			}
			if ( tries > 10 && tries < 99 )
			{
				add.Set( (rand() % 5) - 2,(rand() % 5) - 2 );
			}
			if ( tries > 100 )
			{
				add.Set( (rand() % 9) - 4,(rand() % 9) - 4 );
			}

			newPos.Set( newPos.Add(	add.Mul( KBlockSpriteSize ) ) );

			if ( !iRuntimeData->Level().Reachable( newPos.Div( KBlockSpriteSize ) ) )
				continue;

			for (int cnt=0;cnt<b&&!tooClose;cnt++)
				if ( CMath::GetDist( newPos, iLocalPlayer[ cnt ]->Location()) < KBlockSpriteSize )
					tooClose = true;

			if ( tooClose )
				continue;
			
			found = 1;
		};

		if ( tries >= KTooManyTries )
			throw CCriticalException("CDynGameData::PlacePlayerForLevel(): Placing of players failed!");

		ASSERT( found );

		ply->Location().Set( newPos );
	}
}

bool CDynGameData::TestEnemyLocation( CCoord<int>& aPos, bool aStrictCheck )
{
	int plyDist = KCharacterSpriteSize;
	const int KEnemyRad = 9;

	if ( aStrictCheck )
		plyDist = KCharacterSpriteSize*5;

	if (iRuntimeData->Level().Reachable( aPos.Add(KBlockSpriteSize/2, KBlockSpriteSize/2).Div( KBlockSpriteSize ) ) )
	{
		std::vector< CEnemy >::iterator iter = iRuntimeData->Enemies().begin();
		while(iter != iRuntimeData->Enemies().end() )
		{
			if (CMath::GetDist(aPos,iter->Location()) < KEnemyRad*2 )
			{
				DEBUG1("CDynGameData::TestEnemyLocation: other enemy %s too close.\n",iter->Location().String().c_str() );
				return false;
			}
			++iter;
		}

		for(int b=0; b < iRuntimeData->Level().StartPositions() && !aStrictCheck; b++)
		{
			// don't put enemies on start blocks.
			if (CMath::GetDist( aPos, iRuntimeData->Level().StartPosition( b ).Mul( KBlockSpriteSize )) < KBlockSpriteSize )
			{
				DEBUG0("CDynGameData::TestEnemyLocation: enemy on startblock\n");
				return false;
			}
		}

		for(int c=0; c < Players().size(); c++)
		{
			if ( CMath::GetDist( aPos, iAnyPlayer[c]->Location() ) < plyDist )
			{
				DEBUG0("CDynGameData::TestEnemyLocation: player too close\n");
				return false; 
			}
		}

		return true;
	}
	else
	{
		DEBUG0("CDynGameData::TestEnemyLocation: enemy unreachable\n");
		return false;
	}
}

int CDynGameData::CheckCratePlace(int aX,int aY,int aSize) 
{
	ASSERT(aX>=0);
	ASSERT(aY>=0);

	if ((aY+aSize)/KBlockSpriteSize>=iRuntimeData->Level().Height()) return 1;
	if ((aX+aSize)/KBlockSpriteSize>=iRuntimeData->Level().Width()) return 1;

	if (iRuntimeData->Level().LevelData(aX/KBlockSpriteSize,aY/KBlockSpriteSize).iType!= EBlockTypeFloor) return 1;
	if (iRuntimeData->Level().LevelData(aX/KBlockSpriteSize,(aY+aSize)/KBlockSpriteSize).iType!= EBlockTypeFloor) return 1;
	if (iRuntimeData->Level().LevelData((aX+aSize)/ KBlockSpriteSize,aY/KBlockSpriteSize).iType!= EBlockTypeFloor) return 1;
	if (iRuntimeData->Level().LevelData((aX+aSize)/ KBlockSpriteSize,(aY+aSize)/KBlockSpriteSize).iType!= EBlockTypeFloor) return 1;
	for(int a=0; a<iLocalPlayer.size(); a++) 
		if (CMath::GetDist(CCoord<float>(aX,aY),iLocalPlayer[a]->Location()) <50) return 1;

	return 0;
}

void CDynGameData::UpdateMouseOwner()
{
	int mMode;

	mMode=iGameData->Options()->Data().iMouseMode;
	ASSERT(mMode>=0);
	if (mMode==0||mMode-1>=iLocalPlayer.size())
	{
		iMouseOwner=NULL;
		return;
	}
	iMouseOwner=iLocalPlayer[mMode-1];
}

void CDynGameData::MoveToDeads( CPlayer* aPlayer )
{
	std::vector< CPlayer* >::iterator iter1 = find(iLocalPlayer.begin(), iLocalPlayer.end(), aPlayer);
	std::vector< CPlayer* >::iterator iter2 = find(iAnyPlayer.begin(), iAnyPlayer.end(), aPlayer);

	ASSERT( iter1 != iLocalPlayer.end() );
	ASSERT( iter2 != iAnyPlayer.end() );

	iRuntimeData->DeadPlayers().push_back( *iter1 );
	iLocalPlayer.erase( iter1 );
	iAnyPlayer.erase( iter2 );
}

CGameStatisticsData* CDynGameData::Statistics()
{
	ASSERT( iStatistics );
	return iStatistics;
}

CStatViewData& CDynGameData::StatViewData()
{
	return iStatViewData;
}

void CDynGameData::FPSCounter( int aClockVal )
{
	iLowFramerate=aClockVal-iLastFrame;
	iLastFrame=aClockVal;

	aClockVal /= KUpdFPSInterval; // to get seconds
	while (aClockVal>iLastFPSClockVal)
	{
		iFPSIndex = ( iFPSIndex+1 ) % KFPSAVGAmount;
		iFramesPerSecData[ iFPSIndex ] = 0;
		iLastFPSClockVal++;
	};

	iFramesPerSecData[ iFPSIndex ]++;
}

float CDynGameData::CurrentFPS() const
{
	int a;
	float sum=0;

	for (a=0;a<KFPSAVGAmount;a++)
	{
		if (a!=iFPSIndex)
			sum+=iFramesPerSecData[a];
	}

	return (KTimerFreq/KUpdFPSInterval)*sum/(KFPSAVGAmount-1);
}

void CDynGameData::GraphicsModeChanged()
{
	CCoord<int> mode = iGGI->DrawBuffer()->Rect().BottomRight();

	// reset current rectangles
	for (int a=0;a<KMaxLocalPlayers;a++)
	{
		iBlackRects[a].clear();
		iDrawRects[a].clear();
	}

	// Setup 1 player game window
	// It's full screen
	iDrawRects[0].push_back( CRect<int>(0,0,mode.X(),mode.Y() ) );

	// Setup 2 players game windows
	//   ---------
	//   |   |   |
	//   |1pl|2pl|
	//   |   |   |
	//   ---------
	iDrawRects[1].push_back( CRect<int>(0,0,mode.X()/2,mode.Y()));
	iDrawRects[1].push_back( CRect<int>(mode.X()/2,0,mode.X(),mode.Y()));


	// Setup 3 players game windows
	//   ---------
	//   |   |   |
	//   | 1 | 2 |
	//   ---------
	//   |  |3 | |
	//   ---------


	iDrawRects[2].push_back(CRect<int>(0,0,mode.X()/2,mode.Y()/2));
	iDrawRects[2].push_back(CRect<int>(mode.X()/2,0,mode.X(),mode.Y()/2));
	iDrawRects[2].push_back(CRect<int>(mode.X()/4,mode.Y()/2,mode.X()*3/4,mode.Y()));
	iBlackRects[2].push_back( CRect<int>(0,mode.Y()/2,mode.X()/4,mode.Y()));
	iBlackRects[2].push_back( CRect<int>(mode.X()*3/4,mode.Y()/2,mode.X(),mode.Y()));

	// Setup 4 players game windows
	//   ---------
	//   |   |   |
	//   | 1 | 2 |
	//   ---------
	//   | 3 | 4 |
	//	 |   |   |
	//   ---------
	iDrawRects[3].push_back(CRect<int>(0,0,mode.X()/2,mode.Y()/2));
	iDrawRects[3].push_back(CRect<int>(mode.X()/2,0,mode.X()  ,mode.Y()/2));
	iDrawRects[3].push_back(CRect<int>(0,mode.Y()/2,mode.X()/2,mode.Y()));
	iDrawRects[3].push_back(CRect<int>(mode.X()/2,mode.Y()/2,mode.X(),mode.Y()));

	iDarkBuf->Resize(mode);
}

const CRect<int>& CDynGameData::PlayerDrawRect( int aPlayerNo )
{
	const int tot = iLocalPlayer.size();

	ASSERT(aPlayerNo>=0);
	ASSERT(aPlayerNo<tot);

	return iDrawRects[tot-1][aPlayerNo];
}

const int CDynGameData::BlackRects( int aPlayers )
{
	return iBlackRects[ aPlayers-1 ].size();
}

const CRect<int>& CDynGameData::BlackRect( int aRect, int aPlayers )
{
	DEBUG_ASSERT( aRect < BlackRects( aPlayers ) );
	return iBlackRects[ aPlayers-1 ][ aRect ];
}

int CDynGameData::LevelsInEpisode() const
{
	DEBUG_ASSERT( iLevelsInEpisode > 0 );
	return iLevelsInEpisode;
}

int CDynGameData::CurrentLevel() const
{
	DEBUG_ASSERT( iCurrentLevel >= 0 );
	return iCurrentLevel;
}

bool CDynGameData::SetLevels( int aLevels, int aStartLevel )
{
	if ( aLevels <= 0 )
		return false;
	if ( aLevels >= 10000 )
		return false;
    if ( aStartLevel >= aLevels )
		return false;

	iCurrentLevel = aStartLevel;
	iLevelsInEpisode = aLevels;

	return true;
}

void CDynGameData::NextLevel()
{
	iCurrentLevel++;
}

void CDynGameData::SetupPlayers() 
{
	int b;

	for(b = 0; b < iLocalPlayer.size(); b++)
	{
		CPlayer* pl = iLocalPlayer[b];

		Statistics()->AddPlayer( pl , true );

		if ( strlen( iOptions->Data().iName[b]) <= 0 )
			strcpy(pl->iName,KUnknownSoldier );
		else
//			strcpy(pl->iName,iOptions->Data().iName[b]);
			pl->SetName(iOptions->Data().iName[b]);

		if (EF_DEBUG)
			pl->iCash = 50000;
		else
            pl->iCash = 500;

		pl->iDead = 0;

		pl->SetColorByIndex( b );

		GameMode()->SetupPlayerEpisode( pl );
	}
}

IGameMode* CDynGameData::GameMode()
{
	return iGameMode;
}

void CDynGameData::Setup( IGameMode* aFightMode, TNetworkMode aNetMode, int aEpisode )
{
	iGameMode = aFightMode;
	iNetworkMode = aNetMode;
	iEpisode = aEpisode;
}

void CDynGameData::Explo(const CCoord<int>& aPos,int aSize, float aShare, IBulletObserver* aObserver )
{
	TIngameSoundEffect fx=ESoundEffectNoSound;
	
	if (aSize <= 1)	
		fx=ESoundEffectSmallExplosion;
	if (aSize >= 2 && aSize <= 3)
		fx=ESoundEffectExplosion;
	if (aSize >= 4)
		fx=ESoundEffectBigExplosion;
	
	iRuntimeData->Explo( aPos, aSize, aShare, aObserver );

	iSP->PlayPosSample( fx,KFullVolume, aPos);
}

void CDynGameData::CheckPlayersLiving() 
{
	int a;

	iAllPlayersDead=true;
	for(a=0; a<iLocalPlayer.size() && iAllPlayersDead; a++) 
		if (iLocalPlayer[a]->iDead <= KDeadAnimationDelay) 
			iAllPlayersDead=false;
}

void CDynGameData::RemoveLocalPlayer( CPlayer *aPlayer ) 
{
	CheckPlayersLiving();

	// Things to do when player dies
	// 1. Add player to dead players' list (needed so that we know how many players there was)
	// 2. Remove from local players (so that screen disappears)
	// 3. Fix mouse control (is it needed nowadays?)
	// 4. something else?

	if (iLocalPlayer.size()>1)
	{
		// Remove player from active players
		MoveToDeads( aPlayer );
		CGameStatisticsData::TPlayerStats& stat = Statistics()->PlayerStats( aPlayer );
		stat.iLevelOfDeath = CurrentLevel();
	}
}


void CDynGameData::PushPlayers() 
{
	int a;
	for(a=0; a<iLocalPlayer.size(); a++) 
	if (iLocalPlayer[a]->iPushPower>0) 
	{
		if (iLocalPlayer[a]->iPushPower>0) 
		iLocalPlayer[a]->Move(
			iLocalPlayer[a]->iPushAngle, iLocalPlayer[a]->iPushPower,a,
			PlayerDrawRect( a ));
		iLocalPlayer[a]->iPushPower -= 0.075f;
	}
}



void CDynGameData::DecHits() 
{
	int a;
	for(a=0;a<iLocalPlayer.size();a++) 
		if (iLocalPlayer[a]->iHit>0) 
			iLocalPlayer[a]->iHit--;
}

void CDynGameData::CheckBurn() 
{
	int a;//,mod;

	for(a=0; a<Players().size(); a++) 
		if (!Players()[a]->iDead) 
		{
			Players()[a]->CheckBurning( this );
		}

	std::vector< CEnemy >::iterator iter = iRuntimeData->Enemies().begin();

	while(iter != iRuntimeData->Enemies().end() )
	{
		if (iter->CheckBurning( this ))
		{
			iRuntimeData->KillEnemy( iter );
		}
		else
            ++iter;
	}
}

int CDynGameData::Episode() const
{
	return iEpisode;
}

void CDynGameData::ResetClocks()
{
	int val = TimerCounter=1;
	iStartTime=val;
	iLastFrame=val-1;
	iLowFramerate=val;
}

void CDynGameData::AnimatePlayers() 
{
	int a;
	for(a=0; a<iLocalPlayer.size(); a++) 
		iLocalPlayer[a]->Animate();
}

void CDynGameData::ShowSteam() 
{
	const int buffer = 100;
	int b;
	CSteam* ptr=iRuntimeData->Level().iSteams;

	while(ptr)
	{
		bool du;
		int a;

		for (a=0, du=false; !du && a<iLocalPlayer.size() ;a++)
		{
		   if (CRect<int>(
			   iLocalPlayer[a]->Location().Add( -PlayerDrawRect(a).Width()/2-buffer,-PlayerDrawRect(a).Height()/2-buffer),
 			   iLocalPlayer[a]->Location().Add( PlayerDrawRect(a).Width()/2+buffer, PlayerDrawRect(a).Height()/2+buffer)
			   ).Inside(CCoord<int>(*ptr)))
				du=true;
		}

		if (du) 
		{
			if (ptr->Speed()<KSteamRateArrayLength ) 
				b=KSteamRates[ptr->Speed()];
			else 
				b=1;

			if (iRuntimeData->SteamAnimationCount(b)) 
				iRuntimeData->NewEffect(EEffectSteam,*ptr,ptr->Angle()-3+rand()%6,ptr->Speed() / 2 * 0.75);
		}

		ptr=ptr->Next();
	}
}
