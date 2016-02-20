#include "CGameSubStateLevelInfo.h"

#include "common/CEventHandler.h"
#include "common/CFonts.h"
#include "common/CDrawArea.h"
#include "common/IEnemyType.h"

#include "IGameSubStateController.h"
#include "IGUIStateController.h"
#include "CDynGameData.h"
#include "CGameGraphicsInterface.h"
#include "CGameState.h"
#include "CGameDrawManager.h"
#include "timer.h"
#include "CLevelRuntimeData.h"
#include "IGameMode.h"

CGameSubStateLevelInfo::CGameSubStateLevelInfo(IGameSubStateController* aSubStateController): iStateController (aSubStateController)
{
	iEventHandler = aSubStateController->GUIStateController()->EventHandler();
	iDynData = aSubStateController->DynData();
	iGameState = aSubStateController->GUIStateController()->State();
}

CGameSubStateLevelInfo::~CGameSubStateLevelInfo()
{
}

// return true if draw is needed
bool CGameSubStateLevelInfo::RunFrame()
{
	SDLKey key;
	bool exit=false; // Used for controlling state

	/* Check for almighty quit*/ 
	if (iEventHandler->Kbhit())
	{
		key=iEventHandler->Getch().sym;

		// ESC was pressed -> Quit window
		if (key==SDLK_ESCAPE)
		{
			iGameState->SetStateAndSave(CState(CState::EMainStateGame,EGameStateQuit));
			saveTimer();
			
			iEventHandler->ResetStack();

			return true; // we need manual redraw
		}
		else 
			exit=true;
	}

	if (exit)
	{
		if ( EF_DEBUG ||
			iDynData->GameMode()->ShopEnabled(iDynData->CurrentLevel()) )
			iStateController->GUIStateController()->StartFadeOut(CState(CState::EMainStateGame,EGameStateShop));
		else 
			iStateController->GUIStateController()->StartFadeOut(CState(CState::EMainStateGame,EGameStateOnGoing));
	}

	return false;
}

void CGameSubStateLevelInfo::Draw(CDrawArea& aDirtyArea,CDrawArea& aDrawArea,CGameGraphicsInterface* aGGI)
{
	// Full screen update
	aDrawArea.Combine(aGGI->DrawBuffer()->Rect());

	CGameData* gameData = iStateController->GUIStateController()->GameData();
	COptions* options = iStateController->GUIStateController()->Options();
	
	const CGraphicsBuffer* picture=gameData->MenuSprite(EMenuSpriteMenuBG);

	const int BoxExtra = 3;
	const int MainTitleStartY = 13;
	const int MainTitleWidth = 100;
	const int LevelTitleStartY = 13+40;
	const int BoxWidthMinus = 10;

	char text[120];
	int a, b,c,total;
	const int enemylistStart = 96+aGGI->Font(CGameGraphicsInterface::ESmallMenuFontDark)->Height()*2;

	aGGI->DrawBuffer()->Copy(picture);

	ASSERT(EF_SNPRINTF(text,120, "Level %d", iDynData->CurrentLevel()+1)>0);

	aGGI->Font(CGameGraphicsInterface::EBigMenuFont)->Write(16, MainTitleStartY, text, CFonts::EDrawBelow, CFonts::EDrawToRight, aGGI->DrawBuffer());
	aGGI->Font(CGameGraphicsInterface::EBigMenuFont)->Write(
		aGGI->GD()->Width()/2, 
		LevelTitleStartY, 
		iDynData->LevelRuntime()->Level().iGeneralLevelInfo.iComment,
		CFonts::EDrawBelow, CFonts::EDrawInMiddle, 
		aGGI->DrawBuffer());
	
	aGGI->Font(CGameGraphicsInterface::ESmallMenuFontDark)->Write(16, 96, "The killing list:",CFonts::EDrawBelow, CFonts::EDrawToRight, aGGI->DrawBuffer());
	b = enemylistStart;

	if (!options->Data().iEnemiesOnGame && iDynData->GameMode()->DisableEnemiesPossible() )
	{
		aGGI->Font(CGameGraphicsInterface::ESmallMenuFontDark)->Write(16, b, "Enemies disabled",CFonts::EDrawBelow, CFonts::EDrawToRight,aGGI->DrawBuffer());
		b += aGGI->Font(CGameGraphicsInterface::ESmallMenuFontDark)->Height();

	}
	else
	{
		for (a = 0; a < EEnemyAmount; a ++) 
		{
			for (c=0,total=0;c<iDynData->LevelRuntime()->Level().iEnemy.size();c++)
				if (iDynData->LevelRuntime()->Level().iEnemy[c].iType==a) 
					total++;
	        
			total+=iDynData->LevelRuntime()->Level().iGeneralLevelInfo.iEnemies[a];
	        
			if (total>0)
			{
				ASSERT(EF_SNPRINTF(text,120,"%d %s%s",total, gameData->TypeData()->iEnemyType[a]->Name(), total>1 ? "s":"")>0);
				aGGI->Font(CGameGraphicsInterface::ESmallMenuFontDark)->Write(16, b, text,CFonts::EDrawBelow, CFonts::EDrawToRight, aGGI->DrawBuffer());
				b += aGGI->Font(CGameGraphicsInterface::ESmallMenuFontDark)->Height();
			}
		}

		if (iDynData->GameMode()->ActionTimeMatters() )
		{
			ASSERT(EF_SNPRINTF(text,120,"Your action time is: %s", iStateController->GameDrawManager()->FormatTime((int)(iDynData->LevelRuntime()->Level().iGeneralLevelInfo.iTimeLimit)).c_str())>0);
			aGGI->Font(CGameGraphicsInterface::ESmallMenuFontDark)->Write(16, b+50,text,CFonts::EDrawBelow, CFonts::EDrawToRight, aGGI->DrawBuffer());
		}
	}
}

void CGameSubStateLevelInfo::EnterState()
{
	iStateController->GUIStateController()->EventHandler()->ResetStack();
	iStateController->GUIStateController()->StartFadeIn(CGameData::EPaletteMenu);
}

void CGameSubStateLevelInfo::ExitState()
{
}

