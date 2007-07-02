#include "EditorClipBoard.h"
#include "EditorSelection.h"
#include "../common/CEditableLevel.h"

CEditorClipBoard::CEditorClipBoard(void)
{
}

CEditorClipBoard::~CEditorClipBoard(void)
{
}

void CEditorClipBoard::CopySelection(CEditorSelection* aEditorSelection,CEditableLevel* aLevel)
{
	int fx=0,fy=0;
	bool first=true;
	Clear();
	for (int h=0;h < aLevel->Height();h++)
		for (int w=0;w < aLevel->Width();w++)
			if (aEditorSelection->Includes(w,h))
			{
				if (first) {fx=w;fy=h;first=false;}
				CEditorClipBoardBlock tempBlock(w-fx,h-fy,aLevel->LevelData(w,h).iType,aLevel->LevelData(w,h).iNumber,aLevel->LevelData(w,h).iHeight);
				iBlocks.push_back(tempBlock);
			}
}

void CEditorClipBoard::Clear()
{
	iBlocks.clear();
}

void CEditorClipBoard::Paste(int aX,int aY,CEditableLevel* aLevel)
{
	TBlock newBlock;
	for (int a=0;a<iBlocks.size();a++)
	{
		newBlock.iType = iBlocks.at(a).Type();
		newBlock.iNumber = iBlocks.at(a).Number();
		newBlock.iHeight = iBlocks.at(a).Height();
		if (aLevel->InLevel(iBlocks.at(a).X()+aX, iBlocks.at(a).Y()+aY))
			aLevel->LevelData(iBlocks.at(a).X()+aX,iBlocks.at(a).Y()+aY) = newBlock;
	}
}


std::vector<CEditorClipBoardBlock>* CEditorClipBoard::GetBlocks()
{
	return &iBlocks;
}


