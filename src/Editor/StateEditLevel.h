#ifndef __STATEEDITLEVEL_H__
#define __STATEEDITLEVEL_H__
#include "EditorState.h"

class CEditorTools;
class CEditorClipBoard;

class CStateEditLevel :	public CEditorState
{
public:
	CStateEditLevel(CStateHandler* aStateHandler,CEditorTools* aTools,CEditableLevel* aLevel, CEditorClipBoard* aClipBoard);
	~CStateEditLevel(void);
public:
	void Draw(CEditorDrawer* aDrawer);
	void HandleKeys(CEventHandler* aEventHandler,CEditorDrawer* aDrawer);
	void HandleMouse(CEventHandler* aEventHandler,CEditorDrawer* aDrawer);
	void SetMode(int) { };
protected:
    CEditorTools* iTools;
	CEditorClipBoard* iClipBoard;
	int iSelectionMode,iSelectionX,iSelectionY;
	bool iPasteClipBoardMode;
	bool iRemoveSelectionMode;
	bool iAddSelectionMode;
	int iMouseX;
	int iMouseY;
};
#endif
