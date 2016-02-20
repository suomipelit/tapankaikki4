#ifndef __CGAMETYPEDATA_H
#define __CGAMETYPEDATA_H

#include "defines.h"

#include "IEffectType.h"
#include "IBodyPartType.h"
#include "IBulletType.h"
#include "IWeaponType.h"
#include "IEnemyType.h"
#include "IShield.h"

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


