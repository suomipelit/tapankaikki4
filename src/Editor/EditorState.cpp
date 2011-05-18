#include "EditorState.h"

CEditorState::CEditorState(CEditorBrush* aBrush, CEditableLevel* aLevel, CStateHandler* aStateHandler)
{
	iBrush=aBrush;
	iLevel=aLevel;
	iStateHandler=aStateHandler;
}

CEditorBrush* CEditorState::Brush()
{
	return iBrush;
}

CEditableLevel* CEditorState::Level()
{
	return iLevel;
}

CStateHandler* CEditorState::StateHandler()
{
	return iStateHandler;
}
