#ifndef __CBULLETCRATE_H
#define __CBULLETCRATE_H

#include "CCrate.h"

class IBulletType;

class CBulletCrate : public CCrate
{
public:
	CBulletCrate( const CCoord<int>& aPos, IBulletType* aType, IGUIStateController* aController  );
	void TryPickup( CPlayer* aPicker, const CRect<int>& aDrawRect );
	int SpriteOffset(); // 14*1
	const char* Text();

protected:
	IBulletType* iType;
};

#endif

