#include "CSplash.h"
#include "common/error.h"
#include "common/files.h"

void CSplash::ShowSplash(const char* aImagePath,const char* aIcon,const char* aCaption)
{
	SDL_Rect src, dest;

	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		error("Unable to initialize SDL: %s \n", SDL_GetError());
	}

	SDL_Surface* bitmap = SDL_LoadBMP(getdatapath(std::string(aImagePath)).c_str());
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

	SDL_Window* screen = SDL_CreateWindow(aCaption,
	                                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	                                      bitmap->w, bitmap->h,
	                                      SDL_WINDOW_ALLOW_HIGHDPI|SDL_WINDOW_BORDERLESS);
	if (screen == NULL) 
	{
		error("Unable to create window: %s\n", SDL_GetError());
	}

	if (aIcon!=NULL && exists(aIcon))
	{
		SDL_Surface* icon=SDL_LoadBMP(aIcon);
		SDL_SetWindowIcon(screen, icon);
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(screen, -1, 0);
	if (renderer == NULL)
	{
		error("Unable to create renderer: %s\n", SDL_GetError());
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, bitmap);

	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);

	SDL_DestroyTexture( texture );
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( screen );
	SDL_FreeSurface( bitmap );
}

