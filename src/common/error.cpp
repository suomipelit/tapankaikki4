#ifdef _WINDOWS
#include <windows.h>
#include <winuser.h>
#else
#include <stdarg.h>
#endif
#include <string>
#include <stdio.h>
#include "error.h"

CGameException::CGameException(const char *const& str)
#ifndef __LINUX__
: exception( str )
#endif
{
#ifdef _DEBUG
//	_asm { int 3h }; // Break here
#endif
}

void TKAssert( const char* aAssertion, const char* file, int line )
{
	std::string str;
	char buf[20];

	str = "Assertion (";
	str += aAssertion;
	str += ") failed at ";
	str += file;
	str += ",";
	_snprintf(buf,20,"%d",line);
	str += buf;
	str += KNewLine;

	throw CFailureException( str.c_str() );
 }

void logwrite(const char *out, ...)
{ 
	FILE *f=fopen("runlog.txt","a+");
	va_list list;
	va_start(list, out);

	vfprintf(f,out,list);
	va_end( list );

	fclose(f);
}

void error(const char *fmt,...)
{	auto va_list ap;
	char str[5000];

	va_start(ap,fmt);

	vsprintf(str,fmt,ap);
	logwrite("Error: %s \nExiting...\n",str);

#ifdef _WINDOWS
	MessageBox(NULL,str,"Error!",MB_APPLMODAL|MB_OK|MB_ICONERROR);
#else
	fprintf(stderr, "Error: %s\n", str);
#endif
	
#ifdef _DEBUG
	_asm { int 3h }; // Let's break here
#endif

	va_end(ap);
	exit(1);
}
