#ifndef __WINDOWS_H
#define __WINDOWS_H

#ifdef WIN32

#ifdef COMMON_EXPORTS
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport)
#endif
#define NORETURN __declspec(noreturn)

/* Some functions and names that differ on windows and other platforms */
#define EF_STAT _stat
#define EF_SNPRINTF _snprintf
#define EF_GETCWD _getcwd
#define EF_SDLWINDOW window /* window in SDL_SysWMinfo structure */

#define DIR_SEPARATOR '\\'

#include <direct.h>

inline void ChangeDir(const char* str)
{
	_chdir(str);
}

#ifdef _DEBUG
#define EF_DEBUG 1
#else
#define EF_DEBUG 0
#endif // _DEBUG

#ifndef min
#define min(a,b) ((a)<=(b)?(a):(b))
#endif

#ifndef max
#define max(a,b) ((a)> (b)?(a):(b)) 
#endif

inline const char* strcasestr(const char* src,const char* match)
{
	ASSERT(src);
	ASSERT(match);
	int a;
	int mlen=strlen(match);
	int l=(int)(strlen(src)-mlen);
	
	for (a=0;a<=l;a++)
		if (strnicmp(src+a,match,mlen)==NULL)
			return src+a;
	
	return NULL;
}

#endif // _WINDOWS

#endif // ___WINDOWS_H_


