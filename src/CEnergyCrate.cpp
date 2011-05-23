#include "CEnergyCrate.h"
#include "CPlayer.h"
#include "common/CMath.h"

CEnergyCrate::CEnergyCrate( const CCoord<int>& aPos, IGUIStateController* aController ) : 
	CCrate( aPos, aController  )
{
}

void CEnergyCrate::TryPickup( CPlayer* aPicker, const CRect<int>& aDrawRect )
{
	if (aPicker->iEnergy < KPlayerMaxEnergy+aPicker->iShield*KShieldEnergyAmount) 
	{
		aPicker->iEnergy += 40;
		aPicker->iEnergy = min(aPicker->iEnergy, float(KPlayerMaxEnergy+aPicker->iShield*KShieldEnergyAmount));
		Taken( aPicker->ScreenPos(), aDrawRect );
	}
}

int CEnergyCrate::SpriteOffset()
{
	return 14*2;
}

const char* CEnergyCrate::Text()
{
	return "energy";
}

