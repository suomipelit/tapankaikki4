#include "CGraphicsGenerator.h"
#include "CGraphicsBuffer.h"
#include "CLevel.h"
#include "CPalette.h"
#include "CLightTable.h"
#include "CMath.h"

void CGraphicsGenerator::ZoomWall(CGraphicsBuffer& aSrc, CGraphicsBuffer& aDst, int aLevel)
{
	const int cBlocksW = aSrc.Width() / KBlockSpriteSize;
	const int cBlocksH = aSrc.Height() / KBlockSpriteSize;
	int a,b,xsize = cBlocksW * (20 + aLevel+1),ysize = cBlocksH * (20 + aLevel+1);
	
	aDst.Resize(xsize,ysize);
	for (a = 0; a < cBlocksH ; a++)
		for (b = 0; b < cBlocksW; b++)
			ZoomBlock(aSrc, aDst,b,a,aLevel+1);
}

void CGraphicsGenerator::ZoomBlock(CGraphicsBuffer& aSrc, CGraphicsBuffer& aDst, int aX, int aY, int aAdd)
{
	int a,b,c,d,size;
	
	size = KBlockSpriteSize+aAdd;
	
	for (a = 0, c = 0; a < size; a++)
	{
		for (b = 0, d = 0; b < size; b++)
		{	
			aDst[(a + (aY*size)) * (aDst.Width()) + b + aX*size ] = aSrc[(c+(aY*KBlockSpriteSize)) * aSrc.Width() + d + aX * KBlockSpriteSize];
			if ((b+1) % (size / (aAdd + 1)+1) != 0)
				d++; 
			if (d >= KBlockSpriteSize) 
				d = (KBlockSpriteSize-1);
		}					    
		if ((a+1) % (size / (aAdd + 1)+1) != 0) 
			c++;
		if (c >= KBlockSpriteSize) 
			c = (KBlockSpriteSize-1);
	}
}

void CGraphicsGenerator::MakeFloors(CPalette& aPal, CGraphicsBuffer& aSrc, CGraphicsBuffer& aDst)
{
	int a;
    float re,gr,bl,h,s,lum;
	char col;

	aDst.Resize(aSrc.Width(),aSrc.Height());
	
    for(a=0;a<aSrc.Width()*aSrc.Height(); a++) 
	{
        re=aPal.Color(aSrc[a]).r;
        gr=aPal.Color(aSrc[a]).g;
        bl=aPal.Color(aSrc[a]).b;     
		CPalette::rgb2hsl(re,gr,bl,h,s,lum);
        lum/=3;
        s/=3;
        CPalette::hsl2rgb(h,s,lum,re,gr,bl);
        col=CLightTable::ClosestInPalette((int)re,(int)gr,(int)bl,aPal);
		aDst[a]=col;
	}
}

void CGraphicsGenerator::RotateSprites(CGraphicsBuffer& aBuffer, int aSpriteWidth,int aSpriteHeight,int aAngles,CPalette& pal)
{
	float xx,yy,xx2,yy2;
	int b,c,a,ax,ay;
	char col;
	const int xSprs = aAngles;
	const int ySprs = (aBuffer.Height()/aSpriteHeight);
	float* sini;
	float* cosi;

	sini=(float*)malloc(aAngles*sizeof(float));
	cosi=(float*)malloc(aAngles*sizeof(float));

	for (a=0;a<aAngles;a++)
	{
		sini[a]=sin((float)(a)/aAngles*pi*2);
		cosi[a]=cos((float)(a)/aAngles*pi*2);
	}

	// just to make sure???
	sini[0]=0;
	cosi[0]=1;
	sini[aAngles/4]=1;
	cosi[aAngles/4]=0;
	sini[aAngles/2]=0;
	cosi[aAngles/2]=-1;
	sini[aAngles/4*3]=-1;
	cosi[aAngles/4*3]=0;

	aBuffer.Resize( xSprs*(aSpriteWidth+1), ySprs*(aSpriteHeight+1) );

	for (int column = 1; column < xSprs; column ++)
	{
		for (int row = 0; row < ySprs; row++)
		{
			xx2=-(aSpriteWidth/2+1)*cosi[(column+aAngles/4)%aAngles];
			yy2=-(aSpriteHeight/2+1)*sini[(column+aAngles/4)%aAngles];

			for (a=0,c=0;a<aSpriteHeight+1;a++) 
			{
				xx=-(aSpriteWidth/2+1)*cosi[column];
				yy=-(aSpriteHeight/2+1)*sini[column];

				xx2+=cosi[(column+aAngles/4)%aAngles];
				yy2+=sini[(column+aAngles/4)%aAngles];

				for (b=0;b<aSpriteWidth+1;b++,c++) 
				{
					col=0;
					xx+=cosi[column];
					yy+=sini[column];
					   
					ax=int(xx+xx2+(aSpriteWidth/2+1));
					ay=int((row*(aSpriteHeight+1))+yy+yy2+(aSpriteHeight/2+1));
					   
					if (ay>(row*aSpriteHeight) && ax>0 && 
						ay<(row*aSpriteHeight)+aSpriteHeight && 
						ax<aSpriteWidth)
					{
						col=aBuffer[ay*aBuffer.Width()+ax];
					}

					aBuffer[(a+row*(aSpriteHeight+1))*aBuffer.Width()+b+column*(aSpriteWidth+1)]=col;
				
				}
			}
		}
	}
	free(sini);
	free(cosi);
};
