#include "CEventHandler.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "CGraphicsDevice.h"
#include "IEventInterface.h"

const char* CEventHandler::KeyToStr(SDL_Keycode key)
{
	return SDL_GetKeyName(SDL_Keycode(key));
}

CMouse& CEventHandler::GetMouse()
{
	return *iMouse;
}

//SDL_Keysym CEventHandler::CheckGetch()
//{
//	int tmpEnd;
//
//	tmpEnd=iStackEnd+1;
//	while (tmpEnd>=KStackLength) tmpEnd-=KStackLength;	
//	return iStack[tmpEnd];
//}

SDL_Keysym CEventHandler::Getch()
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
}

void CEventHandler::GrabInputs(bool aGrab)
{
	SDL_SetRelativeMouseMode(SDL_bool(aGrab));
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

void CEventHandler::PushKey(SDL_Keysym aKSYM)
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
					if (SDL_GetWindowFlags(iGD->GetSDLwindow()) & SDL_WINDOW_INPUT_FOCUS)
					{
						iMouse->SetXPos(event.motion.x);
						iMouse->SetYPos(event.motion.y);
						iMouse->AddXRel(event.motion.xrel);
						iMouse->AddYRel(event.motion.yrel);
					}
					break;

				case SDL_MOUSEBUTTONDOWN:
					if (SDL_GetWindowFlags(iGD->GetSDLwindow()) & SDL_WINDOW_INPUT_FOCUS)
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

				case SDL_MOUSEWHEEL:
					if (SDL_GetWindowFlags(iGD->GetSDLwindow()) & SDL_WINDOW_INPUT_FOCUS)
					{
						if (event.wheel.y > 0)
							iMouse->IncButton(CMouse::EButtonWheelUp);
						else
							iMouse->IncButton(CMouse::EButtonWheelDown);
					}
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
