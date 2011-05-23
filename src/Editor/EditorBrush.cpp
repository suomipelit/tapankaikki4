#include "EditorBrush.h"
#include "EditorGraphics.h"
#include "EditorSelection.h"
#include "../common/CEditableLevel.h"
#include "../common/error.h"

CEditorBrush::CEditorBrush(CEditableLevel* aLevel,CEditorSelection* aLevelSelection):iLevel(aLevel),iLevelSelection(aLevelSelection)
{
}

CEditorBrush::~CEditorBrush(void)
{
}

void CEditorBrush::SetRightBlock(TBlockType aType,int aNumber)
{
	iRightBlock.iType=aType;
	iRightBlock.iNumber=aNumber;
	if (aType==EBlockTypeFloor)
		iRightBlock.iHeight=0;
	else 
	{
		ASSERT( aNumber < KBlockHeightTableSize );
		iRightBlock.iHeight=KBlockHeights[aNumber];
	}
	iRightBlock.iCastDarkness=0;
	iRightBlock.iShadow=0;
}
void CEditorBrush::SetLeftBlock(TBlockType aType,int aNumber)
{
	iLeftBlock.iType=aType;
	iLeftBlock.iNumber=aNumber;
	if (aType==EBlockTypeFloor)
		iLeftBlock.iHeight=0;
	else
	{
		ASSERT( aNumber < KBlockHeightTableSize );
		iLeftBlock.iHeight=KBlockHeights[aNumber];
	}
	iLeftBlock.iCastDarkness=0;
	iLeftBlock.iShadow=0;
}

void CEditorBrush::DrawBlock(int aX,int aY,TBlock& aBlock)
{
	if (iLevelSelection->Includes(aX,aY))
	{
		for (int a=0;a < iLevelSelection->Height();a++)
			for (int b=0;b < iLevelSelection->Width();b++)
				if (iLevelSelection->Includes(b,a))
					iLevel->LevelData(b,a) = aBlock;
	} 
	else 
	{
		if (iLevel->InLevel(aX,aY))
			iLevel->LevelData(aX,aY) = aBlock;
		else
			iLevel->OutBlock() = aBlock;
	}
}

void CEditorBrush::DrawRightBlock(int aX,int aY)
{
	DrawBlock(aX,aY,iRightBlock);
}

void CEditorBrush::DrawLeftBlock(int aX,int aY)
{
	DrawBlock(aX,aY,iLeftBlock);
}

void CEditorBrush::ModifyHeight(int aX,int aY,int aAdd)
{
	if (iLevel->InLevel(aX,aY)){
	if (iLevelSelection->Includes(aX,aY))
	{
		for (int a=0;a < iLevelSelection->Height();a++)
		for (int b=0;b < iLevelSelection->Width();b++)
		if (iLevelSelection->Includes(b,a))
		if (iLevel->LevelData(b,a).iType==EBlockTypeWall)
		{
			iLevel->LevelData(b,a).iHeight+=aAdd;
			if (iLevel->LevelData(b,a).iHeight < 0)
				iLevel->LevelData(b,a).iHeight = 0;
			if (iLevel->LevelData(b,a).iHeight > KWallSprites-1)
				iLevel->LevelData(b,a).iHeight = KWallSprites-1;
		}
	} 
	else 
	if (iLevel->LevelData(aX,aY).iType == EBlockTypeWall)
	{
		iLevel->LevelData(aX,aY).iHeight+=aAdd;
		if (iLevel->LevelData(aX,aY).iHeight < 0)
			iLevel->LevelData(aX,aY).iHeight = 0;
		if (iLevel->LevelData(aX,aY).iHeight > KWallSprites-1)
			iLevel->LevelData(aX,aY).iHeight = KWallSprites-1;
	}
	}
	else
		if (iLevel->OutBlock().iType == EBlockTypeWall)
		{
			iLevel->OutBlock().iHeight+=aAdd;
			if (iLevel->OutBlock().iHeight<0) iLevel->OutBlock().iHeight=0;
			if (iLevel->OutBlock().iHeight>KWallSprites-1) iLevel->OutBlock().iHeight=KWallSprites-1;
		}
}
