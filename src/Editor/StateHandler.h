#ifndef __STATEHANDLER_H_
#define __STATEHANDLER_H_

#include "editorapp.h" // Editmodes found here

class CEditorState;
class CEditorDrawer;
class CEditableLevel;
class CMath;
class CEditorTools;
class CEditorClipBoard;

class CStateHandler
{
public:
	CStateHandler(CEditorTools* aTools,CEditableLevel* aLevel,CEditorClipBoard* aClipBoard);
	~CStateHandler(void);
	void SetState(TEditMode aEditMode);
	void SetPrevious();
	CEditorState* CurrentState();

protected: 
	CEditorState* iStates[EEditModes];
	TEditMode iCurrentEditMode;	
	TEditMode iPreviousEditMode;
};

#endif
