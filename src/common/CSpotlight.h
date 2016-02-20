#ifndef __CSPOTLIGHT_H
#define __CSPOTLIGHT_H

#include <stdio.h>

#include "defines.h"
#include "CCoord.h"
#include "CLinkedList.h"

class CSpotLight: public CLinkedList<CSpotLight>
{
public:
	EXPORT CSpotLight(float aX,float aY, int aSize);
	EXPORT CSpotLight(FILE *fptr);
	EXPORT void WriteToFile(FILE *fptr) const;
	EXPORT void ReadFromFile(FILE *fptr);
	inline int Size(){ return iSize; };
	inline CCoord<float>& Location(){ return iLocation; };

private:
	CCoord<float> iLocation;
	int iSize;
};

#endif


