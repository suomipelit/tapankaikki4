#ifndef __CBULLETBASE_H
#define __CBULLETBASE_H

#include "CColorTable.h"
#include "IBulletType.h"

class CBulletBase : public IBulletType
{
public:
	CBulletBase();
	const char* Name() const;
	TBulletType Type() const;
	int Stop() const;
	bool CollisionTrigger() const;
	TBounceType BounceType() const;
	TIngameSoundEffect BounceSound() const;
	int Speed() const;
    int TimeBeforeSpin() const;
    float Slow() const;
	float Power() const;
	int Time() const;
	int Explosion() const;// power of explosion
	float PushPower() const;
	int Cost() const;
	int AmountInCrate() const;
	int Multiplier() const;
	int SleepTime() const;
	int MaximumForPlayer() const;
	int RocketLength() const; // lenght of rocket
	int RocketColor() const; // color of rocket
    TGameSprite Sprite() const; // Number of Sprite
	int XS() const;
	int YS() const;
    int SpriteAngles() const;
    TLightTable Light() const;
	int LightSize() const;
	int LightLuminance() const;
	float LightAdd() const;
	int ApproxDistance() const;

	// Effects of bullet
	TEffect Effect() const;
	int EffectAngle( int aOrigAngle ) const;
	float EffectSpeed() const;
	bool DoEffect( int aCounter ) const;

	TEffect Effect2() const;
	int Effect2Angle( int aOrigAngle ) const;
	float Effect2Speed() const;
	bool DoEffect2( int aCounter ) const;

protected:
	char* iName;
	TIngameSoundEffect iBounceSound;
	TBulletType iType;
	int iStop;
	bool iCollisionTrigger;
	TBounceType iBounceType;
	int iSpeed;
    int iTimeBeforeSpin;
    float iSlow;
	float iPower;
	int iTime;
	int iExplosion;// power of explosion
	float iPushPower;
	int iCost;
	int iAmountInCrate;
	int iMultiplier;
	int iSleepTime;
	int iMaximumForPlayer;
	int iRocketLength; // lenght of rocket
	int iRocketColor; // color of rocket
	TGameSprite iSprite; // Number of Sprite
	int iXS;
	int iYS;
    int iSpriteAngles;
    TLightTable iLight;
	int iLightSize;
	int iLightLuminance;
	float iLightAdd;
	TEffect iEffect;
	TEffect iEffect2;
};

#endif

