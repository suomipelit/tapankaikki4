#ifndef __CGRAPHICSGENERATOR_H_
#define __CGRAPHICSGENERATOR_H_
#include "defines.h"

class CGraphicsBuffer;
class CPalette;

class CGraphicsGenerator
{
public:
	EXPORT static void ZoomWall(CGraphicsBuffer& aSrc, CGraphicsBuffer& aDst, int aLevel);
	EXPORT static void ZoomBlock(CGraphicsBuffer& aSrc, CGraphicsBuffer& aDst, int aX, int aY, int aAdd);
	EXPORT static void MakeFloors(CPalette& aPal, CGraphicsBuffer& aSrc, CGraphicsBuffer& aDst);
	EXPORT static void RotateSprites(CGraphicsBuffer& aBuffer, int aSpriteWidth,int aSpriteHeight,int aAngles,CPalette& pal);

};

#endif

