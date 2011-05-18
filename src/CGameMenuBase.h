// CGameMenuBase.h: interface for the CGameMenuBase class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __GAMEMENUBASE_H_
#define __GAMEMENUBASE_H_

#include <SDL.h>
#include <vector>

#include "common/defines.h"
#include "common/CRect.h"
#include "common/CDrawArea.h"

#include "CGameMenuContainer.h"

class CGameGraphicsInterface;
class CGraphicsBuffer;
class CGameMenuItem;
class CEventHandler;

enum TMenuKeyActions
{
	EMenuKeyProcessed=0,
	EMenuKeyExit,
	EMenuKeyNotProcessed
};

#define MENUTEXT_VERTSEPARATION 35
#define PARAM_SEPARATION 5

class CGameMenuBase
{
public:
	CGameMenuBase(CGameMenuContainer* aGMC);
	virtual ~CGameMenuBase();
	virtual bool RunMenu(CEventHandler* aEH);
	virtual void Draw(CDrawArea& aDirtyArea,CDrawArea& aDrawArea, CGameGraphicsInterface* aGGI);
	virtual void ActivateMenu();
	virtual TGameMenuState PrevMenu()=0;
	inline CGameMenuContainer* Container(){ return iGMC;};
	virtual void SetBackground(const CGraphicsBuffer* aBackground);
	virtual void ValueUpdated( CGameMenuItem* aUpdatedItem );
protected:
	virtual TMenuKeyActions HandleKeyPress(SDL_keysym aKey,CGameGraphicsInterface* aGGI);

protected:
	std::vector<CGameMenuItem*> iMenuItems;
	const CGraphicsBuffer* iBackGround;
	CGameMenuContainer* iGMC;
	int iActiveItem;
	CDrawArea iCleanArea;
};

#endif
