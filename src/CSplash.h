#ifndef __SPLASH_H_
#define __SPLASH_H_

#include <SDL.h>

class CSplash
{
public:
	static void ShowSplash(const char* aImagePath,const char* aIcon,const char* aCaption);
};

#endif
