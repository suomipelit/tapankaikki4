#ifndef __CGAMESTATE_H
#define __CGAMESTATE_H

#define MAINSTATEMASK 0xff00
#define MAINSTATESHIFT 8
#define SUBSTATEMASK  0xff

#include "common/CRect.h"
#include "common/debug.h"
#include "CState.h"

class CSoundPlayer;
class CGameGraphicsInterface;
class CDrawArea;

enum TGameState
{
	EGameStateNone=-1,
	EGameStateOnGoing=0,
	EGameStateShop,
	EGameStateVictory,
	EGameStateStatistics,
	EGameStateNextLevel,
	EGameStateLevelInfo,
	EGameStateQuit,
	EGameStateAmount
};

class IGUIState;
class IGUIStateController;

class CGameState
{
public:
	CGameState(IGUIStateController* aStateController);
	~CGameState();

	void SetStateAndSave(const CState& aState);
	void RestoreState();
	void SetState(const CState& aState);

	inline CState& State(){ return iState; } const
	inline int SubState(){ return iState.iSubState; } const
	inline enum CState::TMainState MainState(){ return iState.iMainState; } const
	IGUIState* GUIState();

	void Draw(CDrawArea& aDirtyArea,CDrawArea& aDrawArea);
	bool RunFrame();
	bool First();
	void Start();

private:
	CState iSave;
	CState iState;
	bool iStateChanged;
	bool iEffectOngoing;

	IGUIState* iGUIStates[CState::EMainStateAmount];
	IGUIStateController* iStateController;

	__DECLARE_TEST;
};

#endif


