#ifndef __EVENTHANDLER_H_
#define __EVENTHANDLER_H_

#include "CMouse.h"

#include <vector>
#include <SDL.h>
#include <SDL_thread.h>

#define KStackLength 0x200
#define KKeyTableLength 0x200

class CGraphicsDevice;
class IEventInterface;

class CEventHandler
{
public:
// methods
	EXPORT CEventHandler(CGraphicsDevice* aGD);
	EXPORT ~CEventHandler();

	EXPORT static char* KeyToStr(int key);
	//EXPORT SDL_keysym CheckGetch();
    EXPORT SDL_keysym Getch();
	EXPORT int Kbhit();
	EXPORT char volatile& State(int index);
	EXPORT void ResetStack();
	EXPORT CMouse& GetMouse();
	EXPORT int HandleEvents();
	EXPORT void PushKey(SDL_keysym key);
	EXPORT void AddEventHandler(IEventInterface* aHandler);
	EXPORT void RemoveAllEventHandlers();
	EXPORT void GrabInputs(bool aGrab);

private:

	SDL_keysym iStack[KStackLength];
	unsigned short iStackHead,iStackEnd;
	char iState[KKeyTableLength];
	CGraphicsDevice* iGD;
	CMouse* iMouse;

// members
	std::vector<IEventInterface*> iHandlers;
};

#endif



