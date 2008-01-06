#ifndef __STATEEDITGENERALDATA_H
#define __STATEEDITGENERALDATA_H

#include "EditorState.h"
#include "editorapp.h"

class CStateEditGeneralData : public CEditorState
{
public:
	CStateEditGeneralData(CStateHandler* aStateHandler,CEditorBrush* aBrush,CEditableLevel* aLevel);
	~CStateEditGeneralData(void);
public:
	void Draw(CEditorDrawer* aDrawer);
	void HandleKeys(CEventHandler* aEventHandler,CEditorDrawer* aDrawer);
	void HandleMouse(CEventHandler* aEventHandler,CEditorDrawer* aDrawer);
	void SetMode(int) { };
private:
	bool iCommentMode;
	int iCursorXPos;
	int iCursorYPos;
};

#endif
