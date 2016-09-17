#ifndef __CCOORD_H
#define __CCOORD_H

#include <sstream>
#include <iostream> 
#include <string>

template <class T> class CCoord;

#include "CMath.h"

template <class T>
class CCoord
{
protected:

	T iX,iY;

public:
	inline CCoord(T aX, T aY): iX(aX), iY(aY) {};
	inline CCoord(){ iX=0;iY=0; };

	inline T X() const { return iX; }
	inline T Y() const { return iY; }

	inline void Set(T aX, T aY){ iX=aX;iY=aY;}
	inline void Set(const CCoord<T>& aCoord){ iX=aCoord.iX;iY=aCoord.iY;}
	inline void SetX(T aX){ iX=aX; }
	inline void SetY(T aY){ iY=aY; }
	inline void SetMovement(int aAngle, float aSpeed)
        {
                Set(
                      (T)(CMath::Static() ->Cose(aAngle)*aSpeed),
                      (T)(CMath::Static() ->Sine(aAngle)*aSpeed)
                   );
        }

	inline CCoord AddMovement(int aAngle, float aSpeed) const { return Add(T(CMath::Static()->Cose(aAngle)*aSpeed),T(CMath::Static()->Sine(aAngle)*aSpeed)); };
	inline CCoord Sub(T aX, T aY)const { return Sub(CCoord<T>(aX,aY));}
	inline CCoord Add(T aX, T aY)const { return Add(CCoord<T>(aX,aY));}
	inline CCoord Sub(const CCoord<T>& aValue)const { return CCoord(iX-aValue.iX,iY-aValue.iY);}
	inline CCoord Add(const CCoord<T>& aValue)const { return CCoord(iX+aValue.iX,iY+aValue.iY);}
	inline CCoord SubX(T aVal)const { return Sub(aVal,0);}
	inline CCoord SubY(T aVal)const { return Sub(0,aVal);}
	inline CCoord AddX(T aVal)const { return Add(aVal,0);}
	inline CCoord AddY(T aVal)const { return Add(0,aVal);}
	inline CCoord Div(T aDivisor)const { return CCoord(iX/aDivisor,iY/aDivisor);}
	inline CCoord Mul(T aMultiplier)const { return CCoord(iX*aMultiplier,iY*aMultiplier);}

	inline T VectorAngle() const { return CMath::Static()->GetAngle(CCoord<T>(0,0),*this); }
	inline T VectorLength() const { return CMath::Static()->GetDist(CCoord<T>(0,0),*this); }

	inline std::string String() const
	{
		std::stringstream tmp;
		std::string t;
		tmp<<"(" << iX << "," << iY << ")";
		tmp >> t;
		return t;
	}

	inline operator CCoord<int>() const 
	{ 
		CCoord<int> tmp;
		tmp.SetX((int)iX);
		tmp.SetY((int)iY);
		return tmp;
	}
	inline operator CCoord<float>() const 
	{ 
		CCoord<float> tmp;
		tmp.SetX((float)iX);
		tmp.SetY((float)iY);
		return tmp;
	}
	inline operator CCoord<double>() const
	{ 
		CCoord<double> tmp;
		tmp.SetX((double)iX);
		tmp.SetY((double)iY);
		return tmp;
	}

	inline bool operator ==(const CCoord<T>& aAnother) const
	{
		if (iX != aAnother.iX) return false;
		if (iY != aAnother.iY) return false;

		return true;
	}

	inline bool operator !=(const CCoord<T>& aAnother) const
	{
		return (!(*this == aAnother));
	}

};

#endif


