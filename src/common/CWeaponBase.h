#ifndef __CWEAPONBASE_H
#define __CWEAPONBASE_H

#include "IWeaponType.h"
#include "defines.h"

class CWeaponBase : public IWeaponType
{
public:
	CWeaponBase();
	const char* Name() const;
	int Sprite() const;
	bool SingleHand() const;
	int Cost() const;
	int LoadingTime() const;
	int Smoke() const;
	int BulletAmount() const;
	int AngleAdd() const;
	bool Gun() const;
	enum TBullet BulletType() const;
	TIngameSoundEffect Sound() const;
	int Priority() const;
	bool ConsumesBullets() const;
protected:
	char* iName;
	int iSprite;
	bool iSingleHand;
	int iCost;
	int iLoadingTime;
	int iSmoke;
	int iBulletAmount;
	int iAngleAdd;
	bool iGun;
	bool iConsumesBullets;
	enum TBullet iBulletType;
	TIngameSoundEffect iSound;
	int iPriority;
};	

#endif
