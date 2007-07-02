#ifndef __IEFFECTTYPE_H_
#define __IEFFECTTYPE_H_

#include "defines.h"
#include "CGameTypeData.h"

class IEffectType
{
public:
	virtual ~IEffectType()=0;
	virtual int TCount() const=0;
	virtual int Order() const=0;
	virtual int Delay() const=0;
	virtual bool Random() const=0;// boolean
	virtual bool Sprite() const=0; // boolean
    virtual int XS() const=0;
	virtual int YS() const=0; 
	virtual int Offset() const=0;
	virtual TLightTable LightTable() const=0;
	virtual int LightSize() const=0;
	virtual int LightAdd(int aCounter) const=0;
	// counter multiplication when too many effects on screen
	virtual int LightOverloadMul() const=0;
	virtual int HeavyOverloadMul() const=0;
	virtual TEffect AfterDeathEffect() const=0;
	virtual bool TransparentSprite() const=0;
};

#endif

