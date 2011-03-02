#include "common/CGameTypeData.h"
#include "common/Types.h"
#include "common/IWeaponType.h"

#include "CWeaponCrate.h"
#include "CPlayer.h"
#include "IGUIStateController.h"

CWeaponCrate::CWeaponCrate( const CCoord<int>& aPos, IWeaponType* aType, IGUIStateController* aController ) : 
	CCrate( aPos, aController )
{
	iType = aType;
}

void CWeaponCrate::TryPickup( CPlayer* aPicker, const CRect<int>& aDrawRect )
{
	int wno = iController->GameData()->TypeData()->ReverseMapWeapon( iType );

	if (aPicker->iWeapons[ wno ] < iType->SingleHand()+1)  
	{
		aPicker->iWeapons[ wno ]++;
		Taken( aPicker->ScreenPos(), aDrawRect );
	}
}

int CWeaponCrate::SpriteOffset()
{
	return 0;// 14*0
}

const char* CWeaponCrate::Text()
{
	return iType->Name();
}


