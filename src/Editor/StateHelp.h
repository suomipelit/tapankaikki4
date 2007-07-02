#ifndef __STATEHELP_H_
#define __STATEHELP_H_

#include "EditorState.h"
#include "editorapp.h"

class CStateHelp : public CEditorState
{
public:
	CStateHelp(CStateHandler* aStateHandler,CEditorBrush* aBrush,CEditableLevel* aLevel);
	~CStateHelp(void);
public:
	void Draw(CEditorDrawer* aDrawer);
	void HandleKeys(CEventHandler* aEventHandler,CEditorDrawer* aDrawer);
	void HandleMouse(CEventHandler* aEventHandler,CEditorDrawer* aDrawer);
	void SetMode(int) { };
};

#endif
