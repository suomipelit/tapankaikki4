#ifndef __EVENTFILTER_H_
#define __EVENTFILTER_H_

#include <SDL.h>
#include "../common/IEventInterface.h"

class CGraphicsDevice;
class CEditorDrawer;
class CEditorLevelIO;
class CStateHandler;
class CEditorSelection;

class CEventFilter: public IEventInterface
{
public:
	CEventFilter(CGraphicsDevice* aGD,CEditorDrawer* aDrawer,CEditorLevelIO* aLevelIO,CStateHandler* aStateHandler, CEditorSelection* aLevelSelection);
	~CEventFilter(void);

	bool HandleEvent(SDL_Event event);


private:
	CGraphicsDevice* iGD;
	CEditorDrawer* iDrawer;
	CEditorLevelIO* iLevelIO;
	CStateHandler* iStateHandler;
	CEditorSelection* iLevelSelection;
};

#endif
