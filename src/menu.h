#ifndef __MENU_H
#define __MENU_H

#include "common/defines.h"
#include "CGameMenuBase.h"

class CGraphicsBuffer;
class CGameGraphicsInterface;
class CGameMenuBase;
class CKeys;
class CGameApp;

/***************************************************\
*	Class CGameMenuOptions							*
\***************************************************/
class CGameMenuOptions: public CGameMenuBase
{
public:
	CGameMenuOptions(CGameMenuContainer* aGMC);
	TGameMenuState PrevMenu();
};

/***************************************************\
*	Class CGameMenuGameOptions						*
\***************************************************/
class CGameMenuGameOptions: public CGameMenuBase
{
public:
	CGameMenuGameOptions(CGameMenuContainer* aGMC);
	TGameMenuState PrevMenu();
	void ValueUpdated( CGameMenuItem* aUpdatedItem );
private:
#ifndef DISABLE_GAMMA
	CGameMenuItem* iGamma;
#endif
};

/***************************************************\
*	Class CGameMenuPlayerOptions						*
\***************************************************/
class CGameMenuPlayerOptions: public CGameMenuBase
{
public:
	CGameMenuPlayerOptions(int playerno,CGameMenuContainer* aGMC);
	TGameMenuState PrevMenu();
};

/***************************************************\
*	Class CGameMenuDeathMatchOptions					*
\***************************************************/
class CGameMenuDeathMatchOptions: public CGameMenuBase
{
public:
	CGameMenuDeathMatchOptions(CGameMenuContainer* aGMC);
	TGameMenuState PrevMenu();
};

/***************************************************\
*	Class CGameMenuAudioOptions						*
\***************************************************/

class CGameMenuAudioOptions: public CGameMenuBase
{
public:
	CGameMenuAudioOptions(CGameMenuContainer* aGMC);
	TGameMenuState PrevMenu();
	void ValueUpdated( CGameMenuItem* aUpdatedItem );

protected:
	TMenuKeyActions HandleKeyPress(SDL_keysym aKey,CGameGraphicsInterface* aGGI);
	CGameMenuItem* iMusicVolume;
	CGameMenuItem* iFXVolume;
};
/***************************************************\
*	Class CGameMenuEpisodeSelect						*
\***************************************************/

class CGameMenuEpisodeSelect: public CGameMenuBase
{
public:
	CGameMenuEpisodeSelect(CGameMenuContainer* aGMC);
	TGameMenuState PrevMenu();
	TMenuKeyActions HandleKeyPress(SDL_keysym aKey,CGameGraphicsInterface* aGGI);

protected:
	int iSubScroll;
	int iSubItem;
	int iItemsTotal;
};

/***************************************************\
*	Class CGameMenuMain								*
\***************************************************/

class CGameMenuMain: public CGameMenuBase
{
public:
	CGameMenuMain(CGameMenuContainer* aGMC);
	TGameMenuState PrevMenu();
};

/***************************************************\
*	Class CGameMenuMultiplayJoin					*
\***************************************************/

class CGameMenuMultiplayJoin: public CGameMenuBase
{
public:
	CGameMenuMultiplayJoin(CGameMenuContainer* aGMC);
	TGameMenuState PrevMenu();
};


/***************************************************\
*	Class CGameMenuMultiplay							*
\***************************************************/

class CGameMenuMultiplay: public CGameMenuBase
{
public:
	CGameMenuMultiplay(CGameMenuContainer* aGMC);
	TGameMenuState PrevMenu();
};

/***************************************************\
*	Class CGameMenuChangeKeys						*
\***************************************************/
class CGameMenuChangeKeys: public CGameMenuBase
{
public:
	CGameMenuChangeKeys(int playerno,CGameMenuContainer* aGMC);
	TGameMenuState PrevMenu();
private:
	int iIndex;
};

/***************************************************\
*	Class CGameMenuCredits							*
\***************************************************/
//class CGameMenuCredits: public CGameMenuBase
//{
//public:
//	CGameMenuCredits(CGameMenuContainer* aGMC);
//	TGameMenuState PrevMenu();
//};

/***************************************************\
*	Class CGameMenuNetOptions						*
\***************************************************/
class CGameMenuNetOptions: public CGameMenuBase
{
public:
	CGameMenuNetOptions(CGameMenuContainer* aGMC);
	TGameMenuState PrevMenu();
};

#endif
