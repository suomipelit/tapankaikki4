#include <stdlib.h>
#include "CLevel.h"
#include "CEnemyBase.h"

CEnemyBase::CEnemyBase()
{
	iName=NULL;
	iSprite=0;
	iSpeed=0.0f;
	iCurrentWeapon=EWeaponFist;
    iHostile=0;
	iEnergy=0;
	iReward=0;
	iExplosionDeath=0;
	iSightDistance=10*KBlockSpriteSize;
}

const char* CEnemyBase::Name() const
{
	return iName;
}

int CEnemyBase::Sprite() const
{
	return iSprite;
}

float CEnemyBase::Speed() const
{
	return iSpeed;
}

enum TWeapon CEnemyBase::CurrentWeapon() const
{
	return iCurrentWeapon;
}

int CEnemyBase::Hostile() const
{
	return iHostile;
}

int CEnemyBase::Energy() const
{
	return iEnergy;
}

int CEnemyBase::Reward() const
{
	return iReward;
}

int CEnemyBase::ExplosionDeath() const
{
	return iExplosionDeath;
}

int CEnemyBase::SightDistance() const
{
	return iSightDistance;
}
