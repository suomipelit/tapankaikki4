#ifndef __CGUISTATELOGO_H_
#define __CGUISTATELOGO_H_

#include "IGUIState.h"
#include "CGameData.h"
#include "CGameState.h"

class CGraphicsBuffer;
class IGUIStateController;

class CGUIStateLogo : public IGUIState
{
public: // constructor
	CGUIStateLogo(CGameData::TPaletteNo aPal, const CGraphicsBuffer* buf, bool aStartMusic, CState aNextState,IGUIStateController* aStateController);

public: // from IGUIState

	// return true if draw is needed
	bool RunFrame();
	void Draw(CDrawArea& aDirtyArea,CDrawArea& aDrawArea,CGameGraphicsInterface* aGGI);
	void EnterState();
	void ExitState();
	void EnterSubState();
	void ExitSubState();

protected:
	int iTimerStart;
	CGameData::TPaletteNo iPal;
	const CGraphicsBuffer* iImage;
	bool iStartMusic;
	CState iNextState;
	IGUIStateController* iStateController;
};


#endif

