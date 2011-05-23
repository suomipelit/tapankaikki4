#include "CState.h"

CState::CState(TMainState aMainState, int aSubstate):iMainState(aMainState),iSubState(aSubstate)
{
	
}

bool CState::operator == (const CState& aState)
{
	if (iMainState == aState.iMainState&&
		iSubState == aState.iSubState)
		return true;
	else
		return false;
}

