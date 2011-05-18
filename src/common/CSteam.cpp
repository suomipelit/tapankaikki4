#include "CSteam.h"
#include "CMath.h"
#include "error.h"

CSteam::CSteam(float aX,float aY,int aAngle,int aSpeed)
{
	iX=aX;
	iY=aY;
	iAngle=aAngle;
	iSpeed=aSpeed;	
}

CSteam::CSteam(FILE *fptr, int aVersion)
{
	ASSERT(fptr);
	ReadFromFile(fptr, aVersion);
}

void CSteam::WriteToFile(FILE *fptr)
{
	ASSERT(fptr);
	int a;
    a=(int)iX;fwrite(&a,sizeof(int),1,fptr);
	a=(int)iY;fwrite(&a,sizeof(int),1,fptr);
	fwrite(&iAngle,sizeof(int),1,fptr);
	fwrite(&iSpeed,sizeof(int),1,fptr);
}

void CSteam::ReadFromFile(FILE *fptr, int aVersion)
{
	ASSERT(fptr);
	int a;

    fread(&a,sizeof(int),1,fptr);iX=(float)a;
	fread(&a,sizeof(int),1,fptr);iY=(float)a;
	fread(&iAngle,sizeof(int),1,fptr);

	if ( aVersion < 9 ) // angles changed to real one
	{
		iAngle = -iAngle + 90;
		ANGLEFIX( iAngle );
	}

	fread(&iSpeed,sizeof(int),1,fptr);
}

