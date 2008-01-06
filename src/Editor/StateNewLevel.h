#ifndef __STATENEWLEVEL_H
#define __STATENEWLEVEL_H

#include "EditorState.h"

#define MaxDigits  3

class CStateNewLevel : public CEditorState
{
public:
	CStateNewLevel(CStateHandler* aStateHandler,CEditorBrush* aBrush,CEditableLevel* aLevel);
	~CStateNewLevel(void);
public:
	void Draw(CEditorDrawer* aDrawer);
	void HandleKeys(CEventHandler* aEventHandler,CEditorDrawer* aDrawer);
	void HandleMouse(CEventHandler* aEventHandler,CEditorDrawer* aDrawer);
	void SetMode(int) { };

private:
	char iNewWidth[MaxDigits+1];
	char iNewHeight[MaxDigits+1];
	short iCurKey;
	int iCursorXPos;
	int iCursorYPos;
};

#endif
