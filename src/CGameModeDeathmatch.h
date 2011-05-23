#ifndef __CGAMEMODEDEATHMATCH_H_
#define __CGAMEMODEDEATHMATCH_H_

#include "IGameMode.h"

class IGUIStateController;

class CGameModeDeathmatch : public IGameMode
{
public:
	CGameModeDeathmatch( IGUIStateController* aGUIStateController );
	~CGameModeDeathmatch();

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

