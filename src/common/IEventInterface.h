#ifndef __IEVENTINTERFACE_H
#define __IEVENTINTERFACE_H

#include <SDL.h>

class IEventInterface
{
public:
	// return true if event is consumed
	bool virtual HandleEvent(SDL_Event event)=0;
};

#endif
