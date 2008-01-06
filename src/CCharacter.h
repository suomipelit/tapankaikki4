#ifndef __CCHARACTER_H
#define __CCHARACTER_H

#include "common/CCoord.h"
#include "common/CRect.h"

#include "IBulletObserver.h"

const int KBodyPartSpriteSize = 20;
const int KCharacterSpriteSize = 28;
const int KEnemySpriteAmount = 40;
const int KPlayerSpriteAmount = 80;
const int KWalkAnimationDelay=50; // walk animation delay
const int KDeadAnimationDelay=75; // dead animation delay

class CPlayer;
class CGameSystem;
class CDynGameData;
class ICharacterObserver;
class IWeaponType;
class CGameData;
class CEnemy;

class CCharacter : public IBulletObserver
{
// Methods
public:
	virtual void Animate()=0;
	virtual void Footprint( int side )=0;
            void LoadWeapons();

	virtual bool GetDamage( float damage, IBulletObserver *aObserver)=0;
	virtual void Shoot(int/*hand*/)=0;
	virtual bool CollisionCheck(const CRect<int>& aRect);
	virtual void Move(int aAngle, float aSpeed, CDynGameData* aDynData);

	virtual void SetCurrentWeapon(int aIndex);
	virtual void SetSecondaryWeapon(int aIndex);

	virtual bool CheckBurning(CDynGameData* aDynData);

	CCharacter(ICharacterObserver* aObserver,CGameData* aGameData);
	inline CCoord<float>& Location(){ return iLocation; };
	inline const CCoord<float>& Location() const { return iLocation; };
	inline int SpriteAngle(){ return (int)(iAngle/4.5f); }
	const IWeaponType*  CurrentWeapon() const;
	const IWeaponType*  SecondaryWeapon() const;

public: // from IBulletObserver
	
	void BulletHit( float aShare );
	void BulletPlayerKill( CPlayer* aPlayer );
	void BulletEnemyKill( CEnemy* aEnemy );
	
public: 
	int iTargetAngle;
    int iBurning;
	IBulletObserver *iFryingObserver;
	float iPushPower;
	int iPushAngle;
	float iAngle,iEnergy;
	int iWalk,iFire,iFire2,iAnim;

	int iLoadCounter;
	int iLoadCounter2;
	int iSecondaryCounter;
    int iShootCount;
protected:
	const IWeaponType* iCurrentWeapon;
	const IWeaponType* iSecondaryWeapon;

	CGameData* iGameData;
	ICharacterObserver* iObserver;
	CCoord<float> iLocation;
};

#endif


