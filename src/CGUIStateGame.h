#ifndef __CGUISTATEGAME_H
#define __CGUISTATEGAME_H

#include "IGUIState.h"

class IGUIStateController;

class CGUIStateGame : public IGUIState
{
public: 
	CGUIStateGame(IGUIStateController* aStateController);

public: // From iGUIState
	// return true if draw is needed
	bool RunFrame();
	void Draw(CDrawArea& aDirtyArea,CDrawArea& aDrawArea,CGameGraphicsInterface* aGGI);
	
	void EnterState();
	void ExitState();
	void EnterSubState();
	void ExitSubState();
private:

	IGUIStateController* iStateController;
};

#endif

