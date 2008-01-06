#ifndef __CEVENTFILTER_H
#define __CEVENTFILTER_H

#include "common/IEventInterface.h"

class IGUIStateController;

class CEventFilter: public IEventInterface
{
public:
	CEventFilter(IGUIStateController* aGA);
	~CEventFilter(void);

	// From IEventInterface
	bool HandleEvent(SDL_Event event);
private:

	IGUIStateController* iGA;
};

#endif

