// CGameMenuContainer.h: interface for the CGameMenuContainer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __GAMEMENUCONTAINER_H_
#define __GAMEMENUCONTAINER_H_

#include "common/CRect.h"
#include "IGUIState.h"

class IGUIStateController;
class CGameGraphicsInterface;
class CGameMenuBase;
class CEventHandler;
class COptions;

enum TGameMenuState
{
	EGameMenuStateQuit=-1,
	EGameMenuStateAudioOptions=0,
	EGameMenuStateDeathMatchOptions,
	EGameMenuStateEpisodeSelect,
	EGameMenuStateGameOptions,
	EGameMenuStateMain,
	EGameMenuStateMultiplay,
	EGameMenuStateOptions,
	EGameMenuStatePlayerOptions1,
	EGameMenuStatePlayerOptions2,
	EGameMenuStatePlayerOptions3,
	EGameMenuStatePlayerOptions4,
	EGameMenuStateChangeKeys1,
	EGameMenuStateChangeKeys2,
	EGameMenuStateChangeKeys3,
	EGameMenuStateChangeKeys4,
	//EGameMenuStateCredits,
	EGameMenuStateNetOptions,
	EGameMenuStateAmount
	
};

class CGameMenuContainer : public IGUIState
{
public:
	CGameMenuContainer(IGUIStateController* aStateController);
	~CGameMenuContainer();
	void ChangeState(TGameMenuState aState);
	COptions* Options();
	inline IGUIStateController* StateController(){ return iStateController; }
	void SkipDrawOnce();
	void DoFullRedraw();

public: // from IGUIState
	// return true if draw is needed
	bool RunFrame();
	void Draw(CDrawArea& aDirtyArea,CDrawArea& aDrawArea,CGameGraphicsInterface* aGGI);
	void EnterState();
	void ExitState();
	void EnterSubState();
	void ExitSubState();

private:
	
	bool iSkipDraw;
	bool iFullRedraw;
	IGUIStateController* iStateController;
	TGameMenuState iState;
 	CGameMenuBase* iGameMenus[EGameMenuStateAmount];
};

#endif
