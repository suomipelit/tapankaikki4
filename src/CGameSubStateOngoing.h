#ifndef __CGAMESUBSTATEONGOING_H_
#define __CGAMESUBSTATEONGOING_H_

#include "IGameSubState.h"
#include "IPositionalSoundObserver.h"
#include "common/CCoord.h"

class IGameSubStateController;
class CDynGameData;
class CEventHandler;

class CGameSubStateOngoing : public IGameSubState, public IPositionalSoundObserver
{
public:
	CGameSubStateOngoing(IGameSubStateController* aSubStateController);

public: // From IGUIState
	~CGameSubStateOngoing();
	// return true if draw is needed
	bool RunFrame();
	void Draw(CDrawArea& aDirtyArea,CDrawArea& aDrawArea,CGameGraphicsInterface* aGGI);
	
	void EnterState();
	void ExitState();
public: // From IPosiotionalSoundObserver
	void SoundHeard(enum TIngameSoundEffect aEffect,float aVolume,CCoord<int> aPos);

private:
	void CheckUserInput();
	void WriteNetworkMessage( CEventHandler* aEventHandler );

private:

	bool iFadeIn;
	IGameSubStateController* iStateController;
	CDynGameData* iDynData;
	CEventHandler* iEH;
	unsigned int iClockval;
};

#endif
