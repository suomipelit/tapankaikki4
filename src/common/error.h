#ifndef __ERROR_H
#define __ERROR_H

#include "defines.h"
#include <exception>

class CGameException : public std::exception
{
private:
	const char *errorstr;
protected:
	EXPORT CGameException();
	EXPORT CGameException(const char *const& str);
public:
	EXPORT const char *what() const noexcept override;
};

// three levels of exceptions
class CCriticalException : public CGameException
{
public:
	inline CCriticalException(const char *const& str) : CGameException( str ){}
};

class CFailureException : public CGameException
{
public:
	inline CFailureException(const char *const& str) : CGameException( str ){}
};

class CEOFException : public CFailureException
{
public:
	inline CEOFException(const char *const& str) : CFailureException( str ){}
};

#ifdef NDEBUG
#undef NDEBUG
#include <assert.h>
#define NDEBUG
#else
#include <assert.h>
#endif

#include "defines.h"

EXPORT void logwrite(const char *out, ...);

EXPORT void NORETURN error(const char *fmt,...);

EXPORT void TKAssert( const char* aAssertion, const char* file, int line );
#define ASSERT(a) (void)( (a) || (TKAssert(#a, __FILE__, __LINE__), 0) )
#define ASSERTPTR( a ) ASSERT( ((void*)a != (void*)0x00000000)&&((void*)a != (void*)0xFEEEFEEE) && ((void*)a != (void*)0xFDFDFDFD) && ((void*)a != (void*)0xDDDDDDDD))

#define LOG0(a) logwrite(a);
#define LOG1(a,b) logwrite(a,b);
#define LOG2(a,b,c) logwrite(a,b,c);
#define LOG3(a,b,c,d) logwrite(a,b,c,d);
#define LOG4(a,b,c,d,e) logwrite(a,b,c,d,e);

#ifdef _DEBUG
#define DEBUG_ASSERT(exp) ASSERT(exp)
#define DEBUG0(a) LOG0(a)
#define DEBUG1(a,b) LOG1(a,b)
#define DEBUG2(a,b,c) LOG2(a,b,c)
#define DEBUG3(a,b,c,d) LOG3(a,b,c,d)
#define DEBUG4(a,b,c,d,e) LOG4(a,b,c,d,e)
#else
#define DEBUG_ASSERT(a)
#define DEBUG0(a)
#define DEBUG1(a,b)
#define DEBUG2(a,b,c)
#define DEBUG3(a,b,c,d)
#define DEBUG4(a,b,c,d,e)
#endif

#endif
