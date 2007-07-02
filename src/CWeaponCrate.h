#ifndef __CWEAPONCRATE_H
#define __CWEAPONCRATE_H

#include "CCrate.h"

class IWeaponType;

class CWeaponCrate : public CCrate
{
public:
	CWeaponCrate( const CCoord<int>& aPos, IWeaponType* aType, IGUIStateController* aController  );
	void TryPickup( CPlayer* aPicker, const CRect<int>& aDrawRect );
	int SpriteOffset(); // 14*0
	const char* Text();

protected:

	IWeaponType* iType;
};

#endif

