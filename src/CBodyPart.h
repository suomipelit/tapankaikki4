#ifndef __BODYPART_H_
#define __BODYPART_H_

#include "common/CCoord.h"
#include "common/Types.h"

class IBodyPartType;
class CDynGameData;
class CSoundPlayer;
class CLevelRuntimeData;

//enum TDirection {};

class CBodyPart
{
public:
	CBodyPart(CCoord<float> aPos, int aAngle,float aSpeed, IBodyPartType* aType,CSoundPlayer* aSP);
	void Move( CLevelRuntimeData* aLevelRuntimeData );
	void Kick(int aAngle);
	inline CCoord<float>& Location(){ return iLocation; };
	void Push(CCoord<float> aVec);
	int SpriteOffset();

	inline const IBodyPartType *Type() const{ return iType;}

	// for binary search algorithms
	operator const CCoord<float>& () const
	{
		return iLocation;
	}

private:
	CCoord<float> iLocation;
	IBodyPartType *iType;
	CSoundPlayer* iSP;
	int iAngle;
	float iSpeed;

	CDynGameData* iDynData;
	void SetAngleAndSpeed(const CCoord<float>& aVec);

//	void Bounce(enum TDirection aSide);
	void Bounce(TDirection aSide);
};

#endif

