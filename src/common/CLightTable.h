#ifndef __CLIGHTTABLE_H
#define __CLIGHTTABLE_H 

#include "defines.h"

class CPalette;

class CLightTable
{
public:
	EXPORT static unsigned char ClosestInPalette(int aRed,int aGreen,int aBlue,CPalette& aPalette);

	unsigned char iData[256][16];
private:
	void Make(int R,int G,int B,CPalette& aPalette);
	void Make2(CPalette& aPalette);

	friend class CColorTable;
};

#endif

