#ifndef __CGAMESUBSTATEQUIT_H
#define __CGAMESUBSTATEQUIT_H

#include "common/CRect.h"
#include "IGameSubState.h"

class IGameSubStateController;
class CEventHandler;
class CDynGameData;
class CGameGraphicsInterface;
class CGameState;

class CGameSubStateQuit : public IGameSubState 
{
public:
	CGameSubStateQuit(IGameSubStateController* aSubStateController);

public: // From IGUIState
	~CGameSubStateQuit();
	// return true if draw is needed
	bool RunFrame();
	void Draw(CDrawArea& aDirtyArea,CDrawArea& aDrawArea,CGameGraphicsInterface* aGGI);
	
	void EnterState();
	void ExitState();
private:
	IGameSubStateController* iGameSubStateController;
	CEventHandler* iEH;
	CDynGameData* iDynData;
	CGameGraphicsInterface* iGGI;
	CGameState* iGameState;
	CRect<int> iRect;

};

#endif

