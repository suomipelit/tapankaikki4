#ifndef __EDITORSTATE_H
#define __EDITORSTATE_H

class CEditorDrawer;
class CEventHandler;
class CEditorBrush;
class CEditableLevel;
class CStateHandler;

class CEditorState
{
public:
	virtual void HandleKeys(CEventHandler* aEventHandler,CEditorDrawer* aDrawer)=0;
	virtual void HandleMouse(CEventHandler* aEventHandler,CEditorDrawer* aDrawer)=0;
	virtual void Draw(CEditorDrawer* aDrawer)=0;
	virtual void SetMode(int)=0; // Sub mode
	CEditorState(CEditorBrush* aBrush, CEditableLevel* aLevel, CStateHandler* aStateHandler);
	CEditorBrush* Brush();
	CEditableLevel* Level();
	CStateHandler* StateHandler();
protected:
	int iCurrentMode; // Sub mode
	CEditorBrush* iBrush;
	CEditableLevel* iLevel;
	CStateHandler* iStateHandler;
};

#endif
