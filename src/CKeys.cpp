#include "CKeys.h"


CKeys& CKeys::operator =(const CKeys& k)
{
	for (int a=0;a<EKeyAmount;a++)
		keys[a]=k.keys[a];

	return *this;
}

CKeys::CKeys()
{
	for (int a=0;a<EKeyAmount;a++)
		keys[a]=0;
}

