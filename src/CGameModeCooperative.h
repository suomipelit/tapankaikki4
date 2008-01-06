#ifndef __CGAMEMODECOOPERATIVE_H
#define __CGAMEMODECOOPERATIVE_H

#include "IGameMode.h"

class IGUIStateController;

class CGameModeCooperative : public IGameMode
{
public:
	CGameModeCooperative( IGUIStateController* aGUIStateController );
	~CGameModeCooperative();

public:
	const char* Name();
	bool ObjectiveFulfilled();
	bool StartFromAnyLevel();
	bool GameOver();
	bool Frags();
	void SetupPlayerEpisode( CPlayer* aPlayer );
	void SetupPlayerLevel( CPlayer* aPlayer );
	bool DMEpisodes();
	bool CrateRespawn();
	bool ActionTimeMatters();
	bool DisableEnemiesPossible();
	bool ShopEnabled( int aLevel );
	bool HidePlayersFromMap();
	bool RandomPlacePlayers();
	bool DMCrates();
	void CharacterDied( CCharacter* aCharacter );
protected:
	
	 IGUIStateController* iGUIStateController;
};

#endif

