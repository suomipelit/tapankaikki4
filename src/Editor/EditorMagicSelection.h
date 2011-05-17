#ifndef __EDITORMAGICSELECTION_H_
#define __EDITORMAGICSELECTION_H_

class CEditableLevel;
class CEditorSelection;
struct TBlock;

class CEditorMagicSelection
{
public:
	CEditorMagicSelection(CEditableLevel* aLevel,CEditorSelection* aSelection);
	~CEditorMagicSelection(void);
	void SelectArea(int aX,int aY);
	void DeSelectArea(int aX,int aY);
private:
	//void Flood(int aX,int aY,TBlock& aBlock);
	void Flood(int aX,int aY,TBlock& aBlock,bool aHoriz,bool aAdd);
	bool BlockChk(int aX,int aY,TBlock& aBlock,bool aAdd);
	CEditableLevel* iLevel;
	CEditorSelection* iSelection;
};

#endif
