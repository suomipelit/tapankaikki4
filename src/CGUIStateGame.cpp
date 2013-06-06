#include "CGUIStateGame.h"
#include "IGUIStateController.h"
#include "IGameSubStateController.h"

CGUIStateGame::CGUIStateGame(IGUIStateController* aStateController) : iStateController(aStateController)
{
}

bool CGUIStateGame::RunFrame()
{
	return iStateController->GameSystem()->RunFrame();
}

void CGUIStateGame::Draw(CDrawArea& aDirtyArea,CDrawArea& aDrawArea,CGameGraphicsInterface* aGGI)
{
	iStateController->GameSystem()->DrawFrame(aDirtyArea,aDrawArea,aGGI);
}

void CGUIStateGame::EnterState()
{
	iStateController->GameSystem()->EnterState();
}
void CGUIStateGame::ExitState()
{
	iStateController->GameSystem()->ExitState();
}

void CGUIStateGame::EnterSubState()
{
	iStateController->GameSystem()->EnterSubState();
};

void CGUIStateGame::ExitSubState()
{
	iStateController->GameSystem()->ExitSubState();
};

