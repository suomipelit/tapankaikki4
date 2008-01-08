#ifndef __LINUX_H
#define __LINUX_H

#ifdef __LINUX__

#define EXPORT
#define NORETURN

/* Some functions and names that differ on windows and other platforms */
#define EF_STAT stat
#define EF_SNPRINTF snprintf
#define EF_GETCWD getcwd
#define EF_SDLWINDOW info.x11.window /* window in SDL_SysWMinfo structure */

#define DIR_SEPARATOR '/'

#ifdef DEBUG
#  define EF_DEBUG 1
#else
#  define EF_DEBUG 0
#endif

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

inline void ChangeDir(const char* str)
{
	chdir(str);
}

inline char* strupr(char *buf)
{	int len=strlen(buf);
	
	for (int a=0;a<len;a++)
	{
		buf[a]=toupper(buf[a]);
	}
	return buf;
}

inline char* strlwr(char *buf)
{	int len=strlen(buf);
	
	for (int a=0;a<len;a++)
	{
		buf[a]=tolower(buf[a]);
	}	
	return buf;
}

inline char *strset(char *buf,char fill)
{	int len=strlen(buf);
	
	for (int a=0;a<len;a++)
	{
		buf[a]=fill;
	}	
	return buf;
}

#endif // linux

#endif // ___LINUX_H_


