#ifndef __MATH_H_
#define __MATH_H_

#include "defines.h"
//#include "CCoord.h"
#include <math.h>

template <class T> class CCoord;

#define pi 3.14159265358979f

#define sign(a) ((a)<0? -1 : ((a)>0? 1 : 0))
#define notnull(a) (a==0 ? 1 : a)

#define ANGLEFIX(a) { while (a>=360) a-=360;\
					  while (a<0) a+=360; }

#ifdef __LINUX__

#include <algorithm>
using std::min;
using std::max;

#else

#ifndef min
#define min(a,b) ((a)<=(b)?(a):(b))
#endif

#ifndef max
#define max(a,b) ((a)> (b)?(a):(b)) 
#endif

#endif

#ifndef abs
template<class T> inline T abs(const T& a) { return a<0 ? -a : a; }
#endif

class CMath
{
private:
	CMath();
	
public:

	EXPORT static const CMath* Static();
	EXPORT static void Destroy();

	static inline void Swap(int& a,int& b)
	{
		int temp = a;
		a = b;
		b = temp;
	}

	inline float Sine(int a) const
	{
		ANGLEFIX(a);
		return iSine[a];
	}

	inline float Cose(int a) const
	{
		ANGLEFIX(a);
		return iCosine[a];
	}
	
	template <class T,class U>
	static inline float GetDist(const CCoord<T>& aPos1,const CCoord<U>& aPos2)
	{	
		return GetDist((float)aPos1.X(),(float)aPos1.Y(),(float)aPos2.X(),(float)aPos2.Y());
	}

	static inline float GetDist(float x1,float y1,float x2,float y2)
	{	
		float xd, yd;
		xd = x1-x2;
		yd = y1-y2;
		return sqrtf( xd*xd + yd*yd );
	}

	template <class T,class U>
	static inline float GetAngle(const CCoord<T>& aPos1,const CCoord<U>& aPos2)
	{
		return GetAngle((float)aPos1.X(),(float)aPos1.Y(),(float)aPos2.X(),(float)aPos2.Y());
	}

	static inline float GetAngle(float x, float y, float x2,float y2) 
	{
		float tmp;
		tmp = atan2f(y2 - y, x2 - x)*180.0f/pi;
		ANGLEFIX(tmp);
		return tmp;
	}

private:
	static CMath* iStatic;
	float iSine[360], iCosine[360];
};

#endif

