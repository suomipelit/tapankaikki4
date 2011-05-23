// CGameMenuContainer.cpp: implementation of the CGameMenuContainer class.
//
//////////////////////////////////////////////////////////////////////

#include "common/CEventHandler.h"


#include "CGameMenuContainer.h"
#include "menu.h"
#include "COptions.h"
#include "CGameState.h"
#include "CGameGraphicsInterface.h"
#include "IGUIStateController.h"
#include "CSoundPlayer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameMenuContainer::CGameMenuContainer(IGUIStateController* aStateController): iStateController(aStateController)
{
	int a;

	/* Test that we have something else than null as parameter*/
	ASSERT(iStateController);

	/* Test that COptions is already available */
	ASSERT(iStateController->Options());

	iGameMenus[EGameMenuStateAudioOptions		]=new CGameMenuAudioOptions(this);
	iGameMenus[EGameMenuStateDeathMatchOptions	]=new CGameMenuDeathMatchOptions(this);
	iGameMenus[EGameMenuStateEpisodeSelect		]=new CGameMenuEpisodeSelect(this);
	iGameMenus[EGameMenuStateGameOptions		]=new CGameMenuGameOptions(this);
	iGameMenus[EGameMenuStateMain				]=new CGameMenuMain(this);
	iGameMenus[EGameMenuStateMultiplay			]=new CGameMenuMultiplay(this);
	iGameMenus[EGameMenuStateOptions			]=new CGameMenuOptions(this);
	iGameMenus[EGameMenuStatePlayerOptions1		]=new CGameMenuPlayerOptions(1,this);
	iGameMenus[EGameMenuStatePlayerOptions2		]=new CGameMenuPlayerOptions(2,this);
	iGameMenus[EGameMenuStatePlayerOptions3		]=new CGameMenuPlayerOptions(3,this);
	iGameMenus[EGameMenuStatePlayerOptions4		]=new CGameMenuPlayerOptions(4,this);
	iGameMenus[EGameMenuStateChangeKeys1		]=new CGameMenuChangeKeys(1,this);
	iGameMenus[EGameMenuStateChangeKeys2		]=new CGameMenuChangeKeys(2,this);
	iGameMenus[EGameMenuStateChangeKeys3		]=new CGameMenuChangeKeys(3,this);
	iGameMenus[EGameMenuStateChangeKeys4		]=new CGameMenuChangeKeys(4,this);
	iGameMenus[EGameMenuStateNetOptions			]=new CGameMenuNetOptions(this);

	for (a=0;a<EGameMenuStateAmount;a++)
	{
		iGameMenus[a]->SetBackground(iStateController->GameData()->MenuSprite(EMenuSpriteMenuBG));
	}

	ChangeState(EGameMenuStateMain);
}

CGameMenuContainer::~CGameMenuContainer()
{	int a;

	for (a=0;a<EGameMenuStateAmount;a++)
		delete(iGameMenus[a]);

}

void CGameMenuContainer::ChangeState(TGameMenuState aState)
{
	if (aState==EGameMenuStateQuit)
	{
		iStateController->State()->SetState(CState(CState::EMainStateQuit,0));
		return;
	}

	iFullRedraw = true;
	iState=aState;

	// Always save options
	iStateController->Options()->Save();
	iGameMenus[iState]->ActivateMenu();
}

COptions* CGameMenuContainer::Options()
{ 
	return iStateController->Options();
};

bool CGameMenuContainer::RunFrame()
{
	bool processedKey;
	iSkipDraw = false;
	if (iStateController->State()->First())
		iStateController->StartFadeIn(CGameData::EPaletteMenu);
	processedKey = iGameMenus[iState]->RunMenu(iStateController->EventHandler());
	if (processedKey)
		iStateController->SoundPlayer()->PlaySample( iStateController->GameData()->Click(),KFullVolume, PAN_MIDDLE);
	return true;
}

void CGameMenuContainer::Draw(CDrawArea& aDirtyArea,CDrawArea& aDrawArea,CGameGraphicsInterface* aGGI)
{
	if (iSkipDraw) return;

	if (iFullRedraw)
	{
		CDrawArea ar;
		ar.Combine(aGGI->DrawBuffer()->Rect());
		iGameMenus[iState]->Draw(ar,aDrawArea,aGGI);
	}
	else
		iGameMenus[iState]->Draw(aDirtyArea,aDrawArea,aGGI);
		
	iFullRedraw = false;
}

void CGameMenuContainer::EnterState()
{
	iStateController->EventHandler()->ResetStack();
	iGameMenus[iState]->ActivateMenu();
}

void CGameMenuContainer::ExitState(){}
void CGameMenuContainer::EnterSubState(){};
void CGameMenuContainer::ExitSubState(){};

void CGameMenuContainer::SkipDrawOnce()
{
	iSkipDraw = true;
}

void CGameMenuContainer::DoFullRedraw()
{
	iFullRedraw = true;
}
