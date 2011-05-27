#include "StateEditGeneralData.h"
#include "EditorDrawer.h"
#include "EditorBrush.h"
#include "StateHandler.h"
#include "../common/CEditableLevel.h"
#include "../common/CEventHandler.h"

CStateEditGeneralData::CStateEditGeneralData(CStateHandler* aStateHandler,CEditorBrush* aBrush,CEditableLevel* aLevel): CEditorState(aBrush,aLevel,aStateHandler)
{
	iCommentMode = false;
	iCursorXPos = 0;
	iCursorYPos = 0;
}

CStateEditGeneralData::~CStateEditGeneralData(void)
{
}

void CStateEditGeneralData::Draw(CEditorDrawer* aDrawer)
{
	aDrawer->DrawGeneralData(iCursorXPos, iCursorYPos, iCommentMode);
}

void CStateEditGeneralData::HandleKeys(CEventHandler* aEventHandler,CEditorDrawer* aDrawer)
{
	const int KItems = EEnemyAmount + 2;

	if (! aEventHandler->Kbhit()) return;
	
	SDL_keysym key = aEventHandler->Getch();
	int curKey = key.sym;

	aEventHandler->ResetStack();

	if (curKey == SDLK_ESCAPE)
	{
		iCursorYPos = 0;
		iCursorXPos = 0;
		iStateHandler->SetPrevious();
		aEventHandler->ResetStack();
		aEventHandler->State(SDLK_ESCAPE)=0;
	}

	if (curKey == SDLK_RIGHT){
		if (iCursorYPos > 1 ) 
			iLevel->iGeneralLevelInfo.iEnemies[iCursorYPos-2]++;
		else
		if (iCursorYPos > 0 )
			iLevel->iGeneralLevelInfo.iTimeLimit+=5;
	}

	if (curKey == SDLK_LEFT){
		if (iCursorYPos > 1 )
		{
			iLevel->iGeneralLevelInfo.iEnemies[iCursorYPos-2]--;
			if (iLevel->iGeneralLevelInfo.iEnemies[iCursorYPos-2]<0)
				iLevel->iGeneralLevelInfo.iEnemies[iCursorYPos-2]=0;
		}
		else
		if (iCursorYPos > 0 )
		{
			iLevel->iGeneralLevelInfo.iTimeLimit-=5;
			if (iLevel->iGeneralLevelInfo.iTimeLimit<0)
				iLevel->iGeneralLevelInfo.iTimeLimit=0;
		}
	}

	if (iCommentMode)
	{
		int len = strlen( iLevel->iGeneralLevelInfo.iComment );

		if (curKey!=SDLK_RETURN && 
			curKey!=SDLK_ESCAPE && 
			curKey!=SDLK_BACKSPACE) 
		{
			int  unic = key.unicode;

			if (len < KCommentMaxLength-1 && unic < 255 && unic > 0)
			{
				iLevel->iGeneralLevelInfo.iComment[ len++ ] = toupper(unic);
				iLevel->iGeneralLevelInfo.iComment[ len   ] = 0;
			}
		}

		if (curKey==SDLK_RETURN || curKey==SDLK_ESCAPE)
			iCommentMode = false;

		if(curKey == SDLK_BACKSPACE && len > 0)
		{
			iLevel->iGeneralLevelInfo.iComment[ len - 1 ]=0;
		}

		iCursorXPos = len;
	} 
	else 
	{
		if (curKey == SDLK_DOWN)
		if (iCursorYPos < KItems-1) 
			iCursorYPos++; 

		if (curKey == SDLK_UP)
		if (iCursorYPos > 0) 
			iCursorYPos--; 

		if (curKey == SDLK_RETURN && iCursorYPos == 0)
		{
			iCommentMode = true;
			iLevel->iGeneralLevelInfo.iComment[0]=0;
			iCursorXPos = 0;
		}

	}
}

void CStateEditGeneralData::HandleMouse(CEventHandler* aEventHandler,CEditorDrawer* aDrawer)
{
}
