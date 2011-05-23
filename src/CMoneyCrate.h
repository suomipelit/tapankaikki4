#ifndef __CMONEYCRATE_H
#define __CMONEYCRATE_H

#include "CCrate.h"

class CMoneyCrate : public CCrate
{
public:
	CMoneyCrate( const CCoord<int>& aPos, int aAmount, IGUIStateController* aController  );
	void TryPickup( CPlayer* aPicker, const CRect<int>& aDrawRect  );
	int SpriteOffset(); // 14*2
	const char* Text();
protected:
	int iAmount;
	char buf[20];
};

#endif


