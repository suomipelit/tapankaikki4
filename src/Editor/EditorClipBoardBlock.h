#ifndef __EDITORCLIPBOARDBLOCK_H
#define __EDITORCLIPBOARDBLOCK_H

#include "../common/CEditableLevel.h"

class CEditorClipBoardBlock
{
public:
	CEditorClipBoardBlock(int aX,int aY,TBlockType aType,int aNumber,int aHeight);
	~CEditorClipBoardBlock(void);
	TBlockType Type(void);
	int X(void);
	int Y(void);
	int Number(void);
	int Height(void);
private:
	int iX,iY;
	TBlockType iType;
	int iNumber;
	int iHeight;
};
#endif
