#include "EditorSelection.h"
#include <stdlib.h>
#include <string>

CEditorSelection::CEditorSelection(void)
{
	iWidth=35; // näillä luvuilla arvioidaan kentän keskim. kokoa
	iHeight=35;
	iSelection = (char *)malloc(iWidth*iHeight);
	Clear();
}

CEditorSelection::~CEditorSelection(void)
{
	free(iSelection);
}

void CEditorSelection::Clear()
{
    memset(iSelection,0,iWidth*iHeight);
}

bool CEditorSelection::Includes(int aX1,int aY1)
{
	if (aX1>=0 && aY1>=0 && aX1<iWidth && aY1<iHeight && iSelection[aY1*iWidth+aX1]==1)
		return true;
	else
		return false;
}
/*
bool CEditorSelection::Empty()
{
	if (sizeof(iSelection)>0)
		return true;
	else 
		return false;
}


int CEditorSelection::Size()
{
	return iWidth*iHeight;
}
*/

void CEditorSelection::Expand(int aW,int aH)
{
	if (iWidth>aW && iHeight>aH) // no need to expand
		return;
	char* oldSel;
	int oldW,oldH;
	oldH=iHeight;
	oldW=iWidth;
	oldSel = (char *)malloc(oldW*oldH);
	memcpy(oldSel,iSelection,oldW*oldH);
	if (iWidth<=aW) iWidth=aW+1; // expand the iSelection
	if (iHeight<=aH) iHeight=aH+1;
	free(iSelection); // free and reallocate
	iSelection = (char *)malloc(iWidth*iHeight);
	memset(iSelection,0,iWidth*iHeight); // clear
	for (int i=0;i < oldH;i++)// copy old to new
		memcpy(iSelection+i*iWidth,oldSel+i*oldW,oldW);
	free(oldSel);
}

void CEditorSelection::Add(int aX,int aY)
{
	Expand(aX,aY); // I know it's slow to check it everytime, wise man
	if (aX>=0 && aY>=0) // would make the selection same size as level on load
		iSelection[aY*iWidth+aX]=1;
}

void CEditorSelection::Remove(int aX,int aY)
{
	if (aX>=0 && aY>=0 && aX<iWidth && aY<iHeight)
        iSelection[aY*iWidth+aX]=0;	
}

void CEditorSelection::RemoveRect(int aX1,int aY1,int aX2,int aY2)
{
	for (int a=aY1;a <= aY2;a++)
		for (int b=aX1;b <= aX2;b++)
			Remove(b,a);
}

void CEditorSelection::AddRect(int aX1,int aY1,int aX2,int aY2)
{
	Expand(aX2,aY2); // This is to make it faster -> allocate in advance
	for (int a=aY1;a <= aY2;a++)
		for (int b=aX1;b <= aX2;b++)
			Add(b,a);
}
