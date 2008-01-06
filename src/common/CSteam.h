#ifndef __CSTEAM_H
#define __CSTEAM_H

#include <stdio.h>

#include "defines.h"
#include "CCoord.h"
#include "CLinkedList.h"

class CSteam: public CCoord<float>, public CLinkedList<CSteam>
{
public:
	EXPORT CSteam(float aX,float aY,int aAngle,int aSpeed);
	EXPORT CSteam(FILE *fptr, int aVersion);
	EXPORT void WriteToFile(FILE *fptr);
	EXPORT void ReadFromFile(FILE *fptr, int aVersion);

	inline int Angle(){ return iAngle;};
	inline int Speed(){ return iSpeed;};
private: 

	int iAngle;
	int iSpeed;
};

#endif

