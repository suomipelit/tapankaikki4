#ifndef __STATEEDITHEIGHTS_H
#define __STATEEDITHEIGHTS_H
#include "EditorState.h"

class CEditorTools;

class CStateEditHeights : public CEditorState
{
public:
	CStateEditHeights(CStateHandler* aStateHandler,CEditorTools* aTools,CEditableLevel* aLevel);
	~CStateEditHeights(void);
public:
	void Draw(CEditorDrawer* aDrawer);
	void HandleKeys(CEventHandler* aEventHandler,CEditorDrawer* aDrawer);
	void HandleMouse(CEventHandler* aEventHandler,CEditorDrawer* aDrawer);
	void SetMode(int) { };
protected:
	CEditorTools* iTools;
	int iSelectionMode;
	int iSelectionX,iSelectionY;
	bool iRemoveSelectionMode;
	bool iAddSelectionMode;
	int iMouseX;
	int iMouseY;
};
#endif
