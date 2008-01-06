#ifndef __IWEAPONTYPE_H
#define __IWEAPONTYPE_H

#include "defines.h"
#include "IBulletType.h"

class IWeaponType
{
public:
	virtual ~IWeaponType()=0;
	virtual const char* Name() const=0;
	virtual int Sprite() const=0;
	virtual bool SingleHand() const=0;
	virtual int Cost() const=0;
	virtual int LoadingTime() const=0;
	virtual int Smoke() const=0;
	virtual int BulletAmount() const=0;
	virtual int AngleAdd() const=0;
	virtual bool Gun() const=0;
	virtual enum TBullet BulletType() const=0;
	virtual TIngameSoundEffect Sound() const=0;
	virtual int Priority() const=0;
	virtual bool ConsumesBullets() const=0;
};

#endif

