#ifndef __EVENTFILTER_H
#define __EVENTFILTER_H

#include <SDL.h>
#include "../common/IEventInterface.h"

class CGraphicsDevice;
class CEditorDrawer;
class CEditorLevelIO;
class CStateHandler;
class CEditorSelection;

class CEventFilter final : public IEventInterface
{
public:
	CEventFilter(CGraphicsDevice* aGD,CEditorDrawer* aDrawer,CEditorLevelIO* aLevelIO,CStateHandler* aStateHandler, CEditorSelection* aLevelSelection);

	bool HandleEvent(SDL_Event event);


private:
	CGraphicsDevice* iGD;
	CEditorDrawer* iDrawer;
	CEditorLevelIO* iLevelIO;
	CStateHandler* iStateHandler;
	CEditorSelection* iLevelSelection;
};

#endif
