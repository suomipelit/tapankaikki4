#ifndef __IGAMEMODE_H
#define __IGAMEMODE_H

class CPlayer;
class CCharacter;
class CDynGameData;

class IGameMode
{
public:
	virtual inline ~IGameMode(void){};

	virtual const char* Name() = 0;
	virtual bool ObjectiveFulfilled() = 0;
	virtual bool StartFromAnyLevel() = 0;
	virtual bool GameOver() = 0;
	virtual bool Frags() = 0;
	virtual void SetupPlayerEpisode( CPlayer* aPlayer ) = 0;
	virtual void SetupPlayerLevel( CPlayer* aPlayer ) = 0;
	virtual bool DMEpisodes() = 0;
	virtual bool CrateRespawn() = 0;
	virtual bool ActionTimeMatters() = 0;
	virtual bool DisableEnemiesPossible() = 0;
	virtual bool ShopEnabled( int aLevel ) = 0;
	virtual bool HidePlayersFromMap() = 0;
	virtual bool RandomPlacePlayers() = 0;
	virtual bool DMCrates() = 0;
	virtual void CharacterDied( CCharacter* aCharacter ) = 0;
};

#endif

