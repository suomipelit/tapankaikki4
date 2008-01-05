#include "CLightTable.h"
#include "CPalette.h"
#include <math.h>
#include <stdlib.h>

void CLightTable::Make(int R,int G,int B,CPalette& aPalette)
{
	int a,b;
	float re,gr,bl,h,s,l;

	for (a=0;a<256;a++)
	{
		re=(float)aPalette.Color(a).r+R;
		gr=(float)aPalette.Color(a).g+G;
		bl=(float)aPalette.Color(a).b+B;

		if (re>255) re=255;
		if (gr>255) gr=255;
		if (bl>255) bl=255;

		if (re<0) re=0;
		if (gr<0) gr=0;
		if (bl<0) bl=0;
 
		CPalette::rgb2hsl(re,gr,bl,h,s,l);

		for (b=0;b<16;b++)
		{
			if (R==0 && G==0 && B==0)
			 if (b>0)
			 {
				l+=((float)(b)/255)/6;
				if (l>1) l=1;
				CPalette::hsl2rgb(h,s,l,re,gr,bl);
			 }

			re+=R/2;
			gr+=G/2;
			bl+=B/2;
 
			if (re>255) re=255;
			if (gr>255) gr=255;
			if (bl>255) bl=255;
 
			if (re<0) re=0;
			if (gr<0) gr=0;
			if (bl<0) bl=0;

			iData[a][b]=ClosestInPalette((int)re,(int)gr,(int)bl,aPalette);
		}
	}
}

void CLightTable::Make2(CPalette& aPalette)
{ // explosive palette
	int a,b,re,gr,bl,ra,ga,ba;

	for (a=0;a<256;a++)
	{
		ra=20;
		ga=0;
		ba=0;

		re=aPalette.Color(a).r;
		gr=aPalette.Color(a).g;
		bl=aPalette.Color(a).b;
 
		for (b=0;b<16;b++)
		{
			if (b>1)
			{
				if (b<9)
				{
					ra+=1;
					ga+=5;
					ba+=1;
				}
				if (b>8)
				ba+=5;

				re=(aPalette.Color(a).r+ra);
				gr=(aPalette.Color(a).g+ga);
				bl=(aPalette.Color(a).b+ba);
			}

			if (re>255) re=255;
			if (gr>255) gr=255;
			if (bl>255) bl=255;

			iData[a][b]=ClosestInPalette(re,gr,bl,aPalette);
		}
	}
}

unsigned char CLightTable::ClosestInPalette(int aRed,int aGreen,int aBlue,CPalette& aPalette)
{
	int a,v,minv=1000;
	unsigned char closest=0;
 
	for (a=0;a<256;a++)
	{
		v=abs(aPalette.Color(a).r - aRed  )+
		  abs(aPalette.Color(a).g - aGreen)+
		  abs(aPalette.Color(a).b - aBlue );
		if (v<minv)
		{	
			minv=v;
			closest=(char)a;
		}
	}
	return(closest);
}
