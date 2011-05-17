#ifndef __CGAMESUBSTATESTATISTICS_H_
#define __CGAMESUBSTATESTATISTICS_H_

#include "IGameSubState.h"

class IGameSubStateController;
class CDynGameData;
class CEventHandler;
class IGUIStateController;
class CGameData;
class CGameDrawManager;
class CGameGraphicsInterface;

class CGameSubStateStatistics : public IGameSubState 
{
public:
	CGameSubStateStatistics(IGameSubStateController* aSubStateController);

public: // From IGUIState
	~CGameSubStateStatistics();
	// return true if draw is needed
	bool RunFrame();
	void Draw(CDrawArea& aDirtyArea,CDrawArea& aDrawArea,CGameGraphicsInterface* aGGI);
	
	void EnterState();
	void ExitState();
private:
	void GivePlayerPrize();
	int GetNextPlayer(int aIndex);
	void DrawPlayerInfo( int aPartOfScreen, int aPlayerNo, CGameGraphicsInterface* aGGI );

private:
	IGameSubStateController* iStateController;
	CDynGameData* iDynData;
	CEventHandler* iEventHandler;
	IGUIStateController* iGUIStateController;
	CGameData* iGameData;
	CGameDrawManager* iGameDrawManager;
	CGameGraphicsInterface* iGGI;
	int iTimeBonus;
};

#endif

