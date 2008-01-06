#ifndef __CGAMESUBSTATELEVELINFO_H
#define __CGAMESUBSTATELEVELINFO_H

#include "IGameSubState.h"

class IGameSubStateController;
class CEventHandler;
class CDynGameData;
class CGameState;

class CGameSubStateLevelInfo : public IGameSubState 
{
public:
	CGameSubStateLevelInfo(IGameSubStateController* aSubStateController);

public: // From IGUIState
	~CGameSubStateLevelInfo();
	// return true if draw is needed
	bool RunFrame();
	void Draw(CDrawArea& aDirtyArea,CDrawArea& aDrawArea,CGameGraphicsInterface* aGGI);
	
	void EnterState();
	void ExitState();
private:
	IGameSubStateController* iStateController;
	CEventHandler* iEventHandler;
	CDynGameData* iDynData;
	CGameState* iGameState;
};

#endif

