#include "StateHelp.h"
#include "EditorDrawer.h"
#include "EditorBrush.h"
#include "StateHandler.h"
#include "../common/CEditableLevel.h"
#include "../common/CEventHandler.h"

CStateHelp::CStateHelp(CStateHandler* aStateHandler,CEditorBrush* aBrush,CEditableLevel* aLevel): CEditorState(aBrush,aLevel,aStateHandler)
{
}

CStateHelp::~CStateHelp(void)
{
}

void CStateHelp::Draw(CEditorDrawer* aDrawer)
{
	aDrawer->DrawInfo();
}

void CStateHelp::HandleKeys(CEventHandler* aEventHandler,CEditorDrawer* aDrawer)
{
	if (! aEventHandler->Kbhit()) return;
	
	short iCurKey = aEventHandler->Getch().sym;

	aEventHandler->ResetStack();

	if (iCurKey == SDLK_ESCAPE)
	{
		iStateHandler->SetPrevious();
		aEventHandler->ResetStack();
		aEventHandler->State(SDLK_ESCAPE)=0;
	}
}

void CStateHelp::HandleMouse(CEventHandler* aEventHandler,CEditorDrawer* aDrawer)
{
}
