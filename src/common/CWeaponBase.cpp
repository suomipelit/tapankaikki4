#include <stdlib.h>
#include "CWeaponBase.h"
#include "error.h"
#include "CGameTypeData.h"

CWeaponBase::CWeaponBase()
{
	iName=NULL;
	iSprite=0;
	iSingleHand=false;
	iCost=0;
	iLoadingTime=0;
	iSmoke=0;
	iBulletAmount=0;
	iConsumesBullets=true;
	iAngleAdd=0;
	iGun=false;
	iBulletType=EBulletFist;
	iSound=ESoundEffectNoSound;
	iPriority=0;
}

const char* CWeaponBase::Name() const
{
	ASSERT(iName);
	return iName;
}

int CWeaponBase::Sprite() const
{
	return iSprite;
}

bool CWeaponBase::SingleHand() const
{
	return iSingleHand;
}

int CWeaponBase::Cost() const
{
	return iCost;
}

int CWeaponBase::LoadingTime() const
{
	return iLoadingTime;
}

int CWeaponBase::Smoke() const
{
	return iSmoke;
}

int CWeaponBase::BulletAmount() const
{
	return iBulletAmount;
}

int CWeaponBase::AngleAdd() const
{
	return iAngleAdd;
}

bool CWeaponBase::Gun() const
{
	return iGun;
}

enum TBullet CWeaponBase::BulletType() const
{
	return iBulletType;
}

TIngameSoundEffect CWeaponBase::Sound() const
{
	return iSound;
}

int CWeaponBase::Priority() const
{
	return iPriority;
}

bool CWeaponBase::ConsumesBullets() const
{
	return iConsumesBullets;
}
