#include <vector>
#include "CCrate.h"

#include "common/CFonts.h"
#include "common/CMath.h"

#include "IGUIStateController.h"
#include "CGameGraphicsInterface.h"
#include "CDynGameData.h"
#include "CSoundPlayer.h"
#include "CGameSystem.h"
#include "CIngameData.h"
#include "CLevelRuntimeData.h"

CCrate::CCrate( const CCoord<int>& aPos, IGUIStateController* aController )
{
	iController = aController;
	iLocation.Set(aPos);
	
	iTextCounter=0;
	iTextCounter2=0;
	iInUse=1;
	iWakeUpCounter=0;
}

void CCrate::MoveText() 
{
	if (iTextCounter>0) 
	{
		iTextPos = iTextPos.AddMovement(iTextAngle,iTextCounter2);
		iTextCounter -= iTextCounter2 ;
		iTextCounter2+=0.05f;
		if (iTextCounter <= 0) 
		{
			iInUse=0;
			if ( iController->GameSystem()->DynData()->GameMode()->CrateRespawn() ) 
				iWakeUpCounter=1;
		}
	}
}

void CCrate::CheckWakeUp()
{
	if (iWakeUpCounter>0) 
	{
		iWakeUpCounter++;
		if (iWakeUpCounter >= KCrateWakeupDelay) 
		{
			iController->GameSystem()->DynData()->LevelRuntime()->NewEffect(EEffectTeleport,iLocation.Add(8,8), 0, 0);
			iTextCounter=0;
			iTextCounter2=0;
			iInUse=1;
			iWakeUpCounter=0;
		}
	}
}

void CCrate::SetTextAngle(int aTextAngle)
{
	iTextAngle = aTextAngle;
}

void CCrate::SetTextCounter(float aTextCounter)
{
	iTextCounter = aTextCounter;
}

void CCrate::Taken( const CCoord<int>& aScreenPos, const CRect<int>& aDrawRect )
{
	iController->SoundPlayer()->PlaySample( 
		iController->GameSystem()->DynData()->LevelRuntime()->IngameData()->SoundFX( ESoundEffectClick ),KFullVolume, PAN_MIDDLE);

	iTextPos = iLocation.
			Add(7, 7). // Why this?
			AddX(-iController->GGI()->Font(CGameGraphicsInterface::ESmallMenuFontDark)->Length( Text() ) ).
			Sub( aScreenPos ).
			Add( aDrawRect.TopLeft() );

	SetTextCounter( CMath::GetDist ( iTextPos,aDrawRect.BottomLeft().Add(10,-20) ) +1 );
	SetTextAngle  ( int( CMath::GetAngle( iTextPos,aDrawRect.BottomLeft().Add(10,-20) ) ) );
}

