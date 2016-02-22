#ifndef __CBODYPARTBASE_H
#define __CBODYPARTBASE_H

#include "IBodyPartType.h"
#include "defines.h"

class CBodyPartBase : public IBodyPartType
{
public:
	CBodyPartBase();
	float Slow() const;
	float Kickspeed() const;
	int Bounce() const;	
	int DropBlood() const;
	TIngameSoundEffect Sound() const;
	int SpriteOffset() const;

protected:
	float iSlow;		// kitka
	float iKickspeed;	// vauhti millä lähtee ku potkasee
	int iBounce;		// boolean 1=kimpoo 0=not kimpoo
	int iDropBlood;		// boolean 1=pudottaa verta 0=not
	TIngameSoundEffect iSound; // sound when kicked or hits the wall
	int iSpriteOffset;
};

#endif
