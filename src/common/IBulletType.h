#ifndef __IBULLETTYPE_H_
#define __IBULLETTYPE_H_

class IBulletType
{
public:

	enum TBounceType
	{
		KBounceTypeNone = 0,
		KBounceTypeNormal,
		KBounceTypeFollowWall,
	};
	enum TBulletType
	{
		EBulletTypeNormal=0,
		EBulletTypeExplosive,
		EBulletTypeIncendiary,
		EBulletTypeFist
	};
public:
	virtual ~IBulletType();

	virtual const char* Name() const=0;
	virtual TBulletType Type() const=0;
	virtual int Stop() const=0;
	virtual bool CollisionTrigger() const=0;
	virtual TBounceType BounceType() const=0;
	virtual TIngameSoundEffect BounceSound() const=0;
	virtual int Speed() const=0;
        virtual int TimeBeforeSpin() const=0;
	// how much speed of bullet slows down each frame 
	virtual float Slow() const=0;
	virtual float Power() const=0;
	virtual int Time() const=0;
	// power of explosion
	virtual int Explosion() const=0;
	virtual float PushPower() const=0;
	virtual int Cost() const=0;
	virtual int AmountInCrate() const=0;
	virtual int Multiplier() const=0;
	virtual int SleepTime() const=0;
	virtual int MaximumForPlayer() const=0;
	virtual int RocketLength() const=0; // lenght of rocket
	virtual int RocketColor() const=0; // color of rocket
        virtual TGameSprite Sprite() const=0; // Number of Sprite
	virtual int XS() const=0;
	virtual int YS() const=0;
        virtual int SpriteAngles() const=0;
        virtual TLightTable Light() const=0;
	virtual int LightSize() const=0;
	virtual int LightLuminance() const=0;
	virtual float LightAdd() const=0;
	virtual int ApproxDistance() const=0;
	// Effects of bullet
	virtual TEffect Effect() const=0;
	virtual int EffectAngle( int aOrigAngle ) const=0;
	virtual float EffectSpeed() const=0;
	virtual bool DoEffect( int aCounter ) const=0;
	virtual TEffect Effect2() const=0;
	virtual int Effect2Angle( int aOrigAngle ) const=0;
	virtual float Effect2Speed() const=0;
	virtual bool DoEffect2( int aCounter ) const=0;
};

#endif

