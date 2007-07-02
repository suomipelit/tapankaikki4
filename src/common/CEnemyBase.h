#ifndef __ENEMYBASE_H_
#define __ENEMYBASE_H_

#include "IEnemyType.h"
#include "CGameTypeData.h"

class CEnemyBase :
	public IEnemyType
{
public:
	CEnemyBase();
	const char* Name() const;
	int Sprite() const;
	float Speed() const;
	enum TWeapon CurrentWeapon() const;
    int Hostile() const;
	int Energy() const;
	int Reward() const;
	int ExplosionDeath() const;
	int SightDistance() const;
protected:
	char* iName;
	int iSprite;
	float iSpeed;
//	enum
	 TWeapon iCurrentWeapon;
    int iHostile;
	int iEnergy;
	int iReward;
	int iExplosionDeath;
	int iSightDistance;
};

#endif

