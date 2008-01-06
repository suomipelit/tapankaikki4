#ifndef __IGAMESUBSTATECONTROLLER_H
#define __IGAMESUBSTATECONTROLLER_H

class CEventHandler;
class IGUIStateController;
class CDynGameData;
class CGameDrawManager;
class CDynShopData;
class IGameMode;

class IGameSubStateController
{
public:
	virtual void BackToMenu()=0;
	virtual CDynGameData* DynData()=0;
	virtual CGameDrawManager* GameDrawManager()=0;
	virtual IGUIStateController* GUIStateController()=0;
	virtual CDynShopData* ShopData()=0;
	virtual CEventHandler* EventHandler()=0;
	virtual void Init(IGameMode* aFightMode, TNetworkMode aNetMode, int aLocalPlayers, int aEpisode, int aLevel) = 0;
	virtual bool RunFrame()=0;
	virtual void DrawFrame(CDrawArea& aDirtyArea,CDrawArea& aDrawArea,CGameGraphicsInterface* aGGI)=0;
	virtual void EnterState()=0;
	virtual void ExitState()=0;
	virtual void EnterSubState()=0;
	virtual void ExitSubState()=0;
};

#endif

