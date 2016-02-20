#ifndef __EDITORSELECTION_H
#define __EDITORSELECTION_H

class CEditorSelection
{
public:
	CEditorSelection(void);
	~CEditorSelection(void);
	void Clear();
	void Add(int aX1,int aY1);
	void Remove(int aX1,int aY1);
	void AddRect(int aX1,int aY1,int aX2,int aY2);
	void RemoveRect(int aX1,int aY1,int aX2,int aY2);
	//int Size();
	//bool Empty();
	bool Includes(int aX1,int aY1);
	int Width() {return iWidth;}
	int Height() {return iHeight;}
	void Expand(int aW,int aH);
protected:
	char* iSelection;
	int iWidth;
	int iHeight;
};

#endif
