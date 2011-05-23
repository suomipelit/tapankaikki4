#ifndef __IPOSITIONALSOUNDOBSERVER_H_
#define __IPOSITIONALSOUNDOBSERVER_H_

#include "common/CCoord.h"

class IPositionalSoundObserver
{
public:
	virtual void SoundHeard(enum TIngameSoundEffect aEffect,float aVolume, CCoord<int> aPos) = 0;
};

#endif

