#include "CDrawArea.h"

CDrawArea::~CDrawArea()
{
	
}

void CDrawArea::Reset()
{
	iRects.clear();
	iRects.reserve( KMaxRectsInDrawArea );
}

CDrawArea::CDrawArea(): iGiveUp (false)
{
	iRects.reserve( KMaxRectsInDrawArea );
}

void CDrawArea::Combine(const CRect<int>& aRect)
{
	if (iGiveUp)
		return;

	if (aRect.Width()==0||aRect.Height()==0) 
		return;

	// At the moment rect amount is limited. Limitation could also be 3/4 of size etc.
	ASSERT(iRects.size()<=KMaxRectsInDrawArea);

	for (int a=0;a<iRects.size();a++)
	{
		if (iRects[a].Overlap(aRect))
		{	// rect is part of other rect
			iRects[a].Combine(aRect); // combine areas

			// Check if last rects overlap with combined rect.
			// This has to be done backwards.
			for (int b=iRects.size()-1;b>a;b--)
			{
				if (iRects[a].Overlap(iRects[b]))
				{
					std::vector< CRect<int> >::iterator it=iRects.begin()+b;
					iRects[a].Combine(*it);
					iRects.erase(it);
				}
			}
			return;
		}
	}

	if (iRects.size()==KMaxRectsInDrawArea)
	{
		iGiveUp=true;
		return;
	}
	else 
		iRects.push_back(aRect);
}

void CDrawArea::Combine(CDrawArea& aDrawArea)
{
	for (int a=0;a<aDrawArea.iRects.size()&&!iGiveUp;a++)
	{
		Combine(aDrawArea.iRects[a]);
	}
}

int CDrawArea::Size() const
{
	if (iGiveUp)
		return 1;
	return iRects.size();
}

const CRect<int>& CDrawArea::Rect(int a, const CRect<int>& aMaximum)
{
	if (iGiveUp)
		return aMaximum;
	
	DEBUG_ASSERT(iRects.size()>a);

	CRect<int>& r=iRects[a];
	DEBUG_ASSERT(r.Width()<=aMaximum.Width());
	DEBUG_ASSERT(r.Height()<=aMaximum.Height());
	DEBUG_ASSERT(r.iLeft>=aMaximum.iLeft);
	DEBUG_ASSERT(r.iRight<=aMaximum.iRight);
	DEBUG_ASSERT(r.iTop>=aMaximum.iTop);
	DEBUG_ASSERT(r.iBottom<=aMaximum.iBottom);

	return r;
}


void CDrawArea::SelfTest()
{
#ifdef _DEBUG
	CDrawArea a,b,c;
	CRect<int> rMax(0,0,640,400);
	CRect<int> r1(0,0,100,100);
	CRect<int> r2(0,0,500,300);
	CRect<int> r3(200,200,300,300);
	CRect<int> rTooBig(0,0,1000,1000);

	ASSERT(a.Size()==0);

	a.Combine(r1);
	ASSERT(a.Size()==1);
	ASSERT(a.Rect(0,rMax).Equals(r1));
	a.Combine(r3);
	ASSERT(a.Size()==2);
	ASSERT(a.Rect(0,rMax).Equals(r1));
	ASSERT(a.Rect(1,rMax).Equals(r3));
	a.Combine(r2);
	ASSERT(a.Size()==1);
	ASSERT(a.Rect(0,rMax).Equals(r2));
	a.Combine(rTooBig);
	ASSERT(a.Size()==1);
#endif
}
