#include "EditorMagicSelection.h"
#include "../common/CEditableLevel.h"
#include "EditorSelection.h"

CEditorMagicSelection::CEditorMagicSelection(CEditableLevel* aLevel,CEditorSelection* aSelection)
{
	iLevel = aLevel;
	iSelection = aSelection;
}

CEditorMagicSelection::~CEditorMagicSelection(void)
{
}

/* Flood Version 2.0 Unski recursive
*/

void CEditorMagicSelection::SelectArea(int aX,int aY)
{
	if (! iLevel->InLevel(aX,aY)) return;
	Flood(aX,aY,iLevel->LevelData(aX,aY),true,true);
}

void CEditorMagicSelection::DeSelectArea(int aX,int aY)
{
	if (! iLevel->InLevel(aX,aY)) return;
	Flood(aX,aY,iLevel->LevelData(aX,aY),true,false);
}

bool CEditorMagicSelection::BlockChk(int aX,int aY,TBlock& aBlock,bool aAdd)
{
	if (aY >= 0 && aY < iLevel->Height() &&
		aX >= 0 && aX < iLevel->Width() &&
		iLevel->LevelData(aX,aY).iType==aBlock.iType &&
		iLevel->LevelData(aX,aY).iNumber==aBlock.iNumber &&
		iSelection->Includes(aX,aY)!=aAdd) 
		return true;
	else 
		return false;
}

void CEditorMagicSelection::Flood(int aX,int aY,TBlock& aBlock,bool aHoriz,bool aAdd)
{
	int min,max;
	if (aHoriz)
	{
		min=0;
		while (BlockChk(aX-min,aY,aBlock,aAdd))
		if (aAdd)
			iSelection->Add(aX-min++,aY);
		else
			iSelection->Remove(aX-min++,aY);
		max=1;
		while (BlockChk(aX+max,aY,aBlock,aAdd))
		if (aAdd)
			iSelection->Add(aX+max++,aY);
		else
			iSelection->Remove(aX+max++,aY);
		for (int a=0;a<min;a++)
		{
			if (BlockChk(aX-a,aY-1,aBlock,aAdd))
				Flood(aX-a,aY-1,aBlock,false,aAdd);
			if (BlockChk(aX-a,aY+1,aBlock,aAdd))
				Flood(aX-a,aY+1,aBlock,false,aAdd);
		}
		for (int a=1;a<max;a++)
		{
			if (BlockChk(aX+a,aY-1,aBlock,aAdd))
				Flood(aX+a,aY-1,aBlock,false,aAdd);
			if (BlockChk(aX+a,aY+1,aBlock,aAdd))
				Flood(aX+a,aY+1,aBlock,false,aAdd);
		}
	}
	else
	{
		min=0;
		while (BlockChk(aX,aY-min,aBlock,aAdd))
		if (aAdd)		
			iSelection->Add(aX,aY-min++);
		else
			iSelection->Remove(aX,aY-min++);
		max=1;
		while (BlockChk(aX,aY+max,aBlock,aAdd))
		if (aAdd)		
			iSelection->Add(aX,aY+max++);
		else
			iSelection->Remove(aX,aY+max++);
		for (int a=0;a<min;a++)
		{
			if (BlockChk(aX-1,aY-a,aBlock,aAdd))
				Flood(aX-1,aY-a,aBlock,true,aAdd);
			if (BlockChk(aX+1,aY-a,aBlock,aAdd))
				Flood(aX+1,aY-a,aBlock,true,aAdd);
		}
		for (int a=1;a<max;a++)
		{
			if (BlockChk(aX-1,aY+a,aBlock,aAdd))
				Flood(aX-1,aY+a,aBlock,true,aAdd);
			if (BlockChk(aX+1,aY+a,aBlock,aAdd))
				Flood(aX+1,aY+a,aBlock,true,aAdd);
		}
	}
}
/*
*	Flood version 1.0 recursive

void CEditorMagicSelection::Flood(int aX,int aY,TBlock& aBlock)
{
	int XOffs=0;
	int YOffs=-1;
	int a;

	for (a=0;a<4;a++)
	{
		switch (a) {
			case 1: YOffs=0;XOffs=1;break;
			case 2: YOffs=1;XOffs=0;break;
			case 3: YOffs=0;XOffs=-1;break;
		}
		if (aY+YOffs >= 0 && aY+YOffs < iLevel->Height() &&
			aX+XOffs >= 0 && aX+XOffs < iLevel->Width() &&
			iLevel->LevelData(aX+XOffs,aY+YOffs).iType==aBlock.iType &&
			iLevel->LevelData(aX+XOffs,aY+YOffs).iNumber==aBlock.iNumber &&
			! iSelection->Includes(aX+XOffs,aY+YOffs))
			{
				iSelection->Add(aX+XOffs,aY+YOffs);
				Flood(aX+XOffs,aY+YOffs,aBlock);
			}
	}
}

void CEditorMagicSelection::SelectArea(int aX,int aY)
{
	if (! iLevel->InLevel(aX,aY)) return;
	iSelection->Add(aX,aY);
	Flood(aX,aY,iLevel->LevelData(aX,aY));
}
*/
