#include "CPalette.h"

#include "CMath.h"

#ifndef __LINUX__
#include <string.h>
#endif

void CPalette::LoadEFPPal(const char *name)
{
	ASSERT(name);
	ASSERT(strlen(name)>0);

	char dir[256];

	FILE *efp=fopen(name,"rb");

	if (efp==NULL)
		error("CPalette::LoadEFPPal: File %s couldn't be opened (curdir==%s)!", name,_getcwd( dir, 256));

	fseek(efp,-768,SEEK_END);
	ReadPalette(efp);
	fclose(efp);
}

void CPalette::LoadPCXPal(const char *name)
{
	ASSERT(name);
	ASSERT(strlen(name)>0);

	char dir[256];
	FILE *pcx=fopen(name,"rb");

	if (!pcx)
		error("CPalette::LoadPCXPal: File %s couldn't be opened (curdir==%s)!", name,_getcwd( dir, 256));

	fseek(pcx,-768,SEEK_END);
	ReadPalette(pcx);
	
	fclose(pcx);
}

void CPalette::AppendPalette(FILE *f) const
{
	ASSERT(f);
	int a;

	for (a=0;a<256;a++)
	{
		fputc(iColors[a].r,f);
		fputc(iColors[a].g,f);
		fputc(iColors[a].b,f);
	}
}

void CPalette::AppendPaletteDiv4(FILE *f) const
{
	ASSERT(f);
	int a;

	for (a=0;a<256;a++)
	{
		fputc(iColors[a].r/4,f);
		fputc(iColors[a].g/4,f);
		fputc(iColors[a].b/4,f);		
	}
}

void CPalette::ReadPalette(FILE *f)
{
	ASSERT(f);
	int a;

	for (a=0;a<256;a++)
	{
		iColors[a].r = fgetc( f );
		iColors[a].g = fgetc( f );
		iColors[a].b = fgetc( f );
	}
}

void CPalette::FourMultiply()
{	
	int c;
	for(c=0;c<256;c++)
	{
		assert(iColors[c].r<64);
		assert(iColors[c].g<64);
		assert(iColors[c].b<64);
		iColors[c].r<<=2;
		iColors[c].g<<=2;
		iColors[c].b<<=2;
	}
}

void CPalette::rgb2hsl(float r, float g, float b, float &H, float &S, float &L) 
{
	float delta;
	float cmax;
	float cmin;
	r/=255;
	g/=255;
	b/=255;
	cmax=max(r, max(g, b));
	cmin=min(r, min(g, b));
	L=(cmax+cmin) /2.0F;
	if (cmax == cmin) 
	{
		S=0;
		H=0;// it's really undefined
	}
	else 
	{
		if (L<0.5F) 
			S=(cmax-cmin) /(cmax+cmin);
		else
			S=(cmax-cmin) /(2.0F-cmax-cmin);
		delta=cmax-cmin;
		if (r == cmax) 
			H=(g-b) /delta;
		else if (g == cmax) 
			H=2.0F+(b-r) /delta;
		else
			H=4.0F+(r-g) /delta;
			H /= 6.0F;
		if (H<0.0F) 
			H+= 1;
	}
}

void CPalette::hsl2rgb(float H, float S, float L, float &R, float &G, float &B) 
{
	float m1, m2;
	if (S == 0) 
	{
		R=G=B=L;
	}
	else 
	{
		if (L<= 0.5F) 
		m2=L*(1.0F+S);
		else
		m2=L+S-L*S;
		m1=2.0F*L-m2;
		R=hue2rgb(m1, m2, H+1.0F/3.0F);
		G=hue2rgb(m1, m2, H);
		B=hue2rgb(m1, m2, H-1.0F/3.0F);
	}
	R*=255;
	G*=255;
	B*=255;
}

float CPalette::hue2rgb(float m1, float m2, float h) 
{
	if (h<0) h+=1.0F;
	if (h>1) h-=1.0F;
	if (6.0F*h<1) 
		return (m1+(m2-m1)*h*6.0F);
	if (2.0F*h<1.0F) 
		return m2;
	if (3.0F*h<2.0F) 
		return (m1+(m2-m1)*((2.0F/3.0F) -h) *6.0F);
	return m1;
}

void CPalette::CopyPalette(unsigned char *buffer)
{
	for (int a=0;a<256;a++)
	{
		iColors[a].r = *(buffer++);
		iColors[a].g = *(buffer++);
		iColors[a].b = *(buffer++);
	}
}
