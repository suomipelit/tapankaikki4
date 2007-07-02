// CGameMenuItem.cpp: implementation of the CGameMenuItem class.
//
//////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdlib.h>
#include "CGameMenuItem.h"
#include "CGameGraphicsInterface.h"
#include "common/CFonts.h"
#include "COptions.h"
#include "timer.h"

namespace
{
	const int KMenuItemRandomMin = 2;
	const int KMenuItemRandom = 16;
	// Pienempi on kovempi
	const int KMenuItemVaimennus = 3;
	const int KMenuItemDisplaceDelay = 0;
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TMenuKeyActions CGameMenuItem::HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* ggi)
{
	return EMenuKeyNotProcessed;
}

CRect<int> CGameMenuItem::Draw(CGraphicsBuffer* aDst,CGameGraphicsInterface* aGGI, int aX, int aY, int aActive)
{
	CRect<int> rect;
	iDisplaceCounter++;

	if (!iText) return rect;

	if ( !iEnabled )
	{
		return aGGI->Font(CGameGraphicsInterface::EBigMenuFontGrey)->Write(aX+MenuXDisplace(aActive),aY+MenuYDisplace(aActive),iText,CFonts::EDrawBelow, CFonts::EDrawInMiddle, aDst);
	}

	if ( aActive || iNotActiveItem )
	{
		rect = aGGI->Font(CGameGraphicsInterface::EBigMenuFont)->Write(aX+MenuXDisplace(aActive),aY+MenuYDisplace(aActive),iText,CFonts::EDrawBelow, CFonts::EDrawInMiddle, aDst);
	}
	else 
	{
		rect = aGGI->Font(CGameGraphicsInterface::EBigMenuFontDark)->Write(aX,aY,iText,CFonts::EDrawBelow, CFonts::EDrawInMiddle, aDst);
	}
	return rect;
}

int CGameMenuItem::Active()
{
	return !iNotActiveItem;
}

int CGameMenuItem::Enabled()
{
	return iEnabled;
}

void CGameMenuItem::SetText(const char* aText)
{
	free(iText);

	if (aText)
		iText=strdup(aText);
	else 
		iText=NULL;
}

CGameMenuItem::CGameMenuItem(CGameMenuBase* aParent, const char *aText): iText(NULL), iEnabled(1), iNotActiveItem(0), iParamStr(NULL), iParent(aParent)
{
	ASSERT( aParent );
	SetText(aText);

	iDisplaceTimerValue=0;
}

CGameMenuItem::~CGameMenuItem()
{
	free(iText);
	free(iParamStr);
}

void CGameMenuItem::NewDisplace()
{
	if (iDisplaceTimerValue>TimerCounter||
		iDisplaceTimerValue+KMenuItemDisplaceDelay<TimerCounter)
	{
		iDisplaceX = (rand()%(KMenuItemRandomMin*2+1)-KMenuItemRandomMin  )+(rand()%(KMenuItemRandom*4+1)-KMenuItemRandom*2)/(1+iDisplaceCounter/KMenuItemVaimennus);
		iDisplaceY = (rand()%(KMenuItemRandomMin*2+1)-KMenuItemRandomMin  )+(rand()%(KMenuItemRandom*2+1)-KMenuItemRandom  )/(1+iDisplaceCounter/KMenuItemVaimennus);
		iDisplaceTimerValue = TimerCounter;
	}
}

int CGameMenuItem::MenuXDisplace(int aActive)
{
	if (!aActive) 
		return 0;

	NewDisplace();
	return iDisplaceX;
}

int CGameMenuItem::MenuYDisplace(int aActive)
{
	if (!aActive) 
		return 0;

	NewDisplace();
	return iDisplaceY;
}

void CGameMenuItem::Activate()
{
	iDisplaceCounter=0;
}

bool CGameMenuItem::LeftKey( SDL_keysym key )
{
	if ( key.sym==SDLK_LEFT || key.sym == iParent->Container()->Options()->Data().iKeys[ 0 ].keys[EKeyLEFT]|| key.sym == iParent->Container()->Options()->Data().iKeys[ 0 ].keys[EKeyLSTRAFE] )
		return true;

	return false;
}

bool CGameMenuItem::RightKey( SDL_keysym key )
{
	if ( key.sym==SDLK_RIGHT || key.sym == iParent->Container()->Options()->Data().iKeys[ 0 ].keys[EKeyRIGHT]|| key.sym == iParent->Container()->Options()->Data().iKeys[ 0 ].keys[EKeyRSTRAFE]  )
		return true;

	return false;
}

bool CGameMenuItem::UpKey( SDL_keysym key )
{
	if ( key.sym==SDLK_UP || key.sym == iParent->Container()->Options()->Data().iKeys[ 0 ].keys[EKeyUP] )
		return true;

	return false;
}

bool CGameMenuItem::DownKey( SDL_keysym key )
{
	if ( key.sym==SDLK_DOWN || key.sym == iParent->Container()->Options()->Data().iKeys[ 0 ].keys[EKeyDOWN] )
		return true;

	return false;
}

bool CGameMenuItem::ActionKey( SDL_keysym key )
{
	if ( key.sym==SDLK_RETURN|| key.sym==SDLK_SPACE || key.sym == iParent->Container()->Options()->Data().iKeys[ 0 ].keys[EKeySHOOT] )
		return true;

	return false;
}

bool CGameMenuItem::AnyKey( SDL_keysym key )
{
	if (LeftKey(key)||RightKey(key)||ActionKey(key))
		return true;

	return false;
}
