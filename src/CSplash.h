#ifndef __CSPLASH_H
#define __CSPLASH_H

#include <SDL.h>

class CSplash
{
public:
	static void ShowSplash(const char* aImagePath,const char* aIcon,const char* aCaption);
};

#endif
