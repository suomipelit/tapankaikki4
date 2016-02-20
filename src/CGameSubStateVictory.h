#ifndef __CGAMESUBSTATEVICTORY_H
#define __CGAMESUBSTATEVICTORY_H

#include "IGameSubState.h"

class IGameSubStateController;
class CEventHandler;

class CGameSubStateVictory : public IGameSubState 
{
public:
	CGameSubStateVictory(IGameSubStateController* aSubStateController);

public: // From IGUIState
	~CGameSubStateVictory();
	// return true if draw is needed
	bool RunFrame();
	void Draw(CDrawArea& aDirtyArea,CDrawArea& aDrawArea,CGameGraphicsInterface* aGGI);
	
	void EnterState();
	void ExitState();
private:
	CEventHandler* iEventHandler;
	IGameSubStateController* iStateController;
};

#endif

