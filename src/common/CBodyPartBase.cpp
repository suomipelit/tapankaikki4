#include "CBodyPartBase.h"

CBodyPartBase::CBodyPartBase()
{
	iSlow=0.0f;
	iKickspeed=0.0f;
	iBounce=0;
	iDropBlood=0;
	iSpriteOffset=0;
	iSound=ESoundEffectNoSound;
}

float CBodyPartBase::Slow() const
{
	return iSlow;
}

float CBodyPartBase::Kickspeed() const
{
	return iKickspeed;
}

int CBodyPartBase::Bounce() const
{
	return iBounce;
}

int CBodyPartBase::DropBlood() const
{
	return iDropBlood;
}

TIngameSoundEffect CBodyPartBase::Sound() const
{
	return iSound;
}

int CBodyPartBase::SpriteOffset() const
{
	return iSpriteOffset;
}
