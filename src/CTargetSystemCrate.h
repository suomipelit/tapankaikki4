#ifndef __CTARGETSYSTEMCRATE_H
#define __CTARGETSYSTEMCRATE_H

#include "CCrate.h"

class CTargetSystemCrate : public CCrate
{
public:
	CTargetSystemCrate( const CCoord<int>& aPos, IGUIStateController* aController  );
	void TryPickup( CPlayer* aPicker, const CRect<int>& aDrawRect  );
	int SpriteOffset(); // 14*2
	const char* Text();
};

#endif

