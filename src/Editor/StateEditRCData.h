#ifndef __STATEEDITRCDATA_H
#define __STATEEDITRCDATA_H

#include "EditorState.h"
#include "editorapp.h"

class CStateEditRCData : public CEditorState
{
public:
	CStateEditRCData(CStateHandler* aStateHandler,CEditorBrush* aBrush,CEditableLevel* aLevel);
	~CStateEditRCData(void);
public:
	void Draw(CEditorDrawer* aDrawer);
	void HandleKeys(CEventHandler* aEventHandler,CEditorDrawer* aDrawer);
	void HandleMouse(CEventHandler* aEventHandler,CEditorDrawer* aDrawer);
	void SetMode(int aEditMode);
private:
	TEditRCDataMode iCurrentMode;
	int iCursorXPos;
	int iCursorYPos;
};

#endif
