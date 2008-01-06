#ifndef __EDITORCLIPBOARD_H
#define __EDITORCLIPBOARD_H
#include <vector>
#include "EditorClipBoardBlock.h"

class CEditorSelection;
class CEditableLevel;

class CEditorClipBoard
{
public:
	CEditorClipBoard(void);
	~CEditorClipBoard(void);
	void CopySelection(CEditorSelection* aEditorSelection,CEditableLevel* aLevel);
	void Clear();
	void Paste(int aX,int aY, CEditableLevel* aLevel);
	std::vector<CEditorClipBoardBlock>* GetBlocks();
private:
	std::vector<CEditorClipBoardBlock> iBlocks;
};
#endif
