#ifndef __CGUISTATEQUIT_H
#define __CGUISTATEQUIT_H

#include "IGUIState.h"
class IGUIStateController;

class CGUIStateQuit : public IGUIState
{
public: // constructor
	CGUIStateQuit(IGUIStateController* aStateController);

public: // from IGUIState

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

