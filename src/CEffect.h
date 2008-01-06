#ifndef __CEFFECT_H
#define __CEFFECT_H

#include "common/CCoord.h"
#include "common/debug.h"

class IEffectType;
class CLevelRuntimeData;

class CEffect
{
public:

	CEffect(IEffectType* aType, const CCoord<float> aPos,int aCount, int aAngle, float aSpeed);
	int Roll(CLevelRuntimeData* aLevelRuntime, int aAmountOfEffects);
	inline CCoord<float>& Location()
	{
		return iLocation;
	};
	inline const IEffectType* Type() const { return iType; };
	inline int Counter() const { return iCounter; };
	
	// for binary search algorithms
	operator const CCoord<float>& () const
	{
		return iLocation;
	}

protected:
	CCoord<float> iVec;
	
	int iCounter;

private:
	const IEffectType *iType;
	CCoord<float> iLocation;

	__DECLARE_TEST;
};

#endif
