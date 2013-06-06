#include "CGameSubStateQuit.h"

#include "common/CEventHandler.h"
#include "common/CFonts.h"

#include "CGameGraphicsInterface.h"
#include "IGameSubStateController.h"
#include "CGameState.h"
#include "CDynGameData.h"
#include "IGUIStateController.h"
#include "timer.h"

CGameSubStateQuit::CGameSubStateQuit(IGameSubStateController* aSubStateController) : iGameSubStateController(aSubStateController)
{
	iEH = aSubStateController->GUIStateController()->EventHandler();
	iDynData = aSubStateController->DynData();
	iGGI = aSubStateController->GUIStateController()->GGI();
	iGameState = aSubStateController->GUIStateController()->State();
}

CGameSubStateQuit::~CGameSubStateQuit()
{
}

// return true if draw is needed
bool CGameSubStateQuit::RunFrame()
{
	if (iEH->Kbhit())
	{
		SDLKey key=iEH->Getch().sym;
		if (key!=SDLK_y)
		{
			restoreTimer();
			iGameState->RestoreState();
			iGameState->First(); // We don't need new fadein
			iEH->GrabInputs(true);
		}
		else
		{
			restoreTimer();
			iGameState->RestoreState();
			if (iGameState->SubState()!=EGameStateOnGoing)
				iGameSubStateController->BackToMenu();
			iDynData->iUserQuit=true;
		}

		return true;
	}

	return false;
}

void CGameSubStateQuit::Draw(CDrawArea& aDirtyArea,CDrawArea& aDrawArea,CGameGraphicsInterface* aGGI)
{
	char quitstr[]="REALLY WANNA QUIT ?(y)";

	iRect = iGGI->Font(CGameGraphicsInterface::ESmallFont)->Write(iGGI->GD()->Width()/2,(iGGI->GD()->Height()/2),quitstr,CFonts::EDrawInVertMiddle, CFonts::EDrawInMiddle, iGGI->DrawBuffer());
	aDrawArea.Combine(iRect);
}

void CGameSubStateQuit::EnterState()
{
	iRect = iGGI->DrawBuffer()->Rect();
}

void CGameSubStateQuit::ExitState()
{
}

