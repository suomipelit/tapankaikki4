#include "CEventHandler.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "CGraphicsDevice.h"
#include "IEventInterface.h"

char* CEventHandler::KeyToStr(int key)
{
	char *ptr;
	ASSERT(key>=SDLK_UNKNOWN);
	ASSERT(key<SDLK_LAST);

	ptr=SDL_GetKeyName(SDLKey(key));

   	return ptr;
}

CMouse& CEventHandler::GetMouse()
{
	return *iMouse;
}

//SDL_keysym CEventHandler::CheckGetch()
//{
//	int tmpEnd;
//
//	tmpEnd=iStackEnd+1;
//	while (tmpEnd>=KStackLength) tmpEnd-=KStackLength;	
//	return iStack[tmpEnd];
//}

SDL_keysym CEventHandler::Getch()
{
	ASSERT( iStackHead!=iStackEnd );

    iStackEnd++;
	while (iStackEnd>=KStackLength) iStackEnd-=KStackLength;
        
	return iStack[iStackEnd];
}

int CEventHandler::Kbhit()
{
    if (iStackHead!=iStackEnd) return(1);
    return(0);
}

CEventHandler::CEventHandler(CGraphicsDevice* aGD)
{
    int a;

	iGD=aGD;
	iMouse=new CMouse();

	for (a=0;a<KKeyTableLength;a++) 
		iState[a]=0;
    iStackHead=0;
    iStackEnd=iStackHead;
	SDL_EnableKeyRepeat(250, 30);
	SDL_EnableUNICODE(1);
}

void CEventHandler::GrabInputs(bool aGrab)
{
	if (aGrab)
		SDL_WM_GrabInput(SDL_GRAB_ON);
	else
		SDL_WM_GrabInput(SDL_GRAB_OFF);
}

CEventHandler::~CEventHandler()
{
	delete(iMouse);
	RemoveAllEventHandlers();
}

void CEventHandler::ResetStack()
{
    iStackEnd=iStackHead;
}

void CEventHandler::PushKey(SDL_keysym aKSYM)
{
	iStackHead++;
	if (iStackHead==iStackEnd) {iStackHead--;return;}
	while (iStackHead>=KStackLength) iStackHead-=KStackLength;

    iStack[iStackHead]=aKSYM;
}

int CEventHandler::HandleEvents()
{
	SDL_Event event;
	IEventInterface* handler;
	unsigned int a;
	char *tmp=0;
	
	/* Poll for events */
    while(iGD->SurfaceOK()&&SDL_PollEvent(&event))
	{
		bool ret=false;
		for (a=0;a<iHandlers.size()&&!ret;a++)
		{
			handler=iHandlers[a];
			ret = handler->HandleEvent(event);
		}

		if ( !ret )
			switch( event.type )
			{
				/* Keyboard event */
				case SDL_KEYDOWN:

					// Check the boundary of table
					if (event.key.keysym.sym<KKeyTableLength)
						State(event.key.keysym.sym)=1;

					PushKey(event.key.keysym);
					break;

				case SDL_KEYUP:
					State(event.key.keysym.sym)=0;                        
					break;

				case SDL_MOUSEMOTION:
					if (SDL_GetAppState()&SDL_APPINPUTFOCUS)
					{
						iMouse->SetXPos(event.motion.x);
						iMouse->SetYPos(event.motion.y);
						iMouse->AddXRel(event.motion.xrel);
						iMouse->AddYRel(event.motion.yrel);
					}
					break;

				case SDL_MOUSEBUTTONDOWN:
					if (SDL_GetAppState()&SDL_APPINPUTFOCUS)
					{
						switch (event.button.button)
						{
							case SDL_BUTTON_LEFT:
								iMouse->IncButton(CMouse::EButtonLeft);
								break;
							case SDL_BUTTON_MIDDLE:
								iMouse->IncButton(CMouse::EButtonMiddle);
								break;
							case SDL_BUTTON_RIGHT:
								iMouse->IncButton(CMouse::EButtonRight);
								break;
							case SDL_BUTTON_WHEELUP:
								iMouse->IncButton(CMouse::EButtonWheelUp);
								break;
							case SDL_BUTTON_WHEELDOWN:
								iMouse->IncButton(CMouse::EButtonWheelDown);
								break;
						};
					}
					break;

				case SDL_MOUSEBUTTONUP:
					switch (event.button.button)
					{
						case SDL_BUTTON_LEFT:
							iMouse->DecButton(CMouse::EButtonLeft);
							break;
						case SDL_BUTTON_MIDDLE:
							iMouse->DecButton(CMouse::EButtonMiddle);
							break;
						case SDL_BUTTON_RIGHT:
							iMouse->DecButton(CMouse::EButtonRight);
							break;
					};

					break;

				/* SDL_QUIT event (window close) */
				case SDL_QUIT:
					return 1;
					break;

				default:
					break;
					}
    }
	return 0;
};

char volatile& CEventHandler::State(int index)
{
	return iState[index];
}

void CEventHandler::AddEventHandler(IEventInterface* aHandler)
{
	iHandlers.push_back(aHandler);
}

void CEventHandler::RemoveAllEventHandlers()
{
	iHandlers.clear();
}
