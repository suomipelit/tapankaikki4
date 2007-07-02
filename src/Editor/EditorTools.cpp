#include "EditorTools.h"
/*
#include "EditorSelection.h"
#include "EditorMagicSelection.h"
#include "EditorBrush.h"
*/
CEditorTools::CEditorTools(CEditorBrush* aBrush,CEditorSelection* aSelection,CEditorMagicSelection* aMagicSelection)
{
	iBrush = aBrush;
	iSelection = aSelection;
	iMagicSelection = aMagicSelection;
}

CEditorTools::~CEditorTools(void)
{
}

CEditorBrush* CEditorTools::Brush() {return iBrush;}
CEditorSelection* CEditorTools::Selection() {return iSelection;}
CEditorMagicSelection* CEditorTools::MagicSelection() {return iMagicSelection;}
