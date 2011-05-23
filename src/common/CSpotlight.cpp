#include "CSpotlight.h"
#include "error.h"

CSpotLight::CSpotLight(float aX,float aY, int aSize)
{
	iLocation.Set(aX,aY);
	iSize=aSize;	
}

CSpotLight::CSpotLight(FILE *fptr)
{
	ASSERT(fptr);
	ReadFromFile(fptr);
}

void CSpotLight::WriteToFile(FILE *fptr) const
{
	ASSERT(fptr);
	int a;
    a=(int)iLocation.X();fwrite(&a,sizeof(int),1,fptr);
	a=(int)iLocation.Y();fwrite(&a,sizeof(int),1,fptr);
	fwrite(&iSize,sizeof(int),1,fptr);
}

void CSpotLight::ReadFromFile(FILE *fptr)
{
	ASSERT(fptr);
	int a;
	int ret;

    ret = fread(&a,sizeof(int),1,fptr);
	if ( ret != 1 )
		throw CFailureException("CSpotLight::ReadFromFile: file read error (1).");
	iLocation.SetX((float)a);
	ret = fread(&a,sizeof(int),1,fptr);
	if ( ret != 1)
		throw CFailureException("CSpotLight::ReadFromFile: file read error (2).");
	iLocation.SetY((float)a);
	ret = fread(&iSize,sizeof(int),1,fptr);
	if ( ret != 1)
		throw CFailureException("CSpotLight::ReadFromFile: file read error (3).");
}
