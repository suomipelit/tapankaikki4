#ifndef __CDRAWAREA_H
#define __CDRAWAREA_H

#include <vector>
#include "CRect.h"

const int KMaxRectsInDrawArea = 32;

class CDrawArea
{
public:
	EXPORT ~CDrawArea();
	EXPORT CDrawArea();
	EXPORT void Combine(const CRect<int>& aRect);
	EXPORT void Combine(CDrawArea& aDrawArea);
	EXPORT void Reset();
	EXPORT int Size() const;
	EXPORT const CRect<int>& Rect(int a, const CRect<int>& aMaximum);

	EXPORT static void SelfTest();

private:
	std::vector< CRect<int> > iRects;
	bool iGiveUp;
};


#endif

