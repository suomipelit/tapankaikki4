// CGameMenuBase.cpp: implementation of the CGameMenuBase class.
//
//////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <SDL_keysym.h>

#include "common/CEventHandler.h"
#include "common/CFonts.h"

#include "CGameMenuBase.h"
#include "CGameMenuItem.h"
#include "CGameGraphicsInterface.h"
#include "CGameVersion.h"
#include "IGUIStateController.h"
#include "COptions.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameMenuBase::CGameMenuBase(CGameMenuContainer* aGMC): 
	iActiveItem(false), 
	iBackGround(0), 
	iMenuItems(0), 
	iGMC(aGMC)
{
	ASSERT(aGMC);
}

CGameMenuBase::~CGameMenuBase()
{
	int a;

	for (a=0;a<iMenuItems.size();a++)
	{
		delete iMenuItems[a];
		iMenuItems[a]=NULL;
	}
	iMenuItems.clear();
}

TMenuKeyActions CGameMenuBase::HandleKeyPress(SDL_keysym aKey,CGameGraphicsInterface* aGGI)
{
	CKeys& pl1keys = iGMC->Options()->Data().iKeys[ 0 ];
	DEBUG_ASSERT(aGGI);
	TMenuKeyActions ret;
	
	ret=iMenuItems[iActiveItem]->HandleKeyPress(aKey,this,aGGI);

	switch (ret)
	{
	case EMenuKeyExit:
		iGMC->ChangeState(PrevMenu());
		return EMenuKeyExit;

	case EMenuKeyProcessed:
		return EMenuKeyProcessed;

	case EMenuKeyNotProcessed:
		break;
	default:
		error("CGameMenuBase::HandleKeyPress: Unknown return value from menuitem!");
	};

	if (aKey.sym==SDLK_ESCAPE)
	{
		iGMC->ChangeState(PrevMenu());
		return EMenuKeyProcessed;
	}

	if (aKey.sym==SDLK_DOWN || 
		aKey.sym==pl1keys.keys[ EKeyDOWN ])
	{
		do
		{
			iActiveItem++;
			iActiveItem%=iMenuItems.size();
		} while (!iMenuItems[iActiveItem]->Enabled()||!iMenuItems[iActiveItem]->Active());

		iMenuItems[iActiveItem]->Activate();
		return EMenuKeyProcessed;

	}

	if (aKey.sym==SDLK_UP||
		aKey.sym==pl1keys.keys[ EKeyUP ])
	{
		do{
		iActiveItem--;
		if (iActiveItem<0) 
			iActiveItem+=iMenuItems.size();
		} while (!iMenuItems[iActiveItem]->Enabled()||!iMenuItems[iActiveItem]->Active());
		iMenuItems[iActiveItem]->Activate();
		return EMenuKeyProcessed;
	}

	return EMenuKeyNotProcessed;
}

void CGameMenuBase::Draw(CDrawArea& aDirtyArea,CDrawArea& aDrawArea, CGameGraphicsInterface* aGGI)
{
	DEBUG_ASSERT(aGGI);
	int a;
	CDrawArea area;

	iCleanArea.Combine(aDirtyArea);
	aDrawArea.Combine(iCleanArea);

	for (a=0;a<iCleanArea.Size();a++)
	{
		CRect<int> rectum = iCleanArea.Rect(a,aGGI->DrawBuffer()->Rect());
		aGGI->DrawBuffer()->Copy(
			rectum.iLeft,
			rectum.iTop,
			iBackGround,
			rectum.iLeft,
			rectum.iTop,
			rectum.Width(),
			rectum.Height());
	}

	if (a>0)
	{
		aGGI->Font(CGameGraphicsInterface::ESmallMenuFontDark)->Write(
		0,
		aGGI->DrawBuffer()->Height(),
		CGameVersion::Version(),
		CFonts::EDrawAbove, CFonts::EDrawToRight, 
		aGGI->DrawBuffer());
	}

	iCleanArea.Reset();

	for (a=0;a<iMenuItems.size();a++)
	{
		CRect<int> rect = 
			iMenuItems[a]->Draw(aGGI->DrawBuffer(),
				aGGI,
				aGGI->DrawBuffer()->Width()/2,
				aGGI->DrawBuffer()->Height()/2+(a-iMenuItems.size()/2)*MENUTEXT_VERTSEPARATION, 
				iActiveItem==a? 1: 0);
		iCleanArea.Combine(rect);
	}

	aDrawArea.Combine(iCleanArea);
}

void CGameMenuBase::SetBackground(const CGraphicsBuffer* aBackGround)
{
	iBackGround=aBackGround;
}

void CGameMenuBase::ValueUpdated( CGameMenuItem* aUpdatedItem )
{
}

void CGameMenuBase::ActivateMenu()
{
	while (!iMenuItems[iActiveItem]->Enabled()||!iMenuItems[iActiveItem]->Active())
	{
		iActiveItem=iActiveItem+1;
		ASSERT(iActiveItem<iMenuItems.size());
	}
	iMenuItems[iActiveItem]->Activate();
}

bool CGameMenuBase::RunMenu(CEventHandler* aEH)
{
	ASSERT(iMenuItems.size());
	DEBUG_ASSERT(aEH);

	//int exitFlag=0;
	//int aKey=0;
	//int first=1;

	if (aEH->Kbhit())
		return (HandleKeyPress(aEH->Getch(),iGMC->StateController()->GGI())!=EMenuKeyNotProcessed);
	else
		return false;
}

