#ifndef __EVENTFILTER_H_
#define __EVENTFILTER_H_

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

