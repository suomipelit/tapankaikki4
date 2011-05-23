#ifndef __DEBUG_H_
#define __DEBUG_H_

#define __DECLARE_TEST inline void __INVARIANT_TEST() const

#define TEST( a ) inline void a::__INVARIANT_TEST() const

#ifdef _DEBUG
#define SELFTEST __INVARIANT_TEST()
#else
#define SELFTEST
#endif

#if (0)
//#ifdef _DEBUG

//#include "common/CGraphicsBuffer.h"
#include "CGraphicsBuffer.h"

#include <stdlib.h>
#include <string.h>

#define KBlocks 100
#define KBlockSize 10240

inline void MEMCLEAN()
{
	void* blocks[KBlocks];
	CGraphicsBuffer* buf;
	char tmp[1000];
	int a;
	memset(tmp, 0, 1000);

	buf = new CGraphicsBuffer(CRect<int>(0,0,100,100));
	delete buf;

	//allocate
	for (a=0;a<KBlocks;a++)
	{
		blocks[a] = calloc(KBlockSize,1);
	}

	// free
	for (a=0;a<KBlocks;a++)
	{
		free(blocks[a]);
	}

	//allocate C++
	for (a=0;a<KBlocks;a++)
	{
		blocks[a] = new char[KBlockSize];
		memset(blocks[a],0,KBlockSize);
	}

	// free C++
	for (a=0;a<KBlocks;a++)
	{
		delete [] blocks[a];
	}
}
#else

#define MEMCLEAN()

#endif

#endif

