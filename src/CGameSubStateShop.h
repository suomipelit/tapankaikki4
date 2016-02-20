#ifndef __CGAMESUBSTATESHOP_H
#define __CGAMESUBSTATESHOP_H

#include "IGameSubState.h"
#include "common/CDrawArea.h"

class IGameSubStateController;
class CEventHandler;
class CDynShopData;
class CSoundPlayer;
class CGraphicsBuffer;
class CGameData;

class CGameSubStateShop : public IGameSubState 
{
public:
	CGameSubStateShop(IGameSubStateController* aSubStateController);

public: // From IGUIState
	~CGameSubStateShop();
	// return true if draw is needed
	bool RunFrame();
	void Draw(CDrawArea& aDirtyArea,CDrawArea& aDrawArea,CGameGraphicsInterface* aGGI);
	
	void EnterState();
	void ExitState();
private:
	bool RunKeys();
	void DrawWareInfo(int aX, int aY,CDrawArea& aDrawArea,CGameGraphicsInterface* aGGI);
	void DrawStaticToBackground();

private:
	int iOldTimer;
	IGameSubStateController* iStateController;
	CEventHandler* iEventHandler;
	CDynShopData* iShopData;
	CSoundPlayer* iSP;
	CGameData* iGameData;
	CDrawArea iCleanArea;
	CGraphicsBuffer* iBackgroundPicture;
};

#endif

