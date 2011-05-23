#include "CEffectBase.h"
#include "defines.h"

CEffectBase::CEffectBase()
{
	iTCount=0;
	iOrder=0;
	iDelay=0;
	iRandom=0;// boolean
	iSprite=0; // boolean
	iXS=0;
	iYS=0; 
	iOffset=0;
	iLightTable= ELightTableNoLight;
	iLightSize=0;
	iLightAddConst=0;
	iLightAddMultiplier=0;
	iLightOLMul=1;
	iHeavyOLMul=1;
	iAfterDeathEffect = EEffectNoEffect;
	iTransparentSprite = false;
}

int CEffectBase::TCount() const
{
	return iTCount;
}

int CEffectBase::Order() const
{
	return iOrder;
}

int CEffectBase::Delay() const
{
	return iDelay;
}

bool CEffectBase::Random() const
{
	return iRandom;
}

bool CEffectBase::Sprite() const
{
	return iSprite;
}

int CEffectBase::XS() const
{
	return iXS;
}

int CEffectBase::YS() const
{
	return iYS;
}

int CEffectBase::Offset() const
{
	return iOffset;
}

TLightTable CEffectBase::LightTable() const
{
	return iLightTable;
}

int CEffectBase::LightSize() const
{
	return iLightSize;
}

int CEffectBase::LightAdd(int aCounter) const
{
	return int(iLightAddConst + aCounter*iLightAddMultiplier);
}

int CEffectBase::LightOverloadMul() const
{
	return iLightOLMul;
}

int CEffectBase::HeavyOverloadMul() const
{
	return iHeavyOLMul;
}

TEffect CEffectBase::AfterDeathEffect() const
{
	return iAfterDeathEffect;
}

bool CEffectBase::TransparentSprite() const
{
	return iTransparentSprite;
}
