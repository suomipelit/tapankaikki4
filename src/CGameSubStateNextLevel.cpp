#include <vector>
#include "CGameSubStateNextLevel.h"
#include "CGameState.h"
#include "IGameSubStateController.h"
#include "CDynGameData.h"
#include "IGUIStateController.h"


CGameSubStateNextLevel::CGameSubStateNextLevel(IGameSubStateController* aSubStateController): iStateController (aSubStateController)
{
}

CGameSubStateNextLevel::~CGameSubStateNextLevel()
{
}

// return true if draw is needed
bool CGameSubStateNextLevel::RunFrame()
{
	iStateController->DynData()->SetupLevelStart();

	if (iStateController->DynData()->NetworkMode()!=ENetworkModeClient)
		iStateController->GUIStateController()->State()->SetState(CState(CState::EMainStateGame, EGameStateLevelInfo));
	else
		iStateController->GUIStateController()->State()->SetState(CState(CState::EMainStateGame, EGameStateOnGoing));

	return false;
}

void CGameSubStateNextLevel::Draw(CDrawArea& aDirtyArea,CDrawArea& aDrawArea,CGameGraphicsInterface* aGGI)
{
}

void CGameSubStateNextLevel::EnterState()
{
}

void CGameSubStateNextLevel::ExitState()
{
}

