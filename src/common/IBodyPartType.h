#ifndef __IBODYPARTTYPE_H_
#define __IBODYPARTTYPE_H_

#include "defines.h"

class IBodyPartType
{
public:
	virtual ~IBodyPartType()=0;
	virtual float Slow() const=0;		// kitka
	virtual float Kickspeed() const=0;	// vauhti millä lähtee ku potkasee
	virtual int Bounce() const=0;		// boolean 1=kimpoo 0=not kimpoo
	virtual int DropBlood() const=0;		// boolean 1=pudottaa verta 0=not
	virtual TIngameSoundEffect Sound() const=0; // sound when kicked or hits the wall
	virtual int SpriteOffset() const=0; // offset in sprite image
};

#endif

