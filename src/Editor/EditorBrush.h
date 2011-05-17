#ifndef __EDITORBRUSH_H_
#define __EDITORBRUSH_H_

#include "../common/CEditableLevel.h"

class CEditorSelection;

class CEditorBrush
{
public:
	CEditorBrush(CEditableLevel* aLevel, CEditorSelection* aLevelSelection);
	~CEditorBrush(void);
	void SetRightBlock(TBlockType aType,int aNumber);
	void SetLeftBlock(TBlockType aType,int aNumber);
	TBlock* RightBlock(){return &iRightBlock;};
	TBlock* LeftBlock(){return &iLeftBlock;};
	void DrawRightBlock(int aX,int aY);
	void DrawLeftBlock(int aX,int aY);
	void ModifyHeight(int aX,int aY,int aAdd);
private:
	void DrawBlock(int aX,int aY,TBlock& aBlock);
	TBlock iRightBlock;
	TBlock iLeftBlock;
	CEditorSelection* iLevelSelection;
	CEditableLevel* iLevel;
};

#endif
