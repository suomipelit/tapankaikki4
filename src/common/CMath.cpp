#include <math.h>
#include "CMath.h"

#ifndef NULL
#define NULL (0)
#endif

CMath* CMath::iStatic=NULL;

CMath::CMath() 
{
	int a;
	for ( a = 0;a < 360;a ++ ) 
	{
		iSine[a]=(float)sin( (float) a / 360*2*pi );
		iCosine[a]=(float)cos( (float) a / 360*2*pi );
	}
}

const CMath* CMath::Static()
{
	if (iStatic==NULL)
		iStatic=new CMath;
	return iStatic;
}

void CMath::Destroy()
{
	delete iStatic;
	iStatic=NULL;
}

