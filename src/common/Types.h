#ifndef __TYPES_H_
#define __TYPES_H_

#include "IShield.h"
#include "CEnemyBase.h"
#include "CBulletBase.h"
#include "CEffectBase.h"
#include "CBodyPartBase.h"
#include "CWeaponBase.h"

class CBasicShield : public IShield
{
public: // From IShield
	CBasicShield();
	const char* Name() const;
	int Cost() const;
	int Maximum() const;
};

class CBodyPartHead : public CBodyPartBase
{
public:
	CBodyPartHead();
};

class CBodyPartArm : public CBodyPartBase
{
public:
	CBodyPartArm();
};

class CBodyPartChest : public CBodyPartBase
{
public:
	CBodyPartChest();
};

class CBodyPartEntrails : public CBodyPartBase
{
public:
	CBodyPartEntrails();
};

class CBodyPartLeftLeg : public CBodyPartBase
{
public:
	CBodyPartLeftLeg();
};

class CBodyPartRightLeg : public CBodyPartBase
{
public:
	CBodyPartRightLeg();
};

class CBodyPartShit : public CBodyPartBase
{
public:
	CBodyPartShit();
};

class CBodyPartBurnedChest : public CBodyPartBase
{
public:
	CBodyPartBurnedChest();
};

class CBodyPartSkull : public CBodyPartBase
{
public:
	CBodyPartSkull();
};

class CBodyPartBones : public CBodyPartBase
{
public:
	CBodyPartBones();
};

class CEffectBlood : public CEffectBase
{
public:
	CEffectBlood();
};

class CEffectSmoke : public CEffectBase
{
public:
	CEffectSmoke();
};

class CEffectExplo : public CEffectBase
{
public:
	CEffectExplo();
};

class CEffectMetal : public CEffectBase
{
public:
	CEffectMetal();
};

class CEffectMetalYellow : public CEffectMetal
{
public:
	CEffectMetalYellow();
};

class CEffectMetalRed : public CEffectMetal
{
public:
	CEffectMetalRed();
};

class CEffectSmoke2 : public CEffectBase
{
public:
	CEffectSmoke2();
};

class CEffectBloodOnGround : public CEffectBase
{
public:
	CEffectBloodOnGround();
};

class CEffectGunSmoke : public CEffectBase
{
public:
	CEffectGunSmoke();
};

class CEffectSteam : public CEffectBase
{
public:
	CEffectSteam();
};

class CEffectTeleport : public CEffectBase
{
public:
	CEffectTeleport();
};

class CEffectFootprint : public CEffectBase
{
public:
	CEffectFootprint();
};

class CEffectFlame : public CEffectBase
{
public:
	CEffectFlame();
};

class CEffectFlame2 : public CEffectBase
{
public:
	CEffectFlame2();
};


class CEnemyPistolboy: public CEnemyBase
{
public:
	CEnemyPistolboy();
};

class CEnemyShotgunManiac: public CEnemyBase
{
public:
	CEnemyShotgunManiac();
};

class CEnemyUziRebel: public CEnemyBase
{
public:
	CEnemyUziRebel();
};

class CEnemyCommando: public CEnemyBase
{
public:
	CEnemyCommando();
};

class CEnemyBazookaBoy: public CEnemyBase
{
public:
	CEnemyBazookaBoy();
};

class CEnemyCivilian: public CEnemyBase
{
public:
	CEnemyCivilian();
};

class CEnemyPunisher: public CEnemyBase
{
public:
	CEnemyPunisher();
};

class CEnemyFlamer: public CEnemyBase
{
public:
	CEnemyFlamer();
};

class CEnemyPunk: public CEnemyBase
{
public:
	CEnemyPunk();
};

class CEnemyGrenadeGay: public CEnemyBase
{
public:
	CEnemyGrenadeGay();
};

class CWeaponFist : public CWeaponBase
{
public:
	CWeaponFist();
};

class CWeaponPistol : public CWeaponBase
{
public:
	CWeaponPistol();
};

class CWeaponShotgun : public CWeaponBase
{
public:
	CWeaponShotgun();
};

class CWeaponUzi : public CWeaponBase
{
public:
	CWeaponUzi();
};

class CWeaponAutoRifle : public CWeaponBase
{
public:
	CWeaponAutoRifle();
};

class CWeaponRocketLauncher : public CWeaponBase
{
public:
	CWeaponRocketLauncher();
};	

class CWeaponAutoLauncher : public CWeaponBase
{
public:	
	CWeaponAutoLauncher();
};	

class CWeaponHeavyLauncher : public CWeaponBase
{
public:
	CWeaponHeavyLauncher();
};	

class CWeaponAutoShotgun : public CWeaponBase
{
public:
	CWeaponAutoShotgun();
};	

class CWeaponC4Activator : public CWeaponBase
{
public:
	CWeaponC4Activator();
};	

class CWeaponFlameThrower : public CWeaponBase
{
public:
	CWeaponFlameThrower();
};	

class CWeaponMineDropper : public CWeaponBase
{
public:
	CWeaponMineDropper();
};

class CWeaponGrenadeLauncher : public CWeaponBase
{
public:
	CWeaponGrenadeLauncher();
};

class CBulletFist : public CBulletBase
{
public:
	CBulletFist();
};
class CBullet9mm : public CBulletBase
{
public:
	CBullet9mm();
};
class CBullet12mm : public CBulletBase
{
public:
	CBullet12mm();
};
class CBulletShotgunShells : public CBulletBase
{
public:
	CBulletShotgunShells();
};

class CBulletRocketBase : public CBulletBase
{
public:
	CBulletRocketBase();
	int EffectAngle( int aOrigAngle ) const;
	float EffectSpeed() const;
	bool DoEffect( int aCounter ) const;

};

class CBulletLightRockets : public CBulletRocketBase
{
public:
	CBulletLightRockets();
};
class CBulletMediumRockets : public CBulletRocketBase
{
public:
	CBulletMediumRockets();
};
class CBulletHeavyRockets : public CBulletRocketBase
{
public:
	CBulletHeavyRockets();
};
class CBulletC4Explosives : public CBulletBase
{
public:
	CBulletC4Explosives();
};
class CBulletGas : public CBulletBase
{
public:
	CBulletGas();
};
class CBulletMines : public CBulletBase
{
public:
	CBulletMines();
};
class CBulletGrenades : public CBulletBase
{
public:
	CBulletGrenades();
};

class CBulletExplosiveRay : public CBulletBase
{
public:
	CBulletExplosiveRay();
	int EffectAngle( int aOrigAngle ) const;
	float EffectSpeed() const;
};

#endif
