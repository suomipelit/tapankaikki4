#include "StateNewLevel.h"
#include "EditorDrawer.h"
#include "EditorBrush.h"
#include "EditorGraphics.h"
#include "StateHandler.h"
#include "../common/CEditableLevel.h"
#include "../common/CEventHandler.h"

CStateNewLevel::CStateNewLevel(CStateHandler* aStateHandler,CEditorBrush* aBrush,CEditableLevel* aLevel): CEditorState(aBrush,aLevel,aStateHandler)
{
	iNewWidth[0]=0;
	iNewHeight[0]=0;
	iCurKey=0;
	iCursorXPos=0;
	iCursorYPos=0;
}

CStateNewLevel::~CStateNewLevel(void)
{
}

void CStateNewLevel::Draw(CEditorDrawer* aDrawer)
{
		aDrawer->DrawNewLevelPrompt(iNewWidth,iNewHeight);
}

int MaxRange(int Value,int Max)
{
	if (Value>Max) return (Max);
	else return (Value);
}

void CStateNewLevel::HandleKeys(CEventHandler* aEventHandler,CEditorDrawer* aDrawer)
{
	if (! aEventHandler->Kbhit()) return;

		char* NewPointer;
		int IntWidth,IntHeight;

		iCurKey = aEventHandler->Getch().sym;

		aEventHandler->ResetStack();

		if (iCurKey == SDLK_ESCAPE)
		{
			iCursorYPos = 0;
			iCursorXPos = 0;
			iNewWidth[0]=0;
			iNewHeight[0]=0;
			iStateHandler->SetPrevious();
			aEventHandler->ResetStack();
			aEventHandler->State(SDLK_ESCAPE)=0;
		}
		if (iCurKey == SDLK_RETURN)
		{
			if (iCursorYPos < 1) 
			{
				iCursorYPos++; 
				iCursorXPos=0;
			} 
			else
			{
				IntWidth=atoi(iNewWidth);
				if (IntWidth > KMaxLevelWidth) IntWidth = KMaxLevelWidth;
				else
					if (IntWidth < KMinLevelWidth) IntWidth = KMinLevelWidth;
				IntHeight=atoi(iNewHeight);
				if (IntHeight > KMaxLevelHeight) IntHeight = KMaxLevelHeight;
				else
					if (IntHeight < KMinLevelHeight) IntHeight = KMinLevelHeight;
				iLevel->New(IntWidth,IntHeight,iBrush->RightBlock()->iType,iBrush->RightBlock()->iNumber,iBrush->RightBlock()->iHeight);
				iCursorYPos = 0;
				iCursorXPos = 0;
				iNewWidth[0]=0;
				iNewHeight[0]=0;
				iStateHandler->SetPrevious();
				aEventHandler->ResetStack();
				aEventHandler->State(SDLK_RETURN)=0;
			}
		}

		if (iCursorYPos==0) NewPointer=iNewWidth;
		if (iCursorYPos==1) NewPointer=iNewHeight;

		if (iCurKey >= SDLK_0 && iCurKey <= SDLK_9)
		{
			NewPointer[MaxRange(iCursorXPos,MaxDigits-1)] = char(iCurKey - SDLK_0 + '0');
			NewPointer[MaxRange(iCursorXPos+1,MaxDigits)] = 0;
			if (iCursorXPos < MaxDigits) iCursorXPos++;
		}

		if (iCurKey == SDLK_BACKSPACE && iCursorXPos > 0)
		{
			iCursorXPos--;
			NewPointer[iCursorXPos] = 0;
		}
}

void CStateNewLevel::HandleMouse(CEventHandler* aEventHandler,CEditorDrawer* aDrawer)
{
}
