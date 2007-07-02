#include "CLevelRuntimeData.h"

#include <algorithm>

#include "common/CMath.h"
#include "CWeaponCrate.h"
#include "CBulletCrate.h"
#include "CEnergyCrate.h"
#include "CMoneyCrate.h"
#include "CTargetSystemCrate.h"
#include "CEffect.h"
#include "CGameSystem.h"
#include "IGUIStateController.h"
#include "CDynGameData.h"
#include "CIngameData.h"
#include "CBullet.h"

namespace
{
	const int KTooManyTries=100000;
};

CLevelRuntimeData::CLevelRuntimeData( CGameData* aGameData, CSoundPlayer* aSP,CGameSystem* aGS, CDynGameData* aDynData ) : iGameData( aGameData ), iGS( aGS ), iSP( aSP ), iDynData( aDynData )
{
	iSteamAnimationCount=0;
	iIngameData = new CIngameData( iDynData );
}

CLevelRuntimeData::~CLevelRuntimeData()
{
	ResetCrates();
	iEnemies.clear();
	iDeadEnemies.clear();
	iBodyparts.clear();
	ResetBullets();
	std::list<CEffect*>::iterator it = iEffects.begin();
	while (it != iEffects.end()){ delete *it;++it;};
	iEffects.clear();

	std::list<CEffect*>::iterator it2 = iNewEffects.begin();
	while (it2 != iNewEffects.end()){ delete *it2;++it2;};
	iNewEffects.clear();
	delete iIngameData;
	ResetDeadPlayers();
}

std::list< CEffect* >& CLevelRuntimeData::Effects()
{
	return iEffects;
}

std::vector< CBodyPart >& CLevelRuntimeData::Bodyparts()
{
	return iBodyparts;
}

std::vector< CEnemy >& CLevelRuntimeData::Enemies()
{
	return iEnemies;
}

std::list< CBullet >& CLevelRuntimeData::Bullets()
{
	return iBullets;
}

std::list< CCrate* >& CLevelRuntimeData::Crates()
{
	return iCrates;
}

std::vector<CPlayer *>& CLevelRuntimeData::DeadPlayers()
{
	return iDeadPlayers;
}

void CLevelRuntimeData::RollEffects() 
{
	std::list< CEffect* >::iterator iter = iEffects.begin();
	int amount = iEffects.size();
	int cnt=0;

	while( iter != iEffects.end() )
	{
		if ( (*iter)->Roll( this,amount ) )
		{
			delete (*iter);
			iter = iEffects.erase( iter );
			++cnt;
		}
		else
		{
			++cnt;
			++iter;
		}
	}
	iSteamAnimationCount++;
}

void CLevelRuntimeData::MoveBullets() 
{
	std::list<CBullet>::iterator ptr=iBullets.begin();

	while(ptr != iBullets.end())
	{
		if (ptr->Move())
		{
			std::list<CBullet>::iterator del=ptr++;
			iBullets.erase( del );
		}
		else
			ptr++;

	}
}

void CLevelRuntimeData::EnemyUpdate() 
{
	std::vector< CEnemy >::iterator iter = iEnemies.begin();

	while(iter != iEnemies.end() )
	{
		iter->DecideAndMove();

		if (iter->iPushPower>0) 
		{
			iter->Move(iter->iPushAngle, iter->iPushPower);
			iter->iPushPower -= 0.075f;
		}
		iter->EnemyCheck();
		iter->Animate();
		++iter;
	}
}

void CLevelRuntimeData::MoveBodyParts() 
{
	std::vector< CBodyPart >::iterator iter = iBodyparts.begin();

	while(iter != iBodyparts.end() )
	{
		iter->Move( this );
		++iter;
	}
}

void CLevelRuntimeData::ChkWakeUps() 
{
	std::list< CCrate* >::iterator iter = iCrates.begin();

	while(iter != iCrates.end() )
	{
		if ((*iter)->InUse()==0) 
			(*iter)->CheckWakeUp();
		++iter;
	}
}

void CLevelRuntimeData::MoveCrateTexts() 
{
	std::list< CCrate* >::iterator iter = iCrates.begin();

	while(iter != iCrates.end() )
	{
		if ((*iter)->InUse()&&(*iter)->TextCounter()>0) 
			(*iter)->MoveText();
		++iter;
	}
}

void CLevelRuntimeData::ResetCrates() 
{
	std::list< CCrate* >::iterator iter = iCrates.begin();

	while(iter != iCrates.end())
	{
		delete (*iter);
		iter++;
	}

	iCrates.clear();
}

void CLevelRuntimeData::ResetBullets() 
{
	iBullets.clear();
}

void CLevelRuntimeData::ResetDeadPlayers()
{
	for (int a=0;a<iDeadPlayers.size();a++)
		delete iDeadPlayers[a];

	iDeadPlayers.clear();	
}
	
void CLevelRuntimeData::NewBodyPart(const CCoord<float>& aPos, int aAngle, float aSpeed, int aNum ) 
{
	ASSERT( aNum >= 0 );
	ASSERT( aNum < EBodyPartAmount );
	iBodyparts.push_back(CBodyPart(aPos,aAngle,aSpeed,iGameData->TypeData()->iBodyPartType[ aNum ], iSP ));
}

void CLevelRuntimeData::NewEffect(int aType,const CCoord<float>& aPos, int aAngle, float iSpeed ) 
{
	if ( aType == EEffectNoEffect )
		return;

	ASSERT( aType >= 0 );
	ASSERT( aType < EEffectAmount );

	CRect<float> r(0,0,iLevel.Width()*KBlockSpriteSize,iLevel.Height()*KBlockSpriteSize);
	if (r.Inside( aPos ))
		iNewEffects.push_back( new CEffect(iGameData->TypeData()->iEffectType[ aType ],aPos,0,aAngle,iSpeed));
}

void CLevelRuntimeData::NewBullet( int aType, const CCoord<float>& aPos, int aAngle, int aTimeAdd, float aShare, IBulletObserver* aObserver )
{
	CCoord<int> realPos = aPos;
	int cnt=1;

	ASSERT( aType >= 0 );
	ASSERT( aType < EBulletAmount );

	while ( iLevel.Collision( realPos.Div(20) ) && cnt < KBlockSpriteSize*1.4) // 1.4 on siis kulmittain kerroin (sqrt(2))
	{
		realPos = aPos.AddMovement( aAngle, -cnt ); // backwards
		cnt++;
	}

	if (cnt >= KBlockSpriteSize*1.4 ) // failure
	{
		realPos = aPos;
	}

	iBullets.push_back(CBullet(iGameData->TypeData()->iBulletType[ aType ],realPos,aObserver,aAngle, aTimeAdd, aShare, iDynData, iSP, iGameData));
}

void CLevelRuntimeData::NewEnemy(int aType, bool aWaitStill,const CCoord<int>& aPos, int aAngle)
{
	ASSERT( aType >= 0 );
	ASSERT( aType < EEnemyAmount );

	iEnemies.push_back( CEnemy(aType,aWaitStill,aPos,aAngle, iGS,iGameData, iGS) );
}

/* iX,iY, iType, iCurrentWeapon, iBullet, iEnergy)*/
void CLevelRuntimeData::NewCrate(const CCoord<int>& aPos,int aType,int aValue)
{
	CCrate* ptr;
	
	switch ( aType )
	{
		case CLevel::EWeaponCrate: ptr = new CWeaponCrate( aPos, iGameData->TypeData()->iWeaponType [ aValue+1 ], iGS->GUIStateController() ); break;
		case CLevel::EBulletCrate: ptr = new CBulletCrate( aPos, iGameData->TypeData()->iBulletType [ aValue+1 ], iGS->GUIStateController() ); break;
		case CLevel::EHealthCrate: ptr = new CEnergyCrate( aPos, iGS->GUIStateController() ); break;
		case CLevel::EMoneyCrate: ptr = new CMoneyCrate( aPos, aValue, iGS->GUIStateController() ); break;
		case CLevel::ETargetCrate: ptr = new CTargetSystemCrate( aPos, iGS->GUIStateController() ); break;
		default: error( "CDynGameData::NewCrate: Invalid crate type: %d\n", aType );
	};

	iCrates.push_back( ptr );
}

void CLevelRuntimeData::Explo(const CCoord<int>& aPos,int aSize, float aShare, IBulletObserver* aObserver )
{
	int a;
	const int ray=aSize*5;

	for(a = 0; a < ray; a++)
		NewBullet(EBulletExplosionRay,aPos,(int)(a*360.0f/ray),(int)(aSize * 2.5f), aShare / ray, aObserver );

	int dst = (rand()%5)+1;
	for(a = 0; a < dst; a++)
		NewEffect( rand()%2 ? EEffectMetalYellow : EEffectMetalRed,aPos,rand()%360,(rand()%2)+2);

	// move bodyparts...
	int push_dist = 25*aSize;

	CCoord<float> left = aPos.SubX(push_dist*2);
	CCoord<float> right = aPos.AddX(push_dist*2);

	std::vector< CBodyPart >::iterator low = lower_bound(iBodyparts.begin(),iBodyparts.end(),left,SortOperator);
	std::vector< CBodyPart >::iterator high = upper_bound(iBodyparts.begin(),iBodyparts.end(),right,SortOperator);

	while(low != high)
	{
		CCoord<float> vec;
		vec = low->Location().Sub( aPos );
		float dist = vec.VectorLength();

		if (dist < push_dist*2)
		{
			if ( iLevel.VisibilityCheck( aPos, low->Location() ) )
				if (dist)
				{
					dist *=dist/2;
					vec.SetX( (rand()%push_dist)*vec.X()/dist );
					vec.SetY( (rand()%push_dist)*vec.Y()/dist );
					low->Push( vec );
				}
		}

		low++;
	}
}

void CLevelRuntimeData::InitEnemies() 
{
	int a,b,c=0;
	bool fuckUp = false;
	bool randFuckUp = false;

	DEBUG0("CLevelRuntimeData::InitEnemies: Placing fixed enemies\n");

	for(a=0; a<iLevel.iEnemy.size(); a++) 
	{
		CCoord<int> pos(iLevel.iEnemy[a].iX,iLevel.iEnemy[a].iY);
		if ( iDynData->TestEnemyLocation( pos, false ) )
			NewEnemy(iLevel.iEnemy[a].iType,1,pos, rand() % 360);
		else
		{
			LOG2("Failed to initialize enemy: %s at %s",iGameData->TypeData()->iEnemyType[ iLevel.iEnemy[a].iType ]->Name(),pos.String().c_str() );
			fuckUp = true;
		}
	}

	DEBUG0("CLevelRuntimeData::InitEnemies: Placing random enemies\n");

	for(b=0; b<EEnemyAmount&& !randFuckUp; b++) 
	{
		for(a=0; a<iLevel.iGeneralLevelInfo.iEnemies[b]&& !randFuckUp; a++) 
		{
			randFuckUp = PlaceEnemy( b );
		}
	}

	CMessageBoard* mb = iGS->GUIStateController()->MessageBoard();

	if ( fuckUp )
	{
		mb->AddMessage("Error: Some enemies could not be placed in the game.");
		mb->AddMessage("Enemies are placed in invalid positions.");
	}

	if ( randFuckUp )
	{
		mb->AddMessage("Error: Enemies cannot be placed. Propably too many random");
		mb->AddMessage("enemies in the game.");
	}

	if ( fuckUp || randFuckUp )
	{
		mb->AddMessage("Level is faulty. Contact author of the level!");
	}
}

std::vector< CEnemy >::iterator CLevelRuntimeData::KillEnemy( std::vector< CEnemy >::iterator& aIter )
{
	iDeadEnemies.push_back( aIter->Location() );
	return iEnemies.erase( aIter );
}

const std::list< CCoord<int> >& CLevelRuntimeData::DeadEnemies() const
{
	return iDeadEnemies;
}


void CLevelRuntimeData::PerFrameCalc()
{
	// do real addition of effects
	int amount = iEffects.size() + iNewEffects.size();

	if (amount > KMaxEffectsInGame)
	{
		std::list< CEffect* >::iterator iti = iEffects.begin();

		while (amount>KMaxEffectsInGame)
		{
			delete (*iti);
			++iti;
			--amount;
		}

		iEffects.erase( iEffects.begin(), iti );
	}
	//else
	//	iEffects.reserve( amount );
	std::list<CEffect*>::iterator it = iNewEffects.begin();
	while (it != iNewEffects.end())
	{
		iEffects.push_back(*it);
		++it;
	};
	iNewEffects.clear();
	// end

	sort( iEnemies.begin(), iEnemies.end(), SortOperator );
	sort( iBodyparts.begin(), iBodyparts.end(), SortOperator );
	//sort( iEffects.begin(), iEffects.end(), PtrSortOperator< CEffect > );

	MoveBullets();
	MoveBodyParts();
	MoveCrateTexts();
}

bool CLevelRuntimeData::PlaceEnemy( int aType ) 
{
	bool done = false;
	CCoord<int> newPos;
	int tryno;

	tryno=0;
	while(done==0) 
	{
		tryno++;
		if (tryno>=KTooManyTries)
			return true;
		newPos.Set((rand()%(iLevel.Width() - 2))+1,(rand()%(iLevel.Height() - 2))+1);
		newPos = newPos.Mul(KBlockSpriteSize);

		done = iDynData->TestEnemyLocation( newPos, true );
	}

	NewEnemy(aType,0,newPos, rand() % 360);
	return false;
}

void CLevelRuntimeData::PlaceGameCrates( bool aDM ) 
{
	int a;
	
	std::vector< CLevel::TCrateInfo >* placedcrates = &iLevel.iPlacedCratesNormal;

	if ( aDM )
		placedcrates = &iLevel.iPlacedCratesDM;

	for(a=0;a<placedcrates->size();a++)
	{
		NewCrate(CCoord<int>((*placedcrates)[a].iX,(*placedcrates)[a].iY),(*placedcrates)[a].iType1,(*placedcrates)[a].iType2);
	}
	
	std::vector< CLevel::TRandomCrateInfo >* crates = &iLevel.iRandomCratesNormal;

	if ( aDM )
		crates = &iLevel.iRandomCratesDM;

	for(a=0; a<crates->size(); a++)
		for (int b=0;b<(*crates)[a].iAmount;b++)
		{
			int x=0; 
			int y=0;

			do 
			{
				x=rand()%(iLevel.Width()*KBlockSpriteSize); 
				y=rand()%(iLevel.Height()*KBlockSpriteSize);
			}while(iDynData->CheckCratePlace(x,y,14));
			/* x,y, iType, iCurrentWeapon, bullet, iEnergy (aseta tc,tc2 inuse)*/
			NewCrate(CCoord<int>(x,y),(*crates)[a].iType1,(*crates)[a].iType2);
		}
}
