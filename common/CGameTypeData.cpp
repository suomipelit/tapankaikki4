#include <string.h>
#include "error.h"
#include "CGameTypeData.h"
#include "Types.h"

CGameTypeData::CGameTypeData()
{
	unsigned int a;

	// Empty all
	for (a=0;a<EBodyPartAmount;a++) 
		iBodyPartType[a]=NULL;
	for (a=0;a<EBulletAmount;a++) 
		iBulletType[a]=NULL;
	for (a=0;a<EWeaponAmount;a++) 
		iWeaponType[a]=NULL;	
	for (a=0;a<EEnemyAmount;a++) 
		iEnemyType[a]=NULL;
	for (a=0;a<EEffectAmount;a++) 
		iEffectType[a]=NULL;
	iShieldType=NULL;	
	
	Load();

	// Make sure that all types are loaded	
//	for (a=0;a<EBodyPartAmount;a++) 
//		ASSERT(iBodyPartType[a]);
	for (a=0;a<EBulletAmount;a++) 
		ASSERT(iBulletType[a]!=NULL);
	for (a=0;a<EWeaponAmount;a++) 
		ASSERT(iWeaponType[a]!=NULL);	
	for (a=0;a<EEnemyAmount;a++) 
		ASSERT(iEnemyType[a]!=NULL);
	for (a=0;a<EEffectAmount;a++) 
		ASSERT(iEffectType[a]!=NULL);
	ASSERT(iShieldType!=NULL);
}

CGameTypeData::~CGameTypeData()
{
	unsigned int a;

	// Empty all
	for (a=0;a<EBodyPartAmount;a++) 
		delete iBodyPartType[a];
	for (a=0;a<EBulletAmount;a++) 
		delete iBulletType[a];
	for (a=0;a<EWeaponAmount;a++) 
		delete iWeaponType[a];	
	for (a=0;a<EEnemyAmount;a++) 
		delete iEnemyType[a];
	for (a=0;a<EEffectAmount;a++) 
		delete iEffectType[a];
	delete iShieldType;	
}

void CGameTypeData::Load()
{
	SetupBulletTypes();
	SetupBodyPartTypes();
	SetupWeaponTypes();
	SetupEnemyTypes();
	SetupEffects();
	SetupShieldData();
}

void CGameTypeData::SetupBodyPartTypes()
{
	iBodyPartType[EBodyPartHead]= new CBodyPartHead;
	iBodyPartType[EBodyPartArm]= new CBodyPartArm;
	iBodyPartType[EBodyPartChest]= new CBodyPartChest;
	iBodyPartType[EBodyPartEntrails]= new CBodyPartEntrails;
	iBodyPartType[EBodyPartLeftLeg]= new CBodyPartLeftLeg;
	iBodyPartType[EBodyPartRightLeg]= new CBodyPartRightLeg;
	iBodyPartType[EBodyPartShit]= new CBodyPartShit;
	iBodyPartType[EBodyPartBurnedChest]= new CBodyPartBurnedChest;
	iBodyPartType[EBodyPartSkull]= new CBodyPartSkull;
	iBodyPartType[EBodyPartBones]= new CBodyPartBones;
}

void CGameTypeData::SetupEnemyTypes()
{
	iEnemyType[EEnemyPistolboy]=new CEnemyPistolboy;
	iEnemyType[EEnemyShotgunManiac]=new CEnemyShotgunManiac;
	iEnemyType[EEnemyUziRebel]=new CEnemyUziRebel;
	iEnemyType[EEnemyCommando]=new CEnemyCommando;
	iEnemyType[EEnemyBazookaBoy]=new CEnemyBazookaBoy;
	iEnemyType[EEnemyCivilian]=new CEnemyCivilian;
	iEnemyType[EEnemyPunisher]=new CEnemyPunisher;
	iEnemyType[EEnemyFlamer]=new CEnemyFlamer;
	iEnemyType[EEnemyPunk]=new CEnemyPunk;
	iEnemyType[EEnemyGrenadeGay]=new CEnemyGrenadeGay;
}

void CGameTypeData::SetupWeaponTypes()
{
	iWeaponType[EWeaponFist]=new CWeaponFist;
	iWeaponType[EWeaponPistol]=new CWeaponPistol;
	iWeaponType[EWeaponShotgun]=new CWeaponShotgun;
	iWeaponType[EWeaponUzi]=new CWeaponUzi;
	iWeaponType[EWeaponAutoRifle]=new CWeaponAutoRifle;
	iWeaponType[EWeaponRocketLauncher]=new CWeaponRocketLauncher;
	iWeaponType[EWeaponAutoLauncher]=new CWeaponAutoLauncher;
	iWeaponType[EWeaponHeavyLauncher]=new CWeaponHeavyLauncher;
	iWeaponType[EWeaponAutoShotgun]=new CWeaponAutoShotgun;
	iWeaponType[EWeaponC4Activator]=new CWeaponC4Activator;
	iWeaponType[EWeaponFlameThrower]=new CWeaponFlameThrower;
	iWeaponType[EWeaponMineDropper]=new CWeaponMineDropper;
	iWeaponType[EWeaponGrenadeLauncher]=new CWeaponGrenadeLauncher;
}

void CGameTypeData::SetupBulletTypes() 
{
	iBulletType[EBulletFist]=new CBulletFist;
	iBulletType[EBullet9mm]=new CBullet9mm;
	iBulletType[EBullet12mm]=new CBullet12mm;
	iBulletType[EBulletShotgunShells]=new CBulletShotgunShells;
	iBulletType[EBulletLightRockets]=new CBulletLightRockets;
	iBulletType[EBulletMediumRockets]=new CBulletMediumRockets;
	iBulletType[EBulletHeavyRockets]=new CBulletHeavyRockets;
	iBulletType[EBulletC4Explosives]=new CBulletC4Explosives;
	iBulletType[EBulletGas]=new CBulletGas;
	iBulletType[EBulletMines]=new CBulletMines;
	iBulletType[EBulletGrenades]=new CBulletGrenades;
	iBulletType[EBulletExplosionRay]=new CBulletExplosiveRay;
}

void CGameTypeData::SetupEffects()	
{
	iEffectType[EEffectBlood]=new CEffectBlood;
	iEffectType[EEffectSmoke]=new CEffectSmoke;
	iEffectType[EEffectExplo]=new CEffectExplo;
	iEffectType[EEffectMetal]=new CEffectMetal;
	iEffectType[EEffectMetalYellow]=new CEffectMetalYellow;
	iEffectType[EEffectMetalRed]=new CEffectMetalRed;
	iEffectType[EEffectSmoke2]=new CEffectSmoke2;
	iEffectType[EEffectBloodOnGround]=new CEffectBloodOnGround;
	iEffectType[EEffectGunSmoke]=new CEffectGunSmoke;
	iEffectType[EEffectSteam]=new CEffectSteam;
	iEffectType[EEffectTeleport]=new CEffectTeleport;
	iEffectType[EEffectFootprint]=new CEffectFootprint;
	iEffectType[EEffectFlame]=new CEffectFlame;
	iEffectType[EEffectFlame2]=new CEffectFlame2;
};

void CGameTypeData::SetupShieldData()
{
	iShieldType=new CBasicShield;
};

int CGameTypeData::ReverseMapBullet(const IBulletType* aBullet) const
{
	int a;

	for (a=0;a<EBulletAmount;a++)
		if (iBulletType[a]==aBullet)
			return a;

	error("CGameTypeData::ReverseMapBullet: failure");
	return -1;
}

int CGameTypeData::ReverseMapWeapon(const IWeaponType* aWeapon) const
{
	int a;

	for (a=0;a<EWeaponAmount;a++)
		if (iWeaponType[a]==aWeapon)
			return a;

	error("CGameTypeData::ReverseMapWeapon: failure");
	return -1;
}

int CGameTypeData::ReverseMapEnemy(const IEnemyType* aEnemy) const
{
	int a;

	for (a=0;a<EEnemyAmount;a++)
		if (iEnemyType[a]==aEnemy)
			return a;

	error("CGameTypeData::ReverseMapEnemy: failure");
	return -1;
}
