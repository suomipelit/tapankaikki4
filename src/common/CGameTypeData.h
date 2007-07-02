#ifndef __GAMETYPEDATA_H_
#define __GAMETYPEDATA_H_

#include "defines.h"

enum TBodyPart
{
	EBodyPartHead,
	EBodyPartArm,
	EBodyPartChest,
	EBodyPartEntrails,
	EBodyPartLeftLeg,
	EBodyPartRightLeg,
	EBodyPartShit,
	EBodyPartBurnedChest,
	EBodyPartSkull,
	EBodyPartBones,
	EBodyPartAmount
};

// Effects...
enum TEffect
{
	EEffectNoEffect=-1,
	EEffectBlood=0,
	EEffectSmoke,
	EEffectExplo,
	EEffectMetal,
	EEffectMetalYellow,
	EEffectMetalRed,
	EEffectSmoke2,
	EEffectBloodOnGround,
	EEffectGunSmoke,
	EEffectSteam,
	EEffectTeleport,
	EEffectFootprint,
	EEffectFlame,
	EEffectFlame2,
	EEffectAmount
};

enum TEnemy
{
	EEnemyPistolboy=0,
	EEnemyShotgunManiac,
	EEnemyUziRebel,
	EEnemyCommando,
	EEnemyBazookaBoy,
	EEnemyCivilian,
	EEnemyPunisher,
	EEnemyFlamer,
	EEnemyPunk,
	EEnemyGrenadeGay,
	EEnemyAmount
};

enum TWeapon
{
	EWeaponFist=0,
	EWeaponPistol,
	EWeaponShotgun,
	EWeaponUzi,
	EWeaponAutoRifle,
	EWeaponRocketLauncher,
	EWeaponAutoLauncher,
	EWeaponHeavyLauncher,
	EWeaponAutoShotgun,
	EWeaponC4Activator,
	EWeaponFlameThrower,
	EWeaponMineDropper,
	EWeaponGrenadeLauncher,
	EWeaponAmount
};

enum TBullet
{
	EBulletFist=0,
	EBullet9mm,
	EBullet12mm,
	EBulletShotgunShells,
	EBulletLightRockets,
	EBulletMediumRockets,
	EBulletHeavyRockets,
	EBulletC4Explosives,
	EBulletGas,
	EBulletMines,
	EBulletGrenades,
	EBulletExplosionRay,
	EBulletAmount
};

class IShield;
class IBodyPartType;
class IBulletType;
class IWeaponType;
class IEnemyType;
class IEffectType;

class CGameTypeData
{
public:
	EXPORT CGameTypeData();
	EXPORT ~CGameTypeData();

	EXPORT int ReverseMapBullet(const IBulletType* aBullet) const;
	EXPORT int ReverseMapWeapon(const IWeaponType* aWeapon) const;
	EXPORT int ReverseMapEnemy(const IEnemyType* aEnemy) const;

	IShield* iShieldType;
	IBodyPartType* iBodyPartType[EBodyPartAmount];
	IBulletType* iBulletType[EBulletAmount];
	IWeaponType* iWeaponType[EWeaponAmount];
	IEnemyType* iEnemyType[EEnemyAmount];
	IEffectType* iEffectType[EEffectAmount];

private:
	void Load();
	void SetupBodyPartTypes();
	void SetupBulletTypes();
	void SetupWeaponTypes();
	void SetupEnemyTypes();
	void SetupEffects();
	void SetupShieldData();
};

#endif


