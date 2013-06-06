#include <vector>
#include "CBulletCrate.h"
#include "common/Types.h"
#include "common/CGameTypeData.h"
#include "common/IBulletType.h"
#include "IGUIStateController.h"
#include "CPlayer.h"

CBulletCrate::CBulletCrate( const CCoord<int>& aPos, IBulletType* aType, IGUIStateController* aController  ) : 
	CCrate( aPos, aController  )
{
	iType = aType;
}

void CBulletCrate::TryPickup( CPlayer* aPicker, const CRect<int>& aDrawRect )
{
	int bno = iController->GameData()->TypeData()->ReverseMapBullet( iType );

	if (aPicker->iBullets[ bno ]<iType->MaximumForPlayer()*iType->Multiplier()) 
	{
		aPicker->iBullets[ bno ]+= iType->AmountInCrate();
		aPicker->iBullets[ bno ] = min( aPicker->iBullets[ bno ], iType->MaximumForPlayer()*iType->Multiplier());
		Taken( aPicker->ScreenPos(), aDrawRect );
	}
}

int CBulletCrate::SpriteOffset()
{
	return 14;
}

const char* CBulletCrate::Text()
{
	return iType->Name();
}

