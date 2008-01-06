#ifndef __CEFFECTBASE_H
#define __CEFFECTBASE_H

#include "IEffectType.h"

class CEffectBase : public IEffectType
{
public:
	CEffectBase();
	int TCount() const;
	int Order() const;
	int Delay() const;
	bool Random() const;// boolean
	bool Sprite() const; // boolean
    int XS() const;
	int YS() const; 
	int Offset() const;
	TLightTable LightTable() const;
	int LightSize() const;
	int LightAdd(int aCounter) const;
	int LightOverloadMul() const;
	int HeavyOverloadMul() const;
	TEffect AfterDeathEffect() const;
	bool TransparentSprite() const;

protected:
	int iTCount;
	int iOrder;
	int iDelay;
	bool iRandom;// boolean
	bool iSprite; // boolean
    int iXS;
	int iYS; 
	int iOffset;
	TLightTable iLightTable;
	int iLightSize;
	int iLightAddConst;
	float iLightAddMultiplier;
	int iLightOLMul;
	int iHeavyOLMul;
	// effect which is launched after ending of this effect
	TEffect iAfterDeathEffect;
	// is effect drawn transparent or not
	bool iTransparentSprite;
};

#endif
