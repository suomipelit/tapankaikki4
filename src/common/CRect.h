#ifndef __RECT_H_
#define __RECT_H_

#include <SDL.h>
#include "error.h"
#include "CCoord.h"

// forward declaration
template <class T> class CRect;

template <class T>
class CRect
{
public:
	T iLeft;
	T iRight;
	T iTop;
	T iBottom;
	inline CRect(const CCoord<T> aTopLeft,const CCoord<T> aBottomRight)
	{
		Set(aTopLeft,aBottomRight);
	}
	inline CRect(T aLeft,T aTop,T aRight, T aBottom)
	{
		Set(aLeft,aTop,aRight,aBottom);
	}
	inline CRect()
	{
		Set(0,0,0,0);
	}

	inline void Set(T aLeft,T aTop,T aRight, T aBottom)
	{
		iLeft=aLeft;
		iTop=aTop;
		iRight=aRight;
		iBottom=aBottom;
	}

	inline void Set(const CCoord<T> aTopLeft,const CCoord<T> aBottomRight)
	{
		Set(aTopLeft.X(),aTopLeft.Y(),aBottomRight.X(),aBottomRight.Y());
	}

	inline CCoord<T> TopLeft() const
	{
		return CCoord<T>(iLeft,iTop);
	}

	inline CCoord<T> BottomRight() const
	{
		return CCoord<T>(iRight,iBottom);
	}

	inline CCoord<T> TopRight() const
	{
		return CCoord<T>(iRight,iTop);
	}

	inline CCoord<T> BottomLeft() const
	{
		return CCoord<T>(iLeft,iBottom);
	}


	inline CCoord<T> Size() const
	{
		return CCoord<T>(Width(),Height());
	}

	inline T Width() const
	{
		DEBUG_ASSERT(iRight>=iLeft);
		return(iRight-iLeft);
	};
	inline T Height() const
	{
		DEBUG_ASSERT(iBottom>=iTop);
		return(iBottom-iTop);
	};

	// Make sure that Right is on the right side and bottom is lower than top
	inline bool Valid() const
	{
		if (iRight<=iLeft) return false;
		if (iBottom<=iTop) return false;
		return true;
	};

	// Check if given coordinates are inside rect
	inline bool Inside(T aX,T aY) const
	{
		if (aX>=iLeft&&
			aX<iRight&&
			aY>=iTop&&
			aY<iBottom)
			return true;
		return false;
	}

	// Check if given coordinates are inside rect
	inline bool Inside(const CCoord<T>& aPoint) const
	{
		return Inside(aPoint.X(),aPoint.Y());
	}

	inline bool Overlap(const CRect<T>& aRect) const
	{
		if (aRect.Width()==0 ||
			aRect.Height()==0) return false;
		if (Width()==0 ||
			Height()==0) return false;

		if (aRect.iBottom<iTop)  return false;
		if (aRect.iTop>iBottom) return false;
		if (aRect.iRight<iLeft)  return false;
		if (aRect.iLeft>iRight) return false;

		// there's no other option than (at least partial) overlap
		return true;
   	}

	inline void FixSmall(CCoord<T>& aPoint) const
	{
		if (aPoint.X()+1==iLeft) aPoint.SetX(iLeft);
		if (aPoint.X()==iRight)  aPoint.SetX(iRight-1);
		if (aPoint.Y()+1==iTop)  aPoint.SetY(iTop);
		if (aPoint.Y()==iBottom) aPoint.SetY(iBottom-1);
	}

	inline void Combine(const CRect<T>& aRect)
	{
		if (aRect.Width()==0 && aRect.Height() == 0)

		if (Width()==0&&Height()==0)
		{
			*this = aRect;
			return;
		}

		if (aRect.iTop<iTop)		iTop	= aRect.iTop;
		if (aRect.iLeft<iLeft)		iLeft	= aRect.iLeft;
		if (aRect.iBottom>iBottom)	iBottom = aRect.iBottom;
		if (aRect.iRight>iRight)	iRight	= aRect.iRight;
	}

	inline bool Equals(const CRect<T>& aRect) const
	{
		if (aRect.iLeft!=iLeft) return false;
		if (aRect.iTop!=iTop) return false;
		if (aRect.iBottom!=iBottom) return false;
		if (aRect.iRight!=iRight) return false;

		return true;
	}

	inline operator SDL_Rect() const
	{
		SDL_Rect r;
		r.w = Width();
		r.h = Height();
		r.x = iLeft;
		r.y = iTop;
		return r;
	}

#ifdef _DEBUG

	// Some unit testing
	inline static void SelfTest()
	{
		// Test constructors
		CRect<int> zerorect;
		CRect<int> nrect(CCoord<int>(1,2),CCoord<int>(9,6));
		CRect<int> nrect2(1,3,5,7);

		ASSERT(zerorect.iLeft == 0);
		ASSERT(zerorect.iRight == 0);
		ASSERT(zerorect.iTop == 0);
		ASSERT(zerorect.iBottom == 0);

		ASSERT(nrect.iLeft == 1);
		ASSERT(nrect.iRight == 9);
		ASSERT(nrect.iTop == 2);
		ASSERT(nrect.iBottom == 6);

		ASSERT(nrect2.iLeft == 1);
		ASSERT(nrect2.iRight == 5);
		ASSERT(nrect2.iTop == 3);
		ASSERT(nrect2.iBottom == 7);


		// Test Set functions
		CRect<int> testrect,testrect2;

		testrect.Set(1,3,5,7);
		testrect2.Set(CCoord<int>(1,3),CCoord<int>(5,7));
		ASSERT(testrect.iLeft == 1);
		ASSERT(testrect.iRight == 5);
		ASSERT(testrect.iTop == 3);
		ASSERT(testrect.iBottom == 7);

		ASSERT(testrect2.iLeft == 1);
		ASSERT(testrect2.iRight == 5);
		ASSERT(testrect2.iTop == 3);
		ASSERT(testrect2.iBottom == 7);

		// Test Topleft & BottomRight

		ASSERT(testrect.TopLeft().X() == 1);
		ASSERT(testrect.TopLeft().Y() == 3);
		ASSERT(testrect.BottomRight().X() == 5);
		ASSERT(testrect.BottomRight().Y() == 7);

		// Test Width & Height

		ASSERT(testrect.Width()==4);
		ASSERT(testrect.Height()==4);

		ASSERT(nrect.Width()==8);
		ASSERT(nrect.Height()==4);

		// Test Valid
		CRect<int> nonvalid(5,6,1,2);

		ASSERT(testrect.Valid());
		ASSERT(!nonvalid.Valid());

		// Test Inside

		CRect<int> testrect3(4,5,100,200);

		ASSERT(!testrect3.Inside(3,4));
		ASSERT(testrect3.Inside(4,5));
		ASSERT(!testrect3.Inside(4,4));
		ASSERT(!testrect3.Inside(3,5));

		ASSERT(!testrect3.Inside(100,200));
		ASSERT(testrect3.Inside(99,199));
		ASSERT(!testrect3.Inside(99,200));
		ASSERT(!testrect3.Inside(100,199));

		ASSERT(!testrect3.Inside(CCoord<int>(3,4)));
		ASSERT(testrect3.Inside(CCoord<int>(4,5)));
		ASSERT(!testrect3.Inside(CCoord<int>(4,4)));
		ASSERT(!testrect3.Inside(CCoord<int>(3,5)));

		ASSERT(!testrect3.Inside(CCoord<int>(100,200)));
		ASSERT(testrect3.Inside(CCoord<int>(99,199)));
		ASSERT(!testrect3.Inside(CCoord<int>(99,200)));
		ASSERT(!testrect3.Inside(CCoord<int>(100,199)));

		// Test Overlap
		CRect<int> over1(1,1,5,5),
				 over2(5,5,9,9),
				 over3(1,1,9,9),
				 over4(2,2,3,3),
				 over5(20,20,30,30),
				 over6(10,10,15,15);

		ASSERT(over1.Overlap(over2));
		ASSERT(over2.Overlap(over3));
		ASSERT(over1.Overlap(over3));
		ASSERT(over1.Overlap(over4));
		ASSERT(!over1.Overlap(over5));
		ASSERT(!over3.Overlap(over6));

		ASSERT(over2.Overlap(over1));
		ASSERT(over3.Overlap(over2));
		ASSERT(over3.Overlap(over1));
		ASSERT(over4.Overlap(over1));
		ASSERT(!over5.Overlap(over1));
		ASSERT(!over6.Overlap(over3));

		// Test FixSmall

		// Test Combine
	};

#endif

};

#endif
// EOF

