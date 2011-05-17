#ifndef __STATEGETBLOCK_H_
#define __STATEGETBLOCK_H_

#include "EditorState.h"
#include "../common/CEditableLevel.h"

class CStateGetBlock : 	public CEditorState
{
public:
	CStateGetBlock(CStateHandler* aStateHandler,CEditorBrush* aBrush,CEditableLevel* aLevel);
	~CStateGetBlock(void);
public:
	void Draw(CEditorDrawer* aDrawer);
	void HandleKeys(CEventHandler* aEventHandler,CEditorDrawer* aDrawer);
	void HandleMouse(CEventHandler* aEventHandler,CEditorDrawer* aDrawer);
	void SetMode(int) { };
protected:
	TBlockType iType;
	int iMouseX;
	int iMouseY;
};

#endif
