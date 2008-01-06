#ifndef __CGAMESYSTEM_H
#define __CGAMESYSTEM_H

#include "CGameState.h"
#include "ICharacterObserver.h"
#include "IGameSubStateController.h"

class CPlayer;
class CGameDrawer;
class CGameDrawManager;
class CNetworkAddress;
class CGameGraphicsInterface;
class CPalette;
class CDynGameData;
class COptions;
class CEventHandler;
class CSoundPlayer;
class CMessageBoard;
class CDynShopData;
class CGameApp;
class CGameData;
class IGameSubState;

class CGameSystem : public ICharacterObserver, public IGameSubStateController
{
public:

	CGameSystem(IGUIStateController* aGUIStateController);
	~CGameSystem();

public: // from IGameSubStateController
	// Method to return to main menu
	void BackToMenu();
	CDynGameData* DynData();
	CGameDrawManager* GameDrawManager();
	IGUIStateController* GUIStateController();
	CDynShopData* ShopData();
	CEventHandler* EventHandler();
	void Init(IGameMode* aFightMode, TNetworkMode aNetMode, int aLocalPlayers, int aEpisode, int aLevel);
	bool RunFrame();
	void DrawFrame(CDrawArea& aDirtyArea,CDrawArea& aDrawArea,CGameGraphicsInterface* aGGI);
	void EnterState();
	void ExitState();
	void EnterSubState();
	void ExitSubState();

public: // from ICharacterObserver
	void CharacterDied(CCharacter* aCharacter);

protected: // new methods

	// Reset object
	void Reset();
	
protected: // DATA
	
	bool iInitialized;
	IGameSubState* iSubStates[EGameStateAmount];

	CGameData* iGameData;
	CDynGameData* iDynData;
	CDynShopData* iShopData;
	COptions* iOptions;
	CEventHandler* iEventHandler;
	CSoundPlayer* iSP;
	CMessageBoard* iMBoard;
	IGUIStateController* iGUIStateController;

	CGameDrawManager* iGameDrawManager;
	CGameGraphicsInterface* iGGI;

};

#endif


