#ifndef __CFONTS_H
#define __CFONTS_H

#include "CRect.h"

class CGraphicsBuffer;

class CFonts
{
public:
	enum TVertAlign
	{
		EDrawBelow = 0,
		EDrawInVertMiddle,
		EDrawAbove
	};
	enum THorizAlign
	{
		EDrawToRight = 0,
		EDrawInMiddle,
		EDrawToLeft
	};

public:
	EXPORT CFonts(const char *name, int aOneIsZero);
    EXPORT ~CFonts();
    EXPORT CRect<int> Write(int aX,int aY,const unsigned char *aStr, TVertAlign aVAlign, THorizAlign aHAlign,CGraphicsBuffer* aTarget) const;

    inline CRect<int> Write(int aX,int aY,const char *aStr, TVertAlign aVAlign, THorizAlign aHAlign, CGraphicsBuffer* aTarget) const
	{
		return Write(aX,aY,(const unsigned char*)aStr,aVAlign,aHAlign,aTarget);
	}
    EXPORT int Length(const unsigned char *aStr) const;
    inline int Length(const char *aStr) const
	{
		return Length((const unsigned char*)aStr);
	}  
	inline int Width() const { return iWidth; };
	inline int Height() const { return iHeight; };

private:
 
	int Load(const char *name); //returns 0 if succesful
	int iOneIsZero;
    char *iFontData;
    int iWidth,iHeight;
};

#endif
