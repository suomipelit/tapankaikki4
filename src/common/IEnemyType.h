#ifndef __IENEMYTYPE_H
#define __IENEMYTYPE_H

#include "CGameTypeData.h"

class IEnemyType
{
public:
	virtual ~IEnemyType()=0;
	virtual const char* Name() const=0;
	virtual int Sprite() const=0;
	virtual float Speed() const=0;
//	enum TWeapon {};
//	virtual enum TWeapon CurrentWeapon() const=0;
	virtual TWeapon CurrentWeapon() const=0;
    virtual int Hostile() const=0;
	virtual int Energy() const=0;
	virtual int Reward() const=0;
	virtual int ExplosionDeath() const=0;
	virtual int SightDistance() const=0;
};

#endif

