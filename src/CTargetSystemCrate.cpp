#include "CTargetSystemCrate.h"
#include "common/Types.h"
#include "common/CGameTypeData.h"
#include "IGUIStateController.h"
#include "CPlayer.h"

CTargetSystemCrate::CTargetSystemCrate( const CCoord<int>& aPos, IGUIStateController* aController  ) : 
	CCrate( aPos, aController  )
{
}

void CTargetSystemCrate::TryPickup( CPlayer* aPicker, const CRect<int>& aDrawRect )
{
	if (!aPicker->iTargetSystem) 
	{
		aPicker->iTargetSystem = true;
		Taken( aPicker->ScreenPos(), aDrawRect );
	}
}

int CTargetSystemCrate::SpriteOffset()
{
	return 14*4;
}

const char* CTargetSystemCrate::Text()
{
	return "target system";
}

