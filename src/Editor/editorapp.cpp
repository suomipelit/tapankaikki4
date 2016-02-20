#include <SDL.h>
#include <SDL_version.h>
#include <SDL_syswm.h>
#include "editorapp.h"
#include "../common/CGraphicsDevice.h"
#include "../common/CEventHandler.h"
#include "../common/error.h"
#include "../common/CEditableLevel.h"
#include "../common/CMath.h"
#include "EventFilter.h"
#include "EditorGraphics.h"
#include "EditorDrawer.h"
#include "StateHandler.h"
#include "EditorState.h"
#include "EditorLevelIO.h"
#include "EditorBrush.h"
#include "EditorSelection.h"
#include "EditorMagicSelection.h"
#include "EditorTools.h"
#include "EditorClipBoard.h"

const int KFrameRate=20;

volatile bool Refresh = false; // Screen update flag
bool NewRefreshPossible = false;

static Uint32 TimerCallBack(Uint32 interval)
{
	NewRefreshPossible = true;		
	return interval;
}

CEditorApp::CEditorApp()
{
	iQuit=0;
	if (SDL_Init(SDL_INIT_TIMER)<0)
	{
		error("Unable to init SDL: %s\n", SDL_GetError());
	}
	atexit(SDL_Quit);

	SDL_SetTimer(1000/KFrameRate, TimerCallBack);
	SDL_SysWMinfo WndInfo; 

#ifndef __unix__ 
    WndInfo.window = 0;
#else
    WndInfo.info.x11.window = 0;
#endif
    SDL_VERSION(&WndInfo.version);
	
	iGD=new CGraphicsDevice("Tapan Kaikki Bloodshed - Level Editor","editor.ico");
	iGD->SetCursorMode(SDL_ENABLE);
#ifndef __unix__
	iGD->SetMode(640,480,8,0,SDL_RESIZABLE);
#else
	iGD->SetMode(640,480,0,0,SDL_RESIZABLE);
#endif
	
	if(SDL_GetWMInfo(&WndInfo)<0)  {
        /* Can't get WndInfo */
		error("Can't get WndInfo");
	}
	
	iGraphics = new CEditorGraphics();
	iLevel = new CEditableLevel();
	iEH = new CEventHandler(iGD);
	iClipBoard = new CEditorClipBoard();
	iLevelSelection = new CEditorSelection();
	iMagicSelection = new CEditorMagicSelection(iLevel,iLevelSelection);
	iBrush = new CEditorBrush(iLevel,iLevelSelection);
	iDrawer = new CEditorDrawer(iGD,iGraphics,iLevel,iLevelSelection,iBrush);
	iLevelIO = new CEditorLevelIO(iLevel,WndInfo,iDrawer,iGraphics,iGD);
	iTools = new CEditorTools(iBrush,iLevelSelection,iMagicSelection);
	iStateHandler = new CStateHandler(iTools,iLevel,iClipBoard);
	iEF=new CEventFilter(iGD,iDrawer, iLevelIO, iStateHandler,iLevelSelection);
	iEH->AddEventHandler(iEF);

	iGraphics->Load();
	iGD->SetPalette(*iGraphics->Palette(),256);

	iLevel->New(30,20,EBlockTypeFloor,0,0);
	iDrawer->SetZoom(2);

	iBrush->SetRightBlock(EBlockTypeFloor,0);
	iBrush->SetLeftBlock(EBlockTypeWall,0);

	iStateHandler->SetState(EEditModeEditLevel);

#ifdef __unix__
	iDrawer->ResetAnimationFlag();
	iStateHandler->CurrentState()->Draw(iDrawer);
	Refresh = iDrawer->UpdateScreen();
#endif
}

void CEditorApp::Run()
{
	do
	{
		// Tämä on main-looppi!
		if (Refresh && NewRefreshPossible)
		{
			iDrawer->ResetAnimationFlag();
			Refresh=false;
			NewRefreshPossible = false;
			iStateHandler->CurrentState()->HandleKeys(iEH,iDrawer);
			iStateHandler->CurrentState()->HandleMouse(iEH,iDrawer);
			iStateHandler->CurrentState()->Draw(iDrawer);
			Refresh = iDrawer->UpdateScreen();
		}
		else
			SDL_Delay( 10 );
	} while (!iEH->HandleEvents());
}


CEditorApp::~CEditorApp()
{
	delete iEF;
	delete iStateHandler;
	delete iTools;
	delete iLevelIO;
	delete iDrawer;
	delete iBrush;
	delete iMagicSelection;
	delete iLevelSelection;
	delete iClipBoard;
	delete iEH;
	delete iLevel;
	delete iGraphics;
	delete iGD;
}
