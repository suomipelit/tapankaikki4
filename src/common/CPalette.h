#ifndef __GAMEPALETTE_H_
#define __GAMEPALETTE_H_

//#include <stdio.h>
#include <SDL.h>
#include "defines.h"
#include "error.h"

class CPalette
{
private:
	SDL_Color iColors[256];

public:
	EXPORT void LoadEFPPal(const char *name);
	EXPORT void LoadPCXPal(const char *name);

	/* Color conversion methods */
	EXPORT static void hsl2rgb(float H,float S,float L,float &R,float &G,float &B );
	EXPORT static void rgb2hsl(float r,float g,float b,float &H,float &S,float &L );
	EXPORT static float hue2rgb(float m1,float m2,float h);

	inline const SDL_Color& Color(int a) const 
	{ 
		ASSERT(a>=0&&a<256);
		return iColors[a];
	};
	inline SDL_Color& Color(int a)
	{ 
		ASSERT(a>=0&&a<256);
		return iColors[a];
	};

	inline SDL_Color* ColorData()
	{
		return iColors;
	}

	inline const SDL_Color* ColorData() const
	{
		return iColors;
	}

	EXPORT void AppendPalette(FILE *f) const;
	EXPORT void AppendPaletteDiv4(FILE *f) const;
	EXPORT void ReadPalette(FILE *f);
	EXPORT void CopyPalette(unsigned char *buffer);
	EXPORT void FourMultiply();

};

#endif
// EOF

