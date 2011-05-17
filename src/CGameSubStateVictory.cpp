#include "common/CEventHandler.h"

#include "CGameSubStateVictory.h"
#include "CGameGraphicsInterface.h"
#include "IGameSubStateController.h"
#include "IGUIStateController.h"
#include "CGameState.h"

CGameSubStateVictory::CGameSubStateVictory(IGameSubStateController* aSubStateController): iStateController (aSubStateController)
{
	iEventHandler = aSubStateController->GUIStateController()->EventHandler();
}

CGameSubStateVictory::~CGameSubStateVictory()
{
}

// return true if draw is needed
bool CGameSubStateVictory::RunFrame()
{
	if (iStateController->GUIStateController()->State()->First())
	{
		iStateController->GUIStateController()->StartFadeIn(CGameData::EPaletteVictory);
	}

	if (iEventHandler->Kbhit())
	{
		iEventHandler->Getch();
		iStateController->BackToMenu();
	}

	return false;
}

void CGameSubStateVictory::Draw(CDrawArea& aDirtyArea,CDrawArea& aDrawArea,CGameGraphicsInterface* aGGI)
{
	aGGI->DrawBuffer()->Copy(iStateController->GUIStateController()->GameData()->MenuSprite(EMenuSpriteVictoryScreen));
	aDrawArea.Combine( aGGI->DrawBuffer()->Rect() );
}

void CGameSubStateVictory::EnterState()
{
	iEventHandler->GrabInputs(false); // return mouse to user
	iEventHandler->ResetStack();
}

void CGameSubStateVictory::ExitState()
{
}

