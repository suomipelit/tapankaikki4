#ifndef ___LINUX_H__
#define ___LINUX_H__

#ifdef __LINUX__

//#error "NOO"

#define putch putchar
#define EXPORT
#define _stat stat

#define _snprintf snprintf
#define _getcwd getcwd
#define stricmp strcasecmp

#ifdef DEBUG
#  define EF_DEBUG 1
#else
#  define EF_DEBUG 0
#endif

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

//#include "error.h"
EXPORT void TKAssert( const char* aAssertion, const char* file, int line );
#define ASSERT(a) (void)( (a) || (TKAssert(#a, __FILE__, __LINE__), 0) )

inline void ChangeDir(const char* str)
{
	chdir(str);
}

inline char* itoa(int a, char *str,int radix)
{
	ASSERT( radix == 10 || radix == 16 );
	if (radix == 10)
		sprintf(str,"%d", a);
	else
		sprintf(str,"%x", a);
	return str;
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


