#include <math.h> 
#include <stdlib.h> 
#include <string.h> 

#include "CGraphicsFader.h"
#include "common/CMath.h"
#include "CGameGraphicsInterface.h"
#include "timer.h"
#include "common/CGraphicsBuffer.h"
#include "common/CGraphicsDevice.h"

CGraphicsFader::CGraphicsFader(CGameGraphicsInterface* aGGI): iGGI(aGGI)
{
	int a;

	for (a=0;a<256;a++) 
		iSine[a]=(int)(255*(sin((float)a/256*pi*2)));
}

void CGraphicsFader::DrawPhase( int phase,const CGraphicsBuffer* image,CGraphicsBuffer *aDest ) 
{
	const unsigned char *srcptr;
	unsigned char *dstptr;
	int x, y, yfix, xfix, l=(phase*30)>>8;
	int targetWidth=aDest->Width(),targetHeight=aDest->Height();
	int srcWidth=image->Width();

	ASSERT(phase>=0&&phase<256);
	ASSERT(image);
	ASSERT(aDest);

	srcptr=image->Ptr();
	dstptr=aDest->Ptr();

	for (y=0;y<aDest->Height();y++) 
	{
		yfix=(l*iSine[(y+phase) &0xff])>>8;
		if (yfix+y<0||yfix+y>=targetHeight) 
			memset(dstptr, 0, targetWidth);
		else 
		{
			yfix*=srcWidth;
			for (x=0; x < targetWidth; x++) 
			{
				xfix=x+((l*iSine[(x+phase)&0xff])>>8);
				if (xfix>0 && xfix < targetWidth) 
					dstptr[x]=srcptr[xfix+yfix];
				else dstptr[x]=0;
			}
		}
		srcptr+=image->Width();
		dstptr+=aDest->Width();
	}
}

void CGraphicsFader::FadeIn(const CGraphicsBuffer* image,CGraphicsBuffer *aDest, const CPalette* pal, int StartTime, int LastingTime) 
{
	int time;
	ASSERT(image);
	ASSERT(aDest);
	ASSERT(pal);
	
	time=TimerCounter;

	if (time>=StartTime+LastingTime)
	{
		iGGI->GD()->SetPalette(*pal,256);
		return;
	}

	if (time>=StartTime)
	{
		iGGI->GD()->SetPalette(*pal,max( (time-StartTime)*256/LastingTime,0));
		DrawPhase(255-(time-StartTime)*256/LastingTime, image,aDest );
	}
}

void CGraphicsFader::FadeOut(const CGraphicsBuffer* image,CGraphicsBuffer *aDest, const CPalette* pal, int StartTime, int LastingTime) 
{
	int time;
	ASSERT(image);
	ASSERT(aDest);
	ASSERT(pal);
	time=TimerCounter;

	if (time>=StartTime+LastingTime)
	{
		iGGI->GD()->SetPalette(*pal,0);
		return;
	}

	if (time>=StartTime)
	{
		DrawPhase((time-StartTime)*256/LastingTime,image, aDest);
		iGGI->GD()->SetPalette(*pal,max( 255 - (time-StartTime)*256/LastingTime,0));
	}
}

void CGraphicsFader::DrawZoom(float mul,const CGraphicsBuffer* image,CGraphicsBuffer* aDest)
{
	ASSERT(image);
	ASSERT(aDest);

    int x=0,y=0;
	int dstw=0,dsth=0;
	int srcw=0,srch=0;
    float xoffs=0,yoffs=0;
	int ixo,iyo;
	unsigned char *dstbuf;
	const unsigned char *srcbuf;
	
	ASSERT(mul>=0);
	ASSERT(mul<=1);

	dsth=aDest->Height();
	dstw=aDest->Width();
	srch=image->Height();
	srcw=image->Width();

	dstbuf=aDest->Ptr();
	srcbuf=image->Ptr();

	yoffs=(dsth*(1.0f-mul))/2.0f;

    for (y=0;y<dsth;y++)
	{
		xoffs=(dstw*(1.0f-mul))/2.0f;
		iyo=(int)yoffs;
		for (x=0;x<dstw;x++)
		{
			ixo=(int)xoffs;
			(*dstbuf)=srcbuf[iyo*srcw+ixo];
			xoffs+=mul;

			dstbuf++;
        }
		yoffs+=mul;
    }
}


void CGraphicsFader::FadeZoomOut(const CGraphicsBuffer* image,CGraphicsBuffer *aDest,const CPalette* pal , int StartTime, int LastingTime)
{
	ASSERT(image);
	ASSERT(aDest);
	ASSERT(pal);
	int time=TimerCounter;

	if (time>=StartTime+LastingTime)
	{
		iGGI->GD()->SetPalette(*pal,0);
		return;
	}

	if (time>=StartTime)
	{
		int phase=(time-StartTime)*256/LastingTime;
		DrawZoom(1-((float)phase*(0.95f/255.0f)),image,aDest);
		iGGI->GD()->SetPalette( *pal, 255-phase );
	}
}

