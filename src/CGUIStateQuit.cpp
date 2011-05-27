#include "common/CEventHandler.h"
#include "common/CFonts.h"

#include "IGUIStateController.h"
#include "CGUIStateQuit.h"
#include "CGameState.h"
#include "CGameGraphicsInterface.h"
#include "texts.h"

CGUIStateQuit::CGUIStateQuit(IGUIStateController* aStateController):iStateController(aStateController)
{
}

void CGUIStateQuit::Draw(CDrawArea& aDirtyArea,CDrawArea& aDrawArea, CGameGraphicsInterface* aGGI)
{	
	int startx, starty;
	int endx,endy;
	const int boxsizeextra=10;
	const CFonts* font=aGGI->Font(CGameGraphicsInterface::EBigMenuFont);
	CRect<int> rect;

	startx=aGGI->GD()->Width()/2-font->Length(KQuitConfirmText)/2;
	starty=aGGI->GD()->Height()/2-font->Height()/2;
	endx=aGGI->GD()->Width()/2+font->Length(KQuitConfirmText)/2;
	endy=aGGI->GD()->Height()/2+font->Height()/2;

	rect.Set(startx-boxsizeextra,starty-boxsizeextra,endx+boxsizeextra,endy+boxsizeextra);
	aGGI->DrawBuffer()->DrawBox(rect,10);
	aDrawArea.Combine(rect);	
	aDrawArea.Combine(font->Write(startx,starty,KQuitConfirmText,CFonts::EDrawBelow, CFonts::EDrawToRight,aGGI->DrawBuffer()));
}


bool CGUIStateQuit::RunFrame()
{
	CEventHandler* eh = iStateController->EventHandler();

	if (eh->Kbhit())
	{
		if (eh->Getch().sym == SDLK_y)
			iStateController->State()->SetState(CState(CState::EMainStateConfirmedQuit,0));
		else
			iStateController->State()->SetState(CState(CState::EMainStateMenu,0));
	}

	return true;
}

void CGUIStateQuit::EnterState()
{
	iStateController->EventHandler()->ResetStack();
}

void CGUIStateQuit::ExitState()
{
	iStateController->SetFullScreenUpdate(true);
}
void CGUIStateQuit::EnterSubState(){};
void CGUIStateQuit::ExitSubState(){};

