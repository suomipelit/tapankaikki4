#ifndef __CLIGHTMAP_H_
#define __CLIGHTMAP_H_

#include "defines.h"

class CLightMap
{
public:
	EXPORT CLightMap();
	EXPORT ~CLightMap();
	EXPORT int Size() const;
	unsigned char *iData;
	EXPORT void Make(int aSize);
private:
	int iSize;
};

#endif

