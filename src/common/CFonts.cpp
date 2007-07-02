#include "CFonts.h"
#include "CGraphicsBuffer.h"

CFonts::CFonts(const char *name, int aOneIsZero)
{
	ASSERT(!Load(name));
	iOneIsZero=aOneIsZero;
}

CFonts::~CFonts()
{
    free(iFontData);   
}

int CFonts::Load(const char *name) 
{
	FILE *fnt;
	int a;

	ASSERT(name!=NULL);

	if ((fnt=fopen(name,"rb")) != 0)
	{
		iWidth=fgetc(fnt);
		iHeight=fgetc(fnt);
		iFontData=(char *) malloc(iHeight*iWidth*256);
		fseek(fnt,256*2,0);
		a=fread(iFontData,iWidth*iHeight*256,1,fnt);
		fclose(fnt);

		if (a!=1)
			return 1;
			
		return 0;
	} 
	else
		return 1;
}


CRect<int> CFonts::Write(int aX,int aY,const unsigned char *aStr,TVertAlign aVAlign, THorizAlign aHAlign,CGraphicsBuffer* aTarget) const
{
	ASSERT(aStr);
	ASSERT(aTarget);
	int a,c,d,xpos1,xpos2,col,textcnt=0,fsize=iWidth*iHeight;
	int maxX=aX,maxY=aY;

	if (aStr[0]==0) 
		return CRect<int>();

	switch (aVAlign)
	{
		case EDrawBelow: break; // nothing to change
		case EDrawInVertMiddle: aY-=Height()/2; break;
		case EDrawAbove: aY-=Height(); break;
		default: throw CCriticalException("CFonts::Write: Invalid vert. align");
	};

	switch (aHAlign)
	{
		case EDrawToRight: break; // nothing to change
		case EDrawInMiddle: aX-=Length(aStr)/2; break;
		case EDrawToLeft: aX-=Length(aStr); break;
		default: throw CCriticalException("CFonts::Write: Invalid horiz. align");
	};


	for (a=0; aStr[a]!=0; a++) 
	{
		c=0;
		col=0;
		while (col==0 &&c<iWidth) 
		{
			// check from left
			for (d=0; d<iHeight; d++) 
			if (iFontData[aStr[a]*fsize+d*iWidth+c]!=0) col=1;
			if (col==0) c++;
		}
		xpos1=c;
		c=iWidth;
		col=0;
		while (col==0 &&c>0) 
		{
			// check from right
			for (d=0; d<iHeight; d++) 
			if (iFontData[aStr[a]*fsize+d*iWidth+(c-1) ]!=0) col=1;
			if (col==0) c--;
		}
		xpos2=c;
		if (xpos1>xpos2) 
		{
			xpos1=0;xpos2=iWidth / 2;
		}
		for(d=0; d<iHeight; d++) 
			for(c=xpos1; c<xpos2; c++) 
				if (iFontData[aStr[a]*fsize+(d*iWidth)+c]!=0)
				   if ((d+aY)<aTarget->Height() && 
						(d+aY)>=0 && 
						(c-xpos1)+aX+textcnt<aTarget->Width() &&
						(c-xpos1)+aX+textcnt>=0)
					{
						if (iFontData[aStr[a]*fsize+(d*iWidth)+c]==1 && iOneIsZero)
							aTarget->PutPixel((c-xpos1)+aX+textcnt,d+aY,0);
						else
							aTarget->PutPixel((c-xpos1)+aX+textcnt,d+aY,iFontData[aStr[a]*fsize+(d*iWidth)+c]);

						if (maxX<(c-xpos1)+aX+textcnt) maxX = (c-xpos1)+aX+textcnt;
						if (maxY<d+aY ) maxY = d+aY;
					}
		textcnt+=xpos2-xpos1;
	}
	return CRect<int>(aX,aY,maxX+1,maxY+1);
};


int CFonts::Length(const unsigned char *str) const
{
	ASSERT(str!=NULL);

	int length=0;
	int a,d,xpos1,xpos2;

	for (a=0; str[a]!=0; a++) 
	{
		int c=0;
		int col=0;
		while (col==0 &&c<iWidth) 
		{
			// check from left
			for (d=0; d<iHeight; d++) 
				if (iFontData[str[a]*(iWidth*iHeight)+(d*iWidth)+c ]!=0) 
					col=1;
			if (col==0) c++;
		}
		xpos1=c;
		c=iWidth-1;
		col=0;
		while (col==0 &&c >0) 
		{
			// check from right
			for (d=0; d<iHeight; d++) 
			 if (iFontData[str[a]*(iWidth*iHeight)+(d*iWidth)+c ]!=0) col=1;
			if (col==0) c--;
		}
		xpos2=c;
		if (xpos1>xpos2) //empty == space
		{
			xpos1=0;xpos2=iWidth / 2;
		}
		length+=(xpos2-xpos1)+1;
	}
	return(length);
}

