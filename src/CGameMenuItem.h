// CGameMenuItem.h: interface for the CGameMenuItem class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __CGAMEMENUITEM_H
#define __CGAMEMENUITEM_H

#include "common/defines.h"
#include "CGameMenuBase.h"

class CGameData;
class CGameGraphicsInterface;
class CGraphicsBuffer;


class CGameMenuItem
{
public:
	CGameMenuItem(CGameMenuBase* aParent, const char *iText);
	virtual ~CGameMenuItem();
	virtual TMenuKeyActions HandleKeyPress(SDL_Keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* ggi);
	virtual CRect<int> Draw(CGraphicsBuffer* aDst,CGameGraphicsInterface* aGGI, int aX, int aY, int aActive);
	virtual int Enabled();
	virtual int Active();
	virtual void Activate();

protected:
	bool LeftKey( SDL_Keysym key );
	bool RightKey( SDL_Keysym key );
	bool UpKey( SDL_Keysym key );
	bool DownKey( SDL_Keysym key );
	bool ActionKey( SDL_Keysym key );
	bool AnyKey( SDL_Keysym key );

	virtual void SetText(const char * aText);
	virtual int MenuXDisplace(int aActive);
	virtual int MenuYDisplace(int aActive);
	virtual void NewDisplace();

	CGameMenuBase* iParent;

	int iDisplaceTimerValue;
	int iDisplaceCounter;
	int iDisplaceX;
	int iDisplaceY;
	bool iEnabled;
	bool iNotActiveItem;
	char* iText;
	char* iParamStr;
};

#endif
