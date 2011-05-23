#ifndef __CENERGYCRATE_H
#define __CENERGYCRATE_H

#include "CCrate.h"

class CEnergyCrate : public CCrate
{
public:
	CEnergyCrate( const CCoord<int>& aPos, IGUIStateController* aController  );
	void TryPickup( CPlayer* aPicker, const CRect<int>& aDrawRect  );
	int SpriteOffset(); // 14*2
	const char* Text();
};

#endif

