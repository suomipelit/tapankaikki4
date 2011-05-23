#include "CSplash.h"
#include "common/error.h"
#include "common/files.h"

void CSplash::ShowSplash(const char* aImagePath,const char* aIcon,const char* aCaption)
{
	SDL_Surface* screen;
	SDL_Rect src, dest;

	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		error("Unable to initialize SDL: %s \n", SDL_GetError());
	}

	SDL_Surface* bitmap = SDL_LoadBMP(aImagePath);
	if (bitmap  == NULL)
	{
		error("Unable to load splash.\n");
	}


	src.x = 0;
	src.y = 0;
	src.w = bitmap->w;
	src.h = bitmap->h;
	dest.x = 0;
	dest.y = 0;
	dest.w = bitmap->w;
	dest.h = bitmap->h;


	atexit(SDL_Quit);

	// This must be done before videomode call...
	if (aIcon!=NULL && exists(aIcon))
	{
		SDL_Surface* icon=SDL_LoadBMP(aIcon);
		SDL_WM_SetIcon(icon, NULL);
	}
	SDL_WM_SetCaption(aCaption,NULL);

	screen = SDL_SetVideoMode(bitmap->w, bitmap->h, 32, SDL_HWPALETTE|SDL_NOFRAME);
	if (screen == NULL) 
	{
		error("Unable to set video mode: %s\n", SDL_GetError());
	}
	SDL_BlitSurface(bitmap, &src, screen, &dest);
    SDL_UpdateRect(screen, 0, 0, 0, 0);
	SDL_Flip(screen);

	SDL_FreeSurface( bitmap );
}

