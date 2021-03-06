#ifndef __EDITORTOOLS_H
#define __EDITORTOOLS_H

class CEditorBrush;
class CEditorSelection;
class CEditorMagicSelection;

class CEditorTools
{
public:
	CEditorTools(CEditorBrush* aBrush,CEditorSelection* aSelection,CEditorMagicSelection* aMagicSelection);
	~CEditorTools(void);
	CEditorBrush* Brush();
	CEditorSelection* Selection();
	CEditorMagicSelection* MagicSelection();
protected:
	CEditorBrush* iBrush;
	CEditorSelection* iSelection;
	CEditorMagicSelection* iMagicSelection;
};

#endif
