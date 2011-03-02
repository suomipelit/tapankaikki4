#ifndef __BULLET_H_
#define __BULLET_H_

#include "common/Types.h"
#include "common/CCoord.h"
#include "common/IBulletType.h"

class IBulletObserver;
class CPlayer;
class CDynGameData;
class CSoundPlayer;
class CGameSystem;
class CGameData;

class CBullet
{
protected:
	bool CheckWalls(TDirection aMode, const CCoord<int>& aNewLoc);
	void Bounce(IBulletType::TBounceType aBounceType, const CCoord<int>& aNewLoc);

protected:
	float iSpeed;
    int iAngle;
	float iSpriteAngle;
	float iSpriteAngleAdd;
    int iTime;
	int iCounter;
	float iShareOfOriginalShot;
	IBulletObserver *iOwner;

	bool iHitReported;

	CCoord<float> iLocation;
	CGameData* iGM;
	CSoundPlayer* iSP;
	CDynGameData* iDynData;
	const IBulletType *iType;

public:
	CBullet(IBulletType* aType,
		CCoord<float> aPos,
		IBulletObserver* aObserver,
		int aAngle,
		int aTime,
		float aShareOfOriginalShot,
		CDynGameData* aDynData,
		CSoundPlayer* aSP,
		CGameData* aGM);

	int Move();
	inline const CCoord<float>& Location(){ return iLocation;};
	inline const IBulletType *Type() const { return iType; };
	int Angle() const { return iAngle; };
	float SpriteAngle() const { return iSpriteAngle; };
	int Counter() const { return iCounter; };

};

#endif


