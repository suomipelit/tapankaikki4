#include <stdlib.h>
#include "CBulletBase.h"
#include "defines.h"
#include "CColorTable.h"

CBulletBase::CBulletBase()
{
	iName=NULL;
	iType=EBulletTypeNormal;
	iStop=0;
	iCollisionTrigger=1;
	iBounceType = KBounceTypeNone;
	iBounceSound = ESoundEffectNoSound;
	iSpeed=0;
    iTimeBeforeSpin=0;
    iSlow=0.0f;
	iPower=0.0f;
	iTime=0;
	iExplosion=0;// power of explosion
	iPushPower=0.0f;
	iCost=0;
	iAmountInCrate=0;
	iMultiplier=1;
	iSleepTime=0;
	iMaximumForPlayer=0;
	iRocketLength=0; // lenght of rocket
	iRocketColor=32; // default color of rocket
	iSprite=EGameSpriteNone; // Number of Sprite
	iXS=0;
	iYS=0;
    iSpriteAngles=0;
	iLight=ELightTableNoLight;
	iLightSize=0;
	iLightLuminance=0;
	iLightAdd=0.0f;
	iEffect=EEffectNoEffect;
	iEffect2=EEffectNoEffect;
}

const char* CBulletBase::Name() const
{
	return iName;
}

CBulletBase::TBulletType CBulletBase::Type() const
{
	return iType;
}

int CBulletBase::Stop() const
{
	return iStop;
}

bool CBulletBase::CollisionTrigger() const
{
	return iCollisionTrigger;
}


IBulletType::TBounceType CBulletBase::BounceType() const
{
	return iBounceType;
}

TIngameSoundEffect CBulletBase::BounceSound() const
{
	return iBounceSound;
}

int CBulletBase::Speed() const
{
	return iSpeed;
}

int CBulletBase::TimeBeforeSpin() const
{
	return iTimeBeforeSpin;
}

float CBulletBase::Slow() const
{
	return iSlow;
}

float CBulletBase::Power() const
{
	return iPower;
}

int CBulletBase::Time() const
{
	return iTime;
}

int CBulletBase::Explosion() const
{
	return iExplosion;
}

float CBulletBase::PushPower() const
{
	return iPushPower;
}

int CBulletBase::Cost() const
{
	return iCost;
}

int CBulletBase::AmountInCrate() const
{
	return iAmountInCrate;
}

int CBulletBase::Multiplier() const
{
	return iMultiplier;
}

int CBulletBase::SleepTime() const
{
	return iSleepTime;
}

int CBulletBase::MaximumForPlayer() const
{
	return iMaximumForPlayer;
}

int CBulletBase::RocketLength() const
{
	return iRocketLength;
}

int CBulletBase::RocketColor() const
{
	return iRocketColor;
}

TGameSprite CBulletBase::Sprite() const
{
	return iSprite;
}

int CBulletBase::XS() const
{
	return iXS;
}

int CBulletBase::YS() const
{
	return iYS;
}

int CBulletBase::SpriteAngles() const
{
	return iSpriteAngles;
}

TLightTable CBulletBase::Light() const
{
	return iLight;
}

int CBulletBase::LightSize() const
{
	return iLightSize;
}

int CBulletBase::LightLuminance() const
{
	return iLightLuminance;
}

float CBulletBase::LightAdd() const
{
	return iLightAdd;
}

int CBulletBase::ApproxDistance() const
{
    int n, a1;
    float an;//for aritmethic serie

	a1=iSpeed;
	n=iTime;
	an=a1+(float)(n-1)*iSlow;
	return (int)(n*((a1+an)/2)); 
}

TEffect CBulletBase::Effect() const
{
	return iEffect;
}

int CBulletBase::EffectAngle( int aOrigAngle ) const
{
	return aOrigAngle;
}
float CBulletBase::EffectSpeed() const
{
	return 0;
}

bool CBulletBase::DoEffect( int aCounter ) const
{
	return true;
}

TEffect CBulletBase::Effect2() const
{
	return iEffect2;
}

int CBulletBase::Effect2Angle( int aOrigAngle ) const
{
	return aOrigAngle;
}

float CBulletBase::Effect2Speed() const
{
	return 0;
}

bool CBulletBase::DoEffect2( int aCounter ) const
{
	return true;
}

