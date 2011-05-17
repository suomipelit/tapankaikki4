#include "common/debug.h"

#include "CGameState.h"
#include "IGUIStateController.h"
#include "CGameSystem.h"
#include "IGUIState.h"
#include "CGUIStateLogo.h"
#include "CGUIStateQuit.h"
#include "CGameMenuContainer.h"
#include "CGUIStateGame.h"

CGameState::CGameState(IGUIStateController* aStateController): 
	iState(CState::EMainStateLogo1,0), 
	iStateChanged(true),
	iSave(CState::EMainStateLogo1,0),
	iEffectOngoing(false),
	iStateController(aStateController)
{
	iGUIStates[CState::EMainStateLogo1] = new CGUIStateLogo(CGameData::EPaletteEFLogo,aStateController->GameData()->MenuSprite(EMenuSpriteEFLogo),true,CState(CState::EMainStateLogo2,0),aStateController);
	iGUIStates[CState::EMainStateLogo2] = new CGUIStateLogo(CGameData::EPaletteTKLogo,aStateController->GameData()->MenuSprite(EMenuSpriteTKLogo),false,CState(CState::EMainStateMenu,0),aStateController);
	iGUIStates[CState::EMainStateMenu] =  new CGameMenuContainer(aStateController);
	iGUIStates[CState::EMainStateGame] =  new CGUIStateGame(aStateController);
	iGUIStates[CState::EMainStateQuit] = new CGUIStateQuit(aStateController);
	iGUIStates[CState::EMainStateConfirmedQuit] = NULL;
	SELFTEST;
}

CGameState::~CGameState()
{
	for (int a=0;a<CState::EMainStateAmount;a++)
	{
		delete iGUIStates[a];
	}
}

void CGameState::RestoreState()
{
	SELFTEST;
	iState=iSave;
	SELFTEST;
}

void CGameState::SetStateAndSave(const CState& aState)
{
	SELFTEST;
	iSave=iState;
	iState=aState;
	SELFTEST;
}

void CGameState::SetState(const CState& aState)
{
	DEBUG2("CGameState::SetState: %d %d\n",aState.iMainState,aState.iSubState);

	SELFTEST;
	// Let's cut out the illegal state changes
	ASSERT( aState.iMainState >= CState::EMainStateLogo1 );
	ASSERT( aState.iMainState < CState::EMainStateAmount );
	ASSERT( aState.iMainState == CState::EMainStateGame || aState.iSubState == 0 );

	if ( aState.iMainState == CState::EMainStateGame)
	{
		ASSERT( aState.iSubState > EGameStateNone );
		ASSERT( aState.iSubState < EGameStateAmount );
		if ( iState.iMainState != CState::EMainStateGame ) // main state changes also
		{
			ASSERT( aState.iSubState == EGameStateNextLevel );
		}
		else  // substate change only
		{
			if ( aState.iSubState != EGameStateQuit ) // let's skip checking in case of QUIT
			{	// We should have some UML diagrams of these...
				ASSERT( iState.iSubState != EGameStateNextLevel || aState.iSubState == EGameStateLevelInfo || aState.iSubState == EGameStateOnGoing );
				ASSERT( iState.iSubState != EGameStateLevelInfo || aState.iSubState == EGameStateShop || aState.iSubState == EGameStateOnGoing );
				ASSERT( iState.iSubState != EGameStateShop || aState.iSubState == EGameStateShop || aState.iSubState == EGameStateOnGoing );
				ASSERT( iState.iSubState != EGameStateOnGoing || aState.iSubState == EGameStateStatistics );
				ASSERT( iState.iSubState != EGameStateStatistics || aState.iSubState == EGameStateNextLevel || aState.iSubState == EGameStateVictory || aState.iSubState == EGameStateStatistics );				
			}
		}
	}


	//	if (iState==aState) return;
	bool iMainChange=iState.iMainState!=aState.iMainState;

	MEMCLEAN();
	if (iGUIStates[MainState()])
		iGUIStates[MainState()]->ExitSubState();

	if (iMainChange)
	{
		MEMCLEAN();
		if (iGUIStates[MainState()])
			iGUIStates[MainState()]->ExitState();
	}
	MEMCLEAN();
	iState=aState;
	iStateChanged=true;
	if (iMainChange)
	{
		MEMCLEAN();
		if (iGUIStates[MainState()])
			iGUIStates[MainState()]->EnterState();
	}
	MEMCLEAN();
	if (iGUIStates[MainState()])
		iGUIStates[MainState()]->EnterSubState();
	MEMCLEAN();

	SELFTEST;
}

bool CGameState::First()
{
	SELFTEST;
	return iStateChanged;
	SELFTEST;
}

bool CGameState::RunFrame()
{
	SELFTEST;
	bool draw;

	if (iGUIStates[MainState()]==NULL)
	{
		return false;
	}

	draw = iGUIStates[MainState()]->RunFrame();
	SELFTEST;
	return draw;
}

void CGameState::Draw(CDrawArea& aDirtyArea,CDrawArea& aDrawArea)
{
	SELFTEST;
	if (iGUIStates[MainState()]!=NULL)
	{
		iGUIStates[MainState()]->Draw(aDirtyArea,aDrawArea,iStateController->GGI());
	}

	iStateChanged = false;
	SELFTEST;
}

void CGameState::Start()
{
	SELFTEST;
	iGUIStates[MainState()]->EnterState();
	SELFTEST;
}

TEST( CGameState )
{
	ASSERTPTR(	this );
	ASSERT( iState.iMainState >= CState::EMainStateLogo1 );
	ASSERT( iState.iMainState < CState::EMainStateAmount );
	ASSERT( iState.iMainState == CState::EMainStateGame || iState.iSubState == 0 );

	if ( iState.iMainState == CState::EMainStateGame)
	{
		ASSERT( iState.iSubState > EGameStateNone );
		ASSERT( iState.iSubState < EGameStateAmount );
	}

}

