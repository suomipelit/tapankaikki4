#include "EditorClipBoardBlock.h"

CEditorClipBoardBlock::CEditorClipBoardBlock(int aX,int aY,TBlockType aType,int aNumber,int aHeight)
{
	iX = aX;
	iY = aY;
	iType = aType;
	iNumber = aNumber;
	iHeight = aHeight;
}

CEditorClipBoardBlock::~CEditorClipBoardBlock(void)
{
}

TBlockType CEditorClipBoardBlock::Type(void)
{
	return iType;
}

int CEditorClipBoardBlock::X(void)
{
	return iX;
}

int CEditorClipBoardBlock::Y(void)
{
	return iY;
}

int CEditorClipBoardBlock::Number(void)
{
	return iNumber;
}

int CEditorClipBoardBlock::Height(void)
{
	return iHeight;
}
