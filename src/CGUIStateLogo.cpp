#include <vector>
#include "CGUIStateLogo.h"
#include "CGameGraphicsInterface.h"
#include "IGUIStateController.h"
#include "timer.h"
#include "common/CEventHandler.h"
#include "CSoundPlayer.h"

CGUIStateLogo::CGUIStateLogo(
	CGameData::TPaletteNo aPal, 
	const CGraphicsBuffer* buf, 
	bool aStartMusic, 
	CState aNextState,
	IGUIStateController* aStateController
	) : iNextState(aNextState)
{
	iPal = aPal;
	iImage = buf;
	iStartMusic = aStartMusic;
	iStateController = aStateController;
}

bool CGUIStateLogo::RunFrame()
{
	if (iStateController->State()->First())
		iStateController->StartFadeIn(iPal);

 	if (TIMER_SECS(iTimerStart)>2)
 	{
 		iStateController->StartFadeOut(iNextState);
 	}
	else if (iStateController->EventHandler()->Kbhit())
	{
		iStateController->StartFadeOut(CState(CState::EMainStateMenu,0));
	}
	return false;
}

void CGUIStateLogo::Draw(CDrawArea& aDirtyArea,CDrawArea& aDrawArea, CGameGraphicsInterface* aGGI)
{
	for (int a=0;a<aDirtyArea.Size();a++)
	{
		CRect<int> r = aDirtyArea.Rect(a,aGGI->DrawBuffer()->Rect());
		aGGI->DrawBuffer()->Copy(r.iLeft,r.iTop,iImage,r.iLeft,r.iTop,r.Width(),r.Height());
		aDrawArea.Combine(r);
	}
}

void CGUIStateLogo::EnterState()
{
	if (iStartMusic)
		iStateController->SoundPlayer()->MenuMusic();

	iTimerStart=TimerCounter;
}

void CGUIStateLogo::ExitState(){}
void CGUIStateLogo::EnterSubState(){};
void CGUIStateLogo::ExitSubState(){};

