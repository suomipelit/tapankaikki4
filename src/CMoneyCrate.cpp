#include <vector>
#include "CMoneyCrate.h"
#include "common/Types.h"
#include "common/CGameTypeData.h"
#include "IGUIStateController.h"
#include "CPlayer.h"

CMoneyCrate::CMoneyCrate( const CCoord<int>& aPos, int aAmount, IGUIStateController* aController  ) : 
	CCrate( aPos, aController  )
{
	iAmount = aAmount;
	ASSERT(EF_SNPRINTF(buf,20,"money (%d)",iAmount)>0);
}

void CMoneyCrate::TryPickup( CPlayer* aPicker, const CRect<int>& aDrawRect )
{
	Taken( aPicker->ScreenPos(), aDrawRect );
	aPicker->iCash+= iAmount;
}

int CMoneyCrate::SpriteOffset()
{
	return 14*3;
}

const char* CMoneyCrate::Text()
{
	return buf;
}

