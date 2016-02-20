#include <stdlib.h>
#include <string.h>
#include <SDL.h>

#include "menu.h"

#include "COptions.h"
#include "MenuItems.h"

/***************************************************\
*	Class CGameMenuMain								*
\***************************************************/

// TODO: CONFIGFILE

CGameMenuMain::CGameMenuMain(CGameMenuContainer* aGMC): CGameMenuBase(aGMC)
{
	iMenuItems.reserve(8);
	iMenuItems.push_back(new CGameMenuItemTitle(this,"main menu"));
	iMenuItems.push_back(new CGameMenuItemTitle(this,""));
	iMenuItems.push_back(new CGameMenuItemStartMenu(this,"single game",EGameMenuStateEpisodeSelect));
	iMenuItems.push_back(new CGameMenuItemStartMenu(this,"start multiplayer game",EGameMenuStateMultiplay));
//	iMenuItems.push_back(new CGameMenuItemStartMenu(this,"join multiplayer game",EGameMenuStateMultiplay));
	iMenuItems.push_back(new CGameMenuItemStartMenu(this,"options",EGameMenuStateOptions));
	iMenuItems.push_back(new CGameMenuItemTitle(this,""));
	iMenuItems.push_back(new CGameMenuItemExit(this,"exit"));
}

TGameMenuState CGameMenuMain::PrevMenu()
{
	return EGameMenuStateQuit;
}


/***************************************************\
*	Class CGameMenuGameOptions						*
\***************************************************/

void CGameMenuGameOptions::ValueUpdated( CGameMenuItem* aUpdatedItem )
{
	if ( aUpdatedItem == iGamma )
	{
		iGMC->Options()->UpdateGammaChanges();
	}
}

CGameMenuGameOptions::CGameMenuGameOptions(CGameMenuContainer* aGMC): CGameMenuBase(aGMC)
{
	COptions* options=aGMC->Options();

	iGamma = new CGameMenuItemPercent(this,"gamma: ",&options->Data().iGamma,KMinGamma,KMaxGamma,KGammaStep);
	iMenuItems.reserve(9);
	iMenuItems.push_back(new CGameMenuItemTitle(this,"game options"));
	iMenuItems.push_back(new CGameMenuItemTitle(this,""));
	iMenuItems.push_back(new CGameMenuItemMouseMode(this,"mouse controls: "));
	iMenuItems.push_back(new CGameMenuItemPercent(this,"mouse sensitivity: ",&options->Data().iMouseSensitivity,KMouseSensitivityMin,KMouseSensitivityMax,KMouseSensitivityStep));
	iMenuItems.push_back( iGamma );

	iMenuItems.push_back(new CGameMenuItemResolution(this,"game resolution: ",&options->Data().iInGameVideoMode ));
	iMenuItems.push_back(new CGameMenuItemFullScreen(this,"fullscreen",&options->Data().iFullScreen));
	iMenuItems.push_back(new CGameMenuItemTitle(this,""));
	iMenuItems.push_back(new CGameMenuItemExit(this,"previous menu"));
}

TGameMenuState CGameMenuGameOptions::PrevMenu()
{
	return EGameMenuStateOptions;
}


/***************************************************\
*	Class CGameMenuDeathMatchOptions					*
\***************************************************/

CGameMenuDeathMatchOptions::CGameMenuDeathMatchOptions(CGameMenuContainer* aGMC): CGameMenuBase(aGMC)
{
	COptions* options=aGMC->Options();

	iMenuItems.reserve(6);
	iMenuItems.push_back(new CGameMenuItemTitle(this,"deathmatch options"));
	iMenuItems.push_back(new CGameMenuItemTitle(this,""));
	iMenuItems.push_back(new CGameMenuItemPercent(this,"moving speed",&options->Data().iDMSpeed,0.50f,5.0f,0.1f));
	iMenuItems.push_back(new CGameMenuItemBoolean(this,"enemies",&options->Data().iEnemiesOnGame));
	iMenuItems.push_back(new CGameMenuItemBoolean(this,"players always have target system",&options->Data().iDMTargetSystem));
	iMenuItems.push_back(new CGameMenuItemTitle(this,""));
	iMenuItems.push_back(new CGameMenuItemExit(this,"previous menu"));
}


TGameMenuState CGameMenuDeathMatchOptions::PrevMenu()
{
	return EGameMenuStateOptions;
}

/***************************************************\
*	Class CGameMenuAudioOptions						*
\***************************************************/

void CGameMenuAudioOptions::ValueUpdated( CGameMenuItem* aUpdatedItem )
{
	if ( aUpdatedItem == iMusicVolume || aUpdatedItem == iFXVolume )
	{
		iGMC->Options()->UpdateAudioChanges();
	}
}

CGameMenuAudioOptions::CGameMenuAudioOptions(CGameMenuContainer* aGMC): CGameMenuBase(aGMC)
{
	COptions* options=aGMC->Options();
	iMenuItems.reserve(8);
	iMusicVolume=new CGameMenuItemPercent(this,"music volume",&options->Data().iMusicVolume,KMusicVolumeMin,KMusicVolumeMax,KMusicVolumeStep);
	iFXVolume=new CGameMenuItemPercent(this,"effects volume",&options->Data().iEffectsVolume,KEffectVolumeMin,KEffectVolumeMax,KEffectVolumeStep);

	iMenuItems.push_back(new CGameMenuItemTitle(this,"audio options"));
	iMenuItems.push_back(new CGameMenuItemTitle(this,""));
	iMenuItems.push_back(new CGameMenuItemMusicTheme(this,"music theme:"));
	iMenuItems.push_back(iMusicVolume);
	iMenuItems.push_back(iFXVolume);
	iMenuItems.push_back(new CGameMenuItemBoolean(this,"reverse stereo",&options->Data().iStereo));
	iMenuItems.push_back(new CGameMenuItemTitle(this,""));
	iMenuItems.push_back(new CGameMenuItemExit(this,"previous menu"));
}

TMenuKeyActions CGameMenuAudioOptions::HandleKeyPress(SDL_keysym aKey,CGameGraphicsInterface* aGGI)
{
	COptions* options=iGMC->Options();

	return CGameMenuBase::HandleKeyPress(aKey,aGGI);
	options->UpdateAudioChanges();
}


TGameMenuState CGameMenuAudioOptions::PrevMenu()
{
	return EGameMenuStateOptions;
}

/***************************************************\
*	Class CGameMenuOptions							*
\***************************************************/

CGameMenuOptions::CGameMenuOptions(CGameMenuContainer* aGMC): CGameMenuBase(aGMC)
{
	iMenuItems.reserve(13);
	iMenuItems.push_back(new CGameMenuItemTitle(this,"options"));
	iMenuItems.push_back(new CGameMenuItemTitle(this,""));
	iMenuItems.push_back(new CGameMenuItemStartMenu(this,"game options",EGameMenuStateGameOptions));
	iMenuItems.push_back(new CGameMenuItemStartMenu(this,"network options",EGameMenuStateNetOptions));
	iMenuItems.push_back(new CGameMenuItemStartMenu(this,"player 1 options",EGameMenuStatePlayerOptions1));
	iMenuItems.push_back(new CGameMenuItemStartMenu(this,"player 2 options",EGameMenuStatePlayerOptions2));
	iMenuItems.push_back(new CGameMenuItemStartMenu(this,"player 3 options",EGameMenuStatePlayerOptions3));
	iMenuItems.push_back(new CGameMenuItemStartMenu(this,"player 4 options",EGameMenuStatePlayerOptions4));
	iMenuItems.push_back(new CGameMenuItemStartMenu(this,"deathmatch options",EGameMenuStateDeathMatchOptions));
	iMenuItems.push_back(new CGameMenuItemStartMenu(this,"audio options",EGameMenuStateAudioOptions));
	iMenuItems.push_back(new CGameMenuItemResetOptions(this,"reset options to default"));
	iMenuItems.push_back(new CGameMenuItemTitle(this,""));
	iMenuItems.push_back(new CGameMenuItemExit(this,"previous menu"));
}


TGameMenuState CGameMenuOptions::PrevMenu()
{
	return EGameMenuStateMain;
}
/***************************************************\
*	Class CGameMenuMultiplayJoin							*
\***************************************************/

CGameMenuMultiplayJoin::CGameMenuMultiplayJoin(CGameMenuContainer* aGMC): CGameMenuBase(aGMC)
{
	iMenuItems.reserve(3);
	iMenuItems.push_back(new CGameMenuItemTitle(this,"Join network game"));
	iMenuItems.push_back(new CGameMenuItemTitle(this,""));
	iMenuItems.push_back(new CGameMenuItemExit(this,"previous menu"));
}


TGameMenuState CGameMenuMultiplayJoin::PrevMenu()
{
	return EGameMenuStateMain;
}

/***************************************************\
*	Class CGameMenuMultiplay							*
\***************************************************/

CGameMenuMultiplay::CGameMenuMultiplay(CGameMenuContainer* aGMC): CGameMenuBase(aGMC)
{
	CGameMenuItemLevelSelect* levSelect=new CGameMenuItemLevelSelect(this,"Level");
	CGameMenuItemSplitSelect* splitSelect=new CGameMenuItemSplitSelect(this,"Split screen");
	CGameMenuItemEpisodeSelect* epiSelect=new CGameMenuItemEpisodeSelect(this,"Episode",levSelect);

	iMenuItems.reserve(10);
// TODO: temporary. remove
//	CGameMenuItemNetworkModeSelect* netSelect=new CGameMenuItemNetworkModeSelect(this,"Network");
//	splitSelect->SetNetworkSelect(netSelect);
//	netSelect->SetSplitSelect(splitSelect);

	iMenuItems.push_back(new CGameMenuItemTitle(this,"Start multiplay"));
	iMenuItems.push_back(new CGameMenuItemTitle(this,""));
	iMenuItems.push_back(epiSelect);
	iMenuItems.push_back(levSelect);
	iMenuItems.push_back(new CGameMenuItemGameModeSelect(this,"Game mode", levSelect, epiSelect));

	iMenuItems.push_back(splitSelect);
//	iMenuItems.push_back(netSelect);
	iMenuItems.push_back(new CGameMenuItemTitle(this,""));
	iMenuItems.push_back(new CGameMenuItemExecMultiplay(this,"Go!"));
	iMenuItems.push_back(new CGameMenuItemTitle(this,""));
	iMenuItems.push_back(new CGameMenuItemExit(this,"previous menu"));
}


TGameMenuState CGameMenuMultiplay::PrevMenu()
{
	return EGameMenuStateMain;
}
/***************************************************\
*	Class CGameMenuEpisodeSelect						*
\***************************************************/

const int KMaxEpisodeInList = 8;

CGameMenuEpisodeSelect::CGameMenuEpisodeSelect(CGameMenuContainer* aGMC): CGameMenuBase(aGMC)
{
	CEpisodeList* episodelist=aGMC->Options()->EpisodeList();
	int amount;
	iItemsTotal = episodelist->Amount( false );
	amount = iItemsTotal;
	iSubScroll=0;
	iSubItem=0;

	ASSERT(amount>=0);
	ASSERT(iItemsTotal>=0);

	if (amount>KMaxEpisodeInList) 
		amount=KMaxEpisodeInList;

	iMenuItems.reserve( 2 + amount );

	iMenuItems.push_back(new CGameMenuItemTitle(this,"Choose episode"));
	iMenuItems.push_back(new CGameMenuItemTitle(this,""));


	for (unsigned int a=0;a<amount;a++)
		iMenuItems.push_back(new CGameMenuItemExecSingleGame(this,episodelist->Episode(a, false )->Name(),a));
}


TGameMenuState CGameMenuEpisodeSelect::PrevMenu()
{
	return EGameMenuStateMain;
}


TMenuKeyActions CGameMenuEpisodeSelect::HandleKeyPress(SDL_keysym aKey,CGameGraphicsInterface* aGGI)
{
	CKeys& pl1keys = iGMC->Options()->Data().iKeys[ 0 ];
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
		iSubItem++;
		if (iSubItem>=iItemsTotal)
		{
			iSubItem=0;
			iSubScroll=0;
		}

		if (iSubItem-iSubScroll>=iMenuItems.size()-2)
			iSubScroll=iSubItem-(iMenuItems.size()-2)+1;
	} else
		if (aKey.sym==SDLK_UP||
		aKey.sym==pl1keys.keys[ EKeyUP ])
		{
			iSubItem--;
			if (iSubItem<0)
			{
				iSubItem+=iItemsTotal;
				iSubScroll=iSubItem-(iMenuItems.size()-2)+1;
			}

			if (iSubItem-iSubScroll<0)
				iSubScroll=iSubItem;
		} else
			return EMenuKeyNotProcessed;

	iActiveItem=2+iSubItem-iSubScroll;

	if (aKey.sym==SDLK_UP||aKey.sym==SDLK_DOWN)
		iMenuItems[iActiveItem]->Activate();


	for (int a=0;a<iMenuItems.size()-2;a++)
	{
		CEpisodeList* episodelist=iGMC->Options()->EpisodeList();
		static_cast<CGameMenuItemExecSingleGame*>(iMenuItems[a+2])->SetEpisodeNo(a+iSubScroll);
		static_cast<CGameMenuItemExecSingleGame*>(iMenuItems[a+2])->SetEpisodeName(episodelist->Episode(a+iSubScroll, false )->Name());
	}

	return EMenuKeyProcessed;
}

/***************************************************\
*	Class CGameMenuChangeKeys						*
\***************************************************/

CGameMenuChangeKeys::CGameMenuChangeKeys(int playerno,CGameMenuContainer* aGMC): CGameMenuBase(aGMC)
{
	COptions* options=aGMC->Options();
	char str[100];
	ASSERT(EF_SNPRINTF(str,100,"Change Player %d Keys",playerno)>0);
	CKeys* kptr;

	iIndex=playerno;
	
	ASSERT(playerno>0&&playerno<=KMaxLocalPlayers);

	kptr=&options->Data().iKeys[playerno-1];

	iMenuItems.reserve( 4 + EKeyAmount );
		
	iMenuItems.push_back(new CGameMenuItemTitle(this,str));
	iMenuItems.push_back(new CGameMenuItemTitle(this,""));

	for (int a=0;a<EKeyAmount;a++)
	{
		iMenuItems.push_back(new CGameMenuItemChangeKey(this,kptr,a));
	};

	iMenuItems.push_back(new CGameMenuItemTitle(this,""));
	iMenuItems.push_back(new CGameMenuItemExit(this,"previous menu"));
}


TGameMenuState CGameMenuChangeKeys::PrevMenu()
{
	switch (iIndex)
	{
		case 1: return EGameMenuStatePlayerOptions1;
		case 2: return EGameMenuStatePlayerOptions2;
		case 3: return EGameMenuStatePlayerOptions3;
		case 4: return EGameMenuStatePlayerOptions4;
		default: throw CCriticalException("CGameMenuChangeKeys::PrevMenu(): Unknown index");
	}
}

/***************************************************\
*	Class CGameMenuPlayerOptions						*
\***************************************************/

CGameMenuPlayerOptions::CGameMenuPlayerOptions(int playerno,CGameMenuContainer* aGMC): CGameMenuBase(aGMC)
{
	COptions* options=aGMC->Options();
	char str[100];

	ASSERT(EF_SNPRINTF(str,100,"Player %d options",playerno)>0);
	ASSERT(playerno>=1&&playerno<=KMaxLocalPlayers);

	iMenuItems.push_back(new CGameMenuItemTitle(this,str));
	iMenuItems.push_back(new CGameMenuItemTitle(this,""));

	switch(playerno)
	{
		case 1: iMenuItems.push_back(new CGameMenuItemStartMenu(this,"define keys",EGameMenuStateChangeKeys1));break;
		case 2: iMenuItems.push_back(new CGameMenuItemStartMenu(this,"define keys",EGameMenuStateChangeKeys2));break;
		case 3: iMenuItems.push_back(new CGameMenuItemStartMenu(this,"define keys",EGameMenuStateChangeKeys3));break;
		case 4: iMenuItems.push_back(new CGameMenuItemStartMenu(this,"define keys",EGameMenuStateChangeKeys4));break;
		default: ASSERT(0);
	}

	iMenuItems.push_back(new CGameMenuItemString(this,"change name: ",options->Data().iName[playerno-1],KPlayerNameMaxLength));
	iMenuItems.push_back(new CGameMenuItemTitle(this,""));
	iMenuItems.push_back(new CGameMenuItemExit(this,"previous menu"));
}


TGameMenuState CGameMenuPlayerOptions::PrevMenu()
{
	return EGameMenuStateOptions;
}

/***************************************************\
*	Class CGameMenuCredits							*
\***************************************************/

//CGameMenuCredits::CGameMenuCredits(CGameMenuContainer* aGMC): CGameMenuBase(aGMC)
//{
//	iMenuItems.push_back(new CGameMenuItemTitle(this,"Credits"));
//	iMenuItems.push_back(new CGameMenuItemTitle(this,""));
//	iMenuItems.push_back(new CGameMenuItemTitle(this,"Coding: Allu, Unski"));
//	iMenuItems.push_back(new CGameMenuItemTitle(this,"Graphics: Allu"));
//	iMenuItems.push_back(new CGameMenuItemTitle(this,"Music: JDruid, Raipe, Timo, Mika"));
//	iMenuItems.push_back(new CGameMenuItemTitle(this,""));
//	iMenuItems.push_back(new CGameMenuItemExit(this,"exit"));
//}
//
//TGameMenuState CGameMenuCredits::PrevMenu()
//{
//	return EGameMenuStateMain;
//}

/***************************************************\
*	Class CGameMenuNetOptions						*
\***************************************************/
CGameMenuNetOptions::CGameMenuNetOptions(CGameMenuContainer* aGMC): CGameMenuBase(aGMC)
{
	iMenuItems.reserve(8);
	iMenuItems.push_back(new CGameMenuItemTitle(this,"Network Options"));
	iMenuItems.push_back(new CGameMenuItemTitle(this,""));
	iMenuItems.push_back(new CGameMenuItemBoolean(this,"Check Newest TK from Internet",&(iGMC->Options()->Data().iWebVersionCheckingEnabled)));
	iMenuItems.push_back(new CGameMenuItemBoolean(this,"Use HTTP Proxy",&(iGMC->Options()->Data().iProxyEnabled)));
	iMenuItems.push_back(new CGameMenuItemString(this,"HTTP Proxy addr: ",iGMC->Options()->Data().iProxyAddr,DNSNAME_LENGTH));
	iMenuItems.push_back(new CGameMenuItemString(this,"HTTP Proxy port: ",iGMC->Options()->Data().iProxyPort,PROXYPORT_LENGTH));
	iMenuItems.push_back(new CGameMenuItemTitle(this,""));
	iMenuItems.push_back(new CGameMenuItemExit(this,"exit"));
}

TGameMenuState CGameMenuNetOptions::PrevMenu()
{
	return EGameMenuStateOptions;
}

