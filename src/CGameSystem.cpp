#include <vector>
#include <string.h>
#include <stdlib.h>

#include "CGameSystem.h"

#include "CGameGraphicsInterface.h"
#include "CDynGameData.h"
#include "CDynShopData.h"
#include "CGameDrawManager.h"
#include "IGUIStateController.h"
#include "IGameMode.h"

// states
#include "CGameSubStateOngoing.h"
#include "CGameSubStateShop.h"
#include "CGameSubStateNextLevel.h"
#include "CGameSubStateLevelInfo.h"
#include "CGameSubStateQuit.h"
#include "CGameSubStateVictory.h"
#include "CGameSubStateStatistics.h"

CGameSystem::CGameSystem(IGUIStateController* aGUIStateController) : 
	iGUIStateController(aGUIStateController), iInitialized (false)
{
	// Check parameters

	ASSERT(iGUIStateController);

	iGGI = iGUIStateController->GGI();
	iGameData = iGUIStateController->GameData();
	iOptions = iGUIStateController->Options();
	iEventHandler = iGUIStateController->EventHandler();
	iSP = iGUIStateController->SoundPlayer();
	iMBoard = iGUIStateController->MessageBoard();

	ASSERT(iGGI);
	ASSERT(iGameData);
	ASSERT(iOptions);
	ASSERT(iEventHandler);
	ASSERT(iSP);
	ASSERT(iMBoard);

	iDynData=NULL;
	iGameDrawManager=NULL;
	iShopData=NULL;

	for (int a=0;a<EGameStateAmount;a++)
		iSubStates[a]=NULL;
}

CGameSystem::~CGameSystem()
{
	Reset();
}

void CGameSystem::Reset()
{
	iInitialized = false;
	delete iShopData;
	delete iGameDrawManager;
	delete iDynData;
	iShopData=NULL;
	iGameDrawManager=NULL;
	iDynData=NULL;

	for (int a=0;a<EGameStateAmount;a++)
	{
		delete iSubStates[a];
		iSubStates[a] = NULL;
	}
}

void CGameSystem::Init(IGameMode* aFightMode, TNetworkMode aNetMode, int aLocalPlayers, int aEpisode, int aLevel) 
{
	int a;

	ASSERT(aLocalPlayers<=KMaxLocalPlayers);
	ASSERT(aLocalPlayers>0);

	// Dynamic initialization :)

	// Safe allocation
	// Make sure that we don't have objects initialized already, we shouldn't have
	ASSERT(!iDynData);
	iDynData = new CDynGameData(iGGI, iGameData,iSP, this, iOptions, aLocalPlayers);
	ASSERT(iDynData);

	ASSERT(!iGameDrawManager);
	iGameDrawManager = new CGameDrawManager(iGGI,iGameData, iOptions,iDynData,iMBoard);
	ASSERT(iGameDrawManager);

	ASSERT(!iShopData);
	iShopData = new CDynShopData;
	ASSERT(iShopData);

	// ASSERT of states
	for (a=0;a<EGameStateAmount;a++)	
		ASSERT(!iSubStates[a]);

	iSubStates[EGameStateOnGoing]=new CGameSubStateOngoing(this);
	iSubStates[EGameStateShop]=new CGameSubStateShop(this);
	iSubStates[EGameStateVictory]=new CGameSubStateVictory(this);
	iSubStates[EGameStateStatistics]=new CGameSubStateStatistics(this);
	iSubStates[EGameStateNextLevel]=new CGameSubStateNextLevel(this);
	iSubStates[EGameStateLevelInfo]=new CGameSubStateLevelInfo(this);
	iSubStates[EGameStateQuit]=new CGameSubStateQuit(this);

	for (a=0;a<EGameStateAmount;a++)	
		ASSERT(iSubStates[a]);

	iDynData->Setup( aFightMode, aNetMode, aEpisode );
	iDynData->SetLocalPlayers(aLocalPlayers);

	// Lets start player initialization by reseting all players
	for(a=0;a<iDynData->Players().size();a++)
	{
		iDynData->Players()[a]->TotalReset(iDynData->PlayerDrawRect(0)); // at this point rect doesn't matter
	}

	for (a=0;a<aLocalPlayers;a++)
	{
		iDynData->MapLocalPlayer(a,a);
	}

	// Let's assign keys for every player
   	for (int a=0;a<iDynData->LocalPlayers().size();a++)
		iDynData->LocalPlayers()[a]->iKeys=iOptions->Data().iKeys[a];

	iDynData->UpdateMouseOwner();

	iDynData->SetupPlayers();

	/* Setup start data*/
	if (iDynData->NetworkMode()!=ENetworkModeClient)
	{
		if (!iDynData->SetLevels( iOptions->EpisodeList()->Episode( iDynData->Episode(), iDynData->GameMode()->DMEpisodes() )->Amount(), aLevel ))
		{
			iMBoard->AddMessage("Error: Cannot start episode from the specified level.");
			iMBoard->AddMessage("Please contact Errorfree for more information!");
			Reset();
			return;
		}
		iInitialized = true;
	}

	ASSERT( iInitialized );	

	/* This should be only place in CGameSystem that changes states besides RunFrame()*/
	iGUIStateController->StartFadeOut(CState(CState::EMainStateGame,EGameStateNextLevel));
}


bool CGameSystem::RunFrame()
{
	if (!iInitialized) return false;

	int state = iGUIStateController->State()->SubState();
	ASSERT(state>=0);
	ASSERT(state<EGameStateAmount);
	ASSERT(iSubStates[state]);

	return iSubStates[state]->RunFrame();
}

void CGameSystem::DrawFrame(CDrawArea& aDirtyArea,CDrawArea& aDrawArea,CGameGraphicsInterface* aGGI)
{
	if (!iInitialized) return;
	int state = iGUIStateController->State()->SubState();

	ASSERT(state>=0);
	ASSERT(state<EGameStateAmount);
	ASSERT(iSubStates[state]);

	iSubStates[state]->Draw(aDirtyArea,aDrawArea,aGGI);
}

void CGameSystem::BackToMenu()
{
	iGUIStateController->StartFadeOut(CState(CState::EMainStateMenu,0));
}

// TODO: This method should goto CampaignManager class
void CGameSystem::CharacterDied(CCharacter* aCharacter)
{
	iDynData->GameMode()->CharacterDied( aCharacter );
}

IGUIStateController* CGameSystem::GUIStateController()
{
	return iGUIStateController;
}

CDynGameData* CGameSystem::DynData()
{
	return iDynData;
}

CGameDrawManager* CGameSystem::GameDrawManager()
{
	return iGameDrawManager;
}

void CGameSystem::EnterState()
{
	ASSERT(iInitialized);
}

void CGameSystem::ExitState()
{
	Reset();
}

void CGameSystem::EnterSubState()
{
	int state = iGUIStateController->State()->SubState();
	ASSERT(state>=0);
	ASSERT(state<EGameStateAmount);
	ASSERT(iSubStates[state]);

	iSubStates[state]->EnterState();
}

void CGameSystem::ExitSubState()
{
	int state = iGUIStateController->State()->SubState();
	ASSERT(state>=0);
	ASSERT(state<EGameStateAmount);
	ASSERT(iSubStates[state]);

	iSubStates[state]->ExitState();
}

CDynShopData* CGameSystem::ShopData()
{
	return iShopData;
}

CEventHandler* CGameSystem::EventHandler()
{
	return iEventHandler;
}

