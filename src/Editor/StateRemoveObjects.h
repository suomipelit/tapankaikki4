#ifndef __STATEREMOVEOBJECTS_H
#define __STATEREMOVEOBJECTS_H

#include "EditorState.h"

class CEditorDrawer;
class CEventHandler;
class CEditableLevel;
class CMath;

class CStateRemoveObjects : public CEditorState
{
public:
	CStateRemoveObjects(CStateHandler* aStateHandler,CEditorBrush* aBrush,CEditableLevel* aLevel);
	~CStateRemoveObjects(void);
	void Draw(CEditorDrawer* aDrawer);
	void HandleKeys(CEventHandler* aEventHandler,CEditorDrawer* aDrawer);
	void HandleMouse(CEventHandler* aEventHandler,CEditorDrawer* aDrawer);
	void SetMode(int){ };
private:

	int iMouseX;
	int iMouseY;
};

#endif
