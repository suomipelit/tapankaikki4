#include "CGameSubStateStatistics.h"

#include "common/CEventHandler.h"
#include "common/CFonts.h"
#include "common/IEnemyType.h"

#include "CLevelRuntimeData.h"
#include "CGameGraphicsInterface.h"
#include "IGameSubStateController.h"
#include "IGUIStateController.h"
#include "CGameState.h"
#include "CDynGameData.h"
#include "CGameDrawManager.h"
#include "CGameStatisticsData.h"
#include "CStatViewData.h"
#include "CSoundPlayer.h"
#include "IGameMode.h"

namespace
{
	// TODO: CONFIGFILE
	const char KStatisticsGameOverText[]="Game Over";
	const char KStatisticsCompletedText[]="Completed !";
	const int KTempStringLength = 120;
};

CGameSubStateStatistics::CGameSubStateStatistics(IGameSubStateController* aSubStateController): iStateController (aSubStateController)
{
	iDynData = aSubStateController->DynData();
	iGUIStateController = aSubStateController->GUIStateController();
	iEventHandler = iGUIStateController ->EventHandler();
	iGameData = iGUIStateController->GameData();
	iGameDrawManager = aSubStateController->GameDrawManager();
	iGGI = iGUIStateController->GGI();
}

CGameSubStateStatistics::~CGameSubStateStatistics()
{
}

// return true if draw is needed
bool CGameSubStateStatistics::RunFrame()
{
	if (iGUIStateController->State()->First())
	{
		iEventHandler->ResetStack();
		iStateController->GUIStateController()->StartFadeIn(CGameData::EPaletteShop);
		iEventHandler->GrabInputs(false); // return mouse to user
	}

	if (iEventHandler->Kbhit())
	{
		SDL_keysym key = iEventHandler->Getch();

		if ( GetNextPlayer(2) == -1  )
		{
			iDynData->StatViewData().SetDone();

			if (iDynData->GameMode()->GameOver() || iDynData->iUserQuit)
			{
				iStateController->BackToMenu();
			}
			else
			{
				if (iDynData->CurrentLevel()>=iDynData->LevelsInEpisode() )/* Check if game's through*/
					iStateController->GUIStateController()->StartFadeOut(CState(CState::EMainStateGame,EGameStateVictory));
				else
					iStateController->GUIStateController()->StartFadeOut(CState(CState::EMainStateGame,EGameStateNextLevel));
			}
		}			
		else // show next statistics screen
		{
			iStateController->GUIStateController()->StartFadeOut(CState(CState::EMainStateGame,EGameStateStatistics));
			iDynData->StatViewData().SetFirstPlayer( GetNextPlayer(2) );
		}
	}
	return false;
}

void CGameSubStateStatistics::Draw(CDrawArea& aDirtyArea,CDrawArea& aDrawArea,CGameGraphicsInterface* aGGI)
{
	const CGraphicsBuffer* gb=iGameData->MenuSprite(EMenuSpriteStatisticsBack);
	const int KTempStringLength = 120;

	const int XSpare = 10;
	const char *str=NULL;

	char text[KTempStringLength];

	int ply1 = GetNextPlayer(0);
	int ply2 = GetNextPlayer(1);

	ASSERT( ply1 != -1 );

	aGGI->DrawBuffer()->Copy(0,0,gb);
	aDrawArea.Combine( aGGI->DrawBuffer()->Rect() );

	if (! iDynData->GameMode()->GameOver() && !iDynData->iUserQuit) 
		str=KStatisticsCompletedText;
	if ( iDynData->iUserQuit || iDynData->GameMode()->GameOver() )
		str=KStatisticsGameOverText;

	ASSERT( str );

	aGGI->Font(CGameGraphicsInterface::EBigMenuFont)->Write(
		(gb->Width()/2), 
		20,
		str,
		CFonts::EDrawInVertMiddle, CFonts::EDrawInMiddle, 
		aGGI->DrawBuffer());

	DrawPlayerInfo( 0, ply1, aGGI );
	if ( ply2 >= 0 )
		DrawPlayerInfo( 1, ply2, aGGI );

	if ( iDynData->GameMode()->ActionTimeMatters() )
	{
		if ( iTimeBonus > 0 )
		{
			ASSERT(_snprintf(text,KTempStringLength,"Time limit: %s Your time: %s Time bonus: +%d",
				iGameDrawManager->FormatTime((int)(iDynData->LevelRuntime()->Level().iGeneralLevelInfo.iTimeLimit)).c_str(),
				iGameDrawManager->FormatTime(iDynData->iCompleteTime).c_str(),
				iTimeBonus)>0);
		}
		else
		{
			if ( iTimeBonus < 0 )
			{
				ASSERT(_snprintf(text,KTempStringLength,"Time limit: %s Your time: %s Time penalty: %d",
					iGameDrawManager->FormatTime((int)(iDynData->LevelRuntime()->Level().iGeneralLevelInfo.iTimeLimit)).c_str(),
					iGameDrawManager->FormatTime(iDynData->iCompleteTime).c_str(),
					iTimeBonus)>0);
			}
			else
			{
				ASSERT(_snprintf(text,KTempStringLength,"Time limit: %s Your time: %s",
					iGameDrawManager->FormatTime((int)(iDynData->LevelRuntime()->Level().iGeneralLevelInfo.iTimeLimit)).c_str(),
					iGameDrawManager->FormatTime(iDynData->iCompleteTime).c_str())>0);
			}
		}

		aGGI->Font(CGameGraphicsInterface::ESmallMenuFontDark)->Write(16, (int)(aGGI->GD()->Height()*0.8f), text,CFonts::EDrawBelow, CFonts::EDrawToRight, aGGI->DrawBuffer());
	}
}


void CGameSubStateStatistics::DrawPlayerInfo( int aPartOfScreen, int aPlayerNo, CGameGraphicsInterface* aGGI )
{
	int a,b;
	const int horizStart = aPartOfScreen*aGGI->DrawBuffer()->Width()/2;
	char text[KTempStringLength];
	const int level = iDynData->Statistics()->LatestLevel();

	ASSERT( aPlayerNo >= 0);
	ASSERT( aPartOfScreen == 0 || aPartOfScreen == 1 );

	CGameStatisticsData::TPlayerLevelStats& stats 
	= iDynData->Statistics()->PlayerStats( aPlayerNo ).LevelStats( level );
	CGameStatisticsData::TPlayerStats& plyStats = iDynData->Statistics()->PlayerStats( aPlayerNo );
	int plyKills = stats.TotalPlayerKills(plyStats.iPlayer);

	if (stats.TotalKills()>0) 
		b=1;
	else
		b=0;

	if (b)
		ASSERT(_snprintf(text,KTempStringLength, "Kills for %s", plyStats.iPlayer->iName)>0);
	if (!b) // ASSERT fuk-up
		ASSERT(_snprintf(text,KTempStringLength, "No kills for %s", plyStats.iPlayer->iName)>0);

	aGGI->Font(CGameGraphicsInterface::ESmallMenuFontDark)->Write(16+horizStart, 60, text,CFonts::EDrawBelow, CFonts::EDrawToRight, aGGI->DrawBuffer());

	for (a = 0, b = 60+aGGI->Font(CGameGraphicsInterface::ESmallMenuFontDark)->Height()*2; a < EEnemyAmount; a++) 
		if (stats.iEnemyKills[a] > 0) 
		{
			ASSERT(_snprintf(text,KTempStringLength, "%d %s%s",
				stats.iEnemyKills[a],
				iGameData->TypeData()->iEnemyType[a]->Name(),
				stats.iEnemyKills[a] > 1? "s":"")>0);

			aGGI->Font(CGameGraphicsInterface::ESmallMenuFontDark)->Write(16+horizStart, b, text,CFonts::EDrawBelow, CFonts::EDrawToRight,  aGGI->DrawBuffer());

			if ( iDynData->GameMode()->ActionTimeMatters() )
			{
				ASSERT(_snprintf(text,KTempStringLength,"+%d", iGameData->TypeData()->iEnemyType[a]->Reward()*stats.iEnemyKills[a])>0);
				aGGI->Font(CGameGraphicsInterface::ESmallMenuFontDark)->Write(220+horizStart, b, text,CFonts::EDrawBelow, CFonts::EDrawToRight, aGGI->DrawBuffer());
			}
			b+=aGGI->Font(CGameGraphicsInterface::ESmallMenuFontDark)->Height();
		}

	if ( plyKills && iDynData->GameMode()->Frags() )
	{
		b+=aGGI->Font(CGameGraphicsInterface::ESmallMenuFontDark)->Height();
        ASSERT(_snprintf(text,KTempStringLength,"%d Frags",plyKills)>0);
		aGGI->Font(CGameGraphicsInterface::ESmallMenuFontDark)->Write(16+horizStart, b,text,CFonts::EDrawBelow, CFonts::EDrawToRight,  aGGI->DrawBuffer());
		b+=aGGI->Font(CGameGraphicsInterface::ESmallMenuFontDark)->Height();

		ASSERT(_snprintf(text,KTempStringLength,"Total kills: %d",stats.TotalKills()+plyKills)>0);
	}
	else
	{
        ASSERT(_snprintf(text,KTempStringLength,"Total kills: %d",stats.TotalKills())>0);
	}

	b+=aGGI->Font(CGameGraphicsInterface::ESmallMenuFontDark)->Height();

	aGGI->Font(CGameGraphicsInterface::ESmallMenuFontDark)->Write(16+horizStart, b,text,CFonts::EDrawBelow, CFonts::EDrawToRight,  aGGI->DrawBuffer());
	b+=aGGI->Font(CGameGraphicsInterface::ESmallMenuFontDark)->Height();

	ASSERT(_snprintf(text,KTempStringLength,"Accuracy: %.1f%%",stats.Accuracy())>0);

	aGGI->Font(CGameGraphicsInterface::ESmallMenuFontDark)->Write(16+horizStart, b, text,CFonts::EDrawBelow, CFonts::EDrawToRight, aGGI->DrawBuffer());
	b+=aGGI->Font(CGameGraphicsInterface::ESmallMenuFontDark)->Height();

	if ( iDynData->GameMode()->ActionTimeMatters() ) // TODO: korjaa joskus
	{
		ASSERT( plyStats.iPlayer->iCash >= 0 );
		ASSERT(_snprintf(text,KTempStringLength,"Cash: %d",plyStats.iPlayer->iCash)>0);
		aGGI->Font(CGameGraphicsInterface::ESmallMenuFontDark)->Write(16+horizStart, b, text,CFonts::EDrawBelow, CFonts::EDrawToRight, aGGI->DrawBuffer());
	}
}

void CGameSubStateStatistics::EnterState()
{
	if (iDynData->StatViewData().Done())
	{
		iDynData->StatViewData().Reset();
		GivePlayerPrize();
		iStateController->GUIStateController()->SoundPlayer()->MenuMusic();
	}
}

void CGameSubStateStatistics::ExitState()
{
}

void CGameSubStateStatistics::GivePlayerPrize()
{
	iTimeBonus = ((int)(iDynData->LevelRuntime()->Level().iGeneralLevelInfo.iTimeLimit)-iDynData->iCompleteTime) *5;
	if (iTimeBonus <-500) iTimeBonus =-500;

	LOG0("CGameSubStateStatistics::GivePlayerPrize():\n");

	// in these cases, no time bonus
	if ( iDynData->iUserQuit || iDynData->GameMode()->GameOver() )
	{
		iTimeBonus = 0;
		LOG0("user quit or all players dead, no bonus\n");
	}

	LOG3("TimeLimit: %d\nCompleteTime: %d\nTimebonus: %d\n",
		iDynData->LevelRuntime()->Level().iGeneralLevelInfo.iTimeLimit,
		iDynData->iCompleteTime,
		iTimeBonus);

	for (int p=0;p<iDynData->LocalPlayers().size();p++)
	{		
		CPlayer* pl = iDynData->LocalPlayers()[p];
		CGameStatisticsData::TPlayerLevelStats& stats = iDynData->Statistics()->PlayerStats( pl ).LevelStats( 
			iDynData->Statistics()->LatestLevel() );

		LOG2("PL %d:\n Money before: %d\n",p,pl->iCash);
	
		for (int a = 0; a < EEnemyAmount; a++)
		{
			ASSERT(iGameData->TypeData()->iEnemyType[a]->Reward()*stats.iEnemyKills[a]>=0);
			pl->iCash += iGameData->TypeData()->iEnemyType[a]->Reward()*stats.iEnemyKills[a];
		}

		pl->iCash += iTimeBonus;

		if (pl->iCash < 0) 
			pl->iCash = 0;

		LOG1("Money after: %d\n",pl->iCash);
	}
}

int CGameSubStateStatistics::GetNextPlayer(int aIndex)
{
	DEBUG_ASSERT( aIndex>=0 );

	for (int a = iDynData->StatViewData().FirstPlayer();a<iDynData->Statistics()->Players();a++)
	{
		if (iDynData->Statistics()->PlayerStats( a ).iLevelOfDeath == -1 ||
			iDynData->Statistics()->PlayerStats( a ).iLevelOfDeath == iDynData->Statistics()->LatestLevel())
		{
			if ( aIndex == 0)
				return a;
			else 
				aIndex--;
		}
	}

	return -1;
}

