#include "EventFilter.h"
#include "../common/CGraphicsDevice.h"
#include "EditorDrawer.h"
#include "EditorLevelIO.h"
#include "StateHandler.h"
#include "EditorState.h"
#include "editorapp.h"
#include "EditorSelection.h"

extern volatile bool Refresh;

CEventFilter::CEventFilter(CGraphicsDevice* aGD, CEditorDrawer* aDrawer, CEditorLevelIO* aLevelIO, CStateHandler* aStateHandler, CEditorSelection* aLevelSelection)
{
	iGD = aGD;
	iDrawer = aDrawer;
	iLevelIO = aLevelIO;
	iStateHandler = aStateHandler;
	iLevelSelection = aLevelSelection;
}

CEventFilter::~CEventFilter(void)
{
}

bool CEventFilter::HandleEvent(SDL_Event	 event)
{
	if (event.type >= SDL_MOUSEMOTION && event.type <= SDL_MOUSEWHEEL)
		Refresh = true;
	if (event.type == SDL_KEYDOWN)
	{
		Refresh = true;
		switch (event.key.keysym.sym)
		{
			case SDLK_F3:
				if (iLevelIO->LoadLevel())
				{
					iDrawer->SetZoom(iDrawer->Zoom());
					iLevelSelection->Clear();
				}
				return true;
			break;
			case SDLK_F9:
				if (iLevelIO->LoadINI())
				{
					iDrawer->SetZoom(iDrawer->Zoom());
				}
				return true;
			break;
			case SDLK_F2: iLevelIO->SaveLevelAs();
				return true;
		}

		if (event.key.keysym.mod & KMOD_CTRL)
			if (event.key.keysym.sym == SDLK_s)
				{
					iLevelIO->SaveLevel();
					return true;
				}
	}
	if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
	{
		iGD->SetMode(event.window.data1,event.window.data2,KBitDepth,0,SDL_WINDOW_RESIZABLE);
		iDrawer->Resize(iGD->Width(),iGD->Height());
		Refresh = true;
		return true;
	}

	return false;
}
