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
	virtual TMenuKeyActions HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* ggi);
	virtual CRect<int> Draw(CGraphicsBuffer* aDst,CGameGraphicsInterface* aGGI, int aX, int aY, int aActive);
	virtual int Enabled();
	virtual int Active();
	virtual void Activate();

protected:
	bool LeftKey( SDL_keysym key );
	bool RightKey( SDL_keysym key );
	bool UpKey( SDL_keysym key );
	bool DownKey( SDL_keysym key );
	bool ActionKey( SDL_keysym key );
	bool AnyKey( SDL_keysym key );

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
