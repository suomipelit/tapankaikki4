#include "CLightMap.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>

void CLightMap::Make(int aSize) 
{
	int a, b, pow, iOffset, dist, hsize;
	hsize=aSize/2;
	iData=(unsigned char *) malloc(aSize*aSize);
	iSize = aSize;
	memset(iData, 0, aSize*aSize);
	for (a=0;a<aSize;a++) 
		for (b=0;b<aSize;b++) 
		{
			dist=(int)sqrt(float((a-hsize) *(a-hsize)+(b-hsize) *(b-hsize)));
			iOffset=(a*aSize) +b;
			pow=16-(int)(dist*(float) (16/(float) (aSize/2)));
			if (pow>15) pow=15;
			if (pow<0) pow=0;
			iData[iOffset]=(char) pow;
		}
}

CLightMap::CLightMap(): iData(NULL), iSize(0)
{
}

CLightMap::~CLightMap()
{
	free(iData);
}

int CLightMap::Size() const
{
	return iSize;
}
