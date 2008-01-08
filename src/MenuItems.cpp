#include <string.h>
#include "common/CEventHandler.h"
#include "common/CFonts.h"

#include "MenuItems.h"
#include "CGameGraphicsInterface.h"
#include "texts.h"
#include "IGameSubStateController.h"
#include "IGUIStateController.h"
#include "CMessageBoard.h"
#include "IGameMode.h"
// TODO: CONFIGFILE
const char* KKeySelectionText  = "*Press a key*";

/***************************************************\
*	Class CGameMenuItemStartMenu						*
\***************************************************/

CGameMenuItemStartMenu::~CGameMenuItemStartMenu()
{
}

CGameMenuItemStartMenu::CGameMenuItemStartMenu(CGameMenuBase* aParent, const char *aStr,TGameMenuState aState): CGameMenuItem(aParent, aStr), iNextState(aState)
{
}

TMenuKeyActions CGameMenuItemStartMenu::HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu, CGameGraphicsInterface* aGGI)
{
	if (ActionKey( key ))
	{
		iParent->Container()->ChangeState(iNextState);
		return EMenuKeyProcessed;
	}
	return EMenuKeyNotProcessed;
}

/***************************************************\
*	Class CGameMenuItemExit							*
\***************************************************/


CGameMenuItemExit::CGameMenuItemExit(CGameMenuBase* aParent,const char *aStr): CGameMenuItem(aParent, aStr)
{
}


TMenuKeyActions CGameMenuItemExit::HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI)
{
	if (ActionKey( key ))
		return EMenuKeyExit;

	return EMenuKeyNotProcessed;
}

/***************************************************\
*	Class CGameMenuItemResetOptions							*
\***************************************************/


CGameMenuItemResetOptions::CGameMenuItemResetOptions(CGameMenuBase* aParent,const char *aStr): CGameMenuItem(aParent, aStr)
{
}

TMenuKeyActions CGameMenuItemResetOptions::HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI)
{
	if (ActionKey( key ))
	{
		aMenu->Container()->Options()->Reset();
		aMenu->Container()->Options()->Save();
		aMenu->Container()->StateController()->MessageBoard()->AddMessage("Options reset!");
		return EMenuKeyProcessed;
	}

	return EMenuKeyNotProcessed;
}

/***************************************************\
*	Class CGameMenuItemExecSingleGame				*
\***************************************************/


CGameMenuItemExecSingleGame::CGameMenuItemExecSingleGame(CGameMenuBase* aParent,const char *aStr, int aEpisNo): CGameMenuItem(aParent, aStr), iEpisodeNumber(aEpisNo)
{
}


TMenuKeyActions CGameMenuItemExecSingleGame::HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI)
{
	if (ActionKey( key ))
	{
		iParent->Container()->StateController()->GameSystem()->Init(
			iParent->Container()->StateController()->GameData()->GameMode( 0 ),ENetworkModeNone,1,iEpisodeNumber, iParent->Container()->StateController()->DebugStartLevel() );

		// we return exit and we don't want to redraw new menu before starting game (looks ugly)
		aMenu->Container()->SkipDrawOnce(); 
		return EMenuKeyExit;
	}

	return EMenuKeyNotProcessed;
}

void CGameMenuItemExecSingleGame::SetEpisodeNo(int a)
{
	iEpisodeNumber=a;
}

void CGameMenuItemExecSingleGame::SetEpisodeName(const char* str)
{
	SetText(str);
}

/***************************************************\
*	Class CGameMenuItemExecMultiplay				*
\***************************************************/


CGameMenuItemExecMultiplay::CGameMenuItemExecMultiplay(CGameMenuBase* aParent,const char *aStr): CGameMenuItem(aParent, aStr)
{
}


TMenuKeyActions CGameMenuItemExecMultiplay::HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI)
{
	if (ActionKey( key ))
	{
		COptionsData& opt=aMenu->Container()->Options()->Data();

		iParent->Container()->StateController()->GameSystem()->Init(
			iParent->Container()->StateController()->GameData()->GameMode( opt.iMultiPlayGameMode ),
			opt.iMultiPlayNetworkMode,
			opt.iMultiPlayers,
			opt.iMultiPlayEpisode,
			opt.iMultiPlayLevel);

		aMenu->Container()->SkipDrawOnce(); 
		return EMenuKeyExit;
	}

	return EMenuKeyNotProcessed;
}

/***************************************************\
*	Class CGameMenuItemTitle						*
\***************************************************/

CGameMenuItemTitle::CGameMenuItemTitle(CGameMenuBase* aParent,const char *aStr): CGameMenuItem(aParent, aStr)
{
	iNotActiveItem=1;
	strupr(iText);
}

/***************************************************\
*	Class CGameMenuItemNetworkModeSelect			*
\***************************************************/

void CGameMenuItemNetworkModeSelect::SetSplitSelect(CGameMenuItemSplitSelect* aSplitSelect)
{
	iSplitSelect = aSplitSelect;
}


CGameMenuItemNetworkModeSelect::CGameMenuItemNetworkModeSelect(CGameMenuBase* aParent,const char *aStr): CGameMenuItem(aParent,NULL)
{
	iSplitSelect = NULL;
	iParamStr=strdup(aStr);
	if (iParent->Container()->Options()->Data().iMultiPlayNetworkMode == ENetworkModeClient)
		iParent->Container()->Options()->Data().iMultiPlayNetworkMode=ENetworkModeServer;

	UpdateText();
}

void CGameMenuItemNetworkModeSelect::UpdateText()
{
    char *tmp;
	tmp=(char*)malloc(strlen(iParamStr)+20);
	switch (iParent->Container()->Options()->Data().iMultiPlayNetworkMode)
	{
		case ENetworkModeNone:ASSERT(EF_SNPRINTF(tmp,strlen(iParamStr)+20,"%s: disabled",iParamStr)>0);
			if (iSplitSelect)
				iSplitSelect->EnableSplit();
			break;
		case ENetworkModeServer:ASSERT(EF_SNPRINTF(tmp,strlen(iParamStr)+20,"%s: server",iParamStr)>0);break;
		default:error("CGameMenuItemNetworkModeSelect::UpdateText: Some kind of a traditional fuck-up.");
	};
	SetText(tmp);
	free(tmp);
}

TMenuKeyActions CGameMenuItemNetworkModeSelect::HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI)
{
	if (AnyKey( key ))
	{
		if (iParent->Container()->Options()->Data().iMultiPlayNetworkMode==ENetworkModeNone)
			iParent->Container()->Options()->Data().iMultiPlayNetworkMode=ENetworkModeServer;
		else 
			iParent->Container()->Options()->Data().iMultiPlayNetworkMode=ENetworkModeNone;

		UpdateText();

		return EMenuKeyProcessed;
	}
	return EMenuKeyNotProcessed;
}

CRect<int> CGameMenuItemNetworkModeSelect::Draw(CGraphicsBuffer* aDst,CGameGraphicsInterface* aGGI, int aX, int aY, int aActive)
{
	return CGameMenuItem::Draw(aDst,aGGI,aX,aY,aActive);
}

void CGameMenuItemNetworkModeSelect::EnableNetwork()
{
	if (iParent->Container()->Options()->Data().iMultiPlayNetworkMode==ENetworkModeNone)
	{
		iParent->Container()->Options()->Data().iMultiPlayNetworkMode=ENetworkModeServer;
		UpdateText();
		iParent->Container()->DoFullRedraw();
	}
}


/***************************************************\
*	Class CGameMenuItemGameModeSelect				*
\***************************************************/


CGameMenuItemGameModeSelect::CGameMenuItemGameModeSelect(CGameMenuBase* aParent,const char *aStr, CGameMenuItemLevelSelect* aLevelSelect, CGameMenuItemEpisodeSelect* aEpisodeSelect ): CGameMenuItem(aParent,NULL), iLevelSelect( aLevelSelect ), iEpisodeSelect( aEpisodeSelect )
{
	iParamStr=strdup(aStr);
	UpdateText();
	UpdateLevelSelect();
}

void CGameMenuItemGameModeSelect::UpdateText()
{
    char *tmp;
	tmp=(char*)malloc(strlen(iParamStr)+20);
	
	ASSERT(EF_SNPRINTF(tmp,strlen(iParamStr)+20,"%s: %s",iParamStr,
		iParent->Container()->StateController()->GameData()->GameMode(
			iParent->Container()->Options()->Data().iMultiPlayGameMode
			)->Name())>0);

	SetText(tmp);
	free(tmp);
}

void CGameMenuItemGameModeSelect::UpdateLevelSelect()
{
	CGameData* gm = iParent->Container()->StateController()->GameData();
	COptions* opt = iParent->Container()->Options();
	if ( opt->Data().iMultiPlayEpisode < 0 )
	{
		opt->Data().iMultiPlayEpisode=0;
	}
	iLevelSelect->UpdateText();
	iEpisodeSelect->UpdateText();
	iParent->Container()->DoFullRedraw();

	if (gm->GameMode(opt->Data().iMultiPlayGameMode)->StartFromAnyLevel())
	{
		iLevelSelect->SetEnabled( true );
	}
	else
	{
		iLevelSelect->SetEnabled( false );
		iLevelSelect->ResetLevel();
	}

}

TMenuKeyActions CGameMenuItemGameModeSelect::HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI)
{
	CGameData* gm = iParent->Container()->StateController()->GameData();
	COptions* opt = iParent->Container()->Options();
	bool ok = false;
	int prev = opt->Data().iMultiPlayGameMode;
	
	if (LeftKey( key ))
	{
        opt->Data().iMultiPlayGameMode++;
		ok=true;
	}

	if (RightKey( key ))
	{
		opt->Data().iMultiPlayGameMode--;
		ok=true;
	}

	if (ok)
	{
		if (opt->Data().iMultiPlayGameMode >= gm->GameModes())
			opt->Data().iMultiPlayGameMode = 0;
		if (opt->Data().iMultiPlayGameMode <0 )
			opt->Data().iMultiPlayGameMode = gm->GameModes()-1;

		if (gm->GameMode( prev )->DMEpisodes() ||
			!gm->GameMode( opt->Data().iMultiPlayGameMode )->DMEpisodes())
			opt->Data().iMultiPlayEpisode--;

		if (!gm->GameMode( prev )->DMEpisodes() ||
			gm->GameMode( opt->Data().iMultiPlayGameMode )->DMEpisodes())
			opt->Data().iMultiPlayEpisode++;

		UpdateLevelSelect();
		return EMenuKeyProcessed;
	}

	return EMenuKeyNotProcessed;
}

CRect<int> CGameMenuItemGameModeSelect::Draw(CGraphicsBuffer* aDst,CGameGraphicsInterface* aGGI, int aX, int aY, int aActive)
{
	UpdateText();
	return CGameMenuItem::Draw(aDst,aGGI,aX,aY,aActive);
}

/***************************************************\
*	Class CGameMenuItemSplitSelect					*
\***************************************************/

void CGameMenuItemSplitSelect::SetNetworkSelect(CGameMenuItemNetworkModeSelect* aNetSelect)
{
	iNetSelect = aNetSelect;
}

CGameMenuItemSplitSelect::CGameMenuItemSplitSelect(CGameMenuBase* aParent,const char *aStr): CGameMenuItem(aParent,NULL)
{
	iNetSelect = NULL;
	iParamStr=strdup(aStr);
	UpdateText();
}

void CGameMenuItemSplitSelect::UpdateText()
{
	char *tmp;
	tmp=(char*)malloc(strlen(iParamStr)+20);

	if (iParent->Container()->Options()->Data().iMultiPlayers<1||
		iParent->Container()->Options()->Data().iMultiPlayers>4)
		iParent->Container()->Options()->Data().iMultiPlayers=1;
	if ( !iNetSelect && iParent->Container()->Options()->Data().iMultiPlayers == 1 )
		iParent->Container()->Options()->Data().iMultiPlayers = 2;

	switch (iParent->Container()->Options()->Data().iMultiPlayers)
	{
		case 1:ASSERT(EF_SNPRINTF(tmp,strlen(iParamStr)+20,"%s: disabled",iParamStr)>0);
			ASSERT( iNetSelect );
			iNetSelect->EnableNetwork();
			break;
		case 2:
		case 3:
		case 4:ASSERT(EF_SNPRINTF(tmp,strlen(iParamStr)+20,"%s: %d players",iParamStr,iParent->Container()->Options()->Data().iMultiPlayers)>0);break;
		default:error("CGameMenuItemSplitSelect::UpdateText: Too many players...");
	}

	SetText(tmp);
	free(tmp);
}

TMenuKeyActions CGameMenuItemSplitSelect::HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI)
{

	if (ActionKey(key) || RightKey( key))
	{
		iParent->Container()->Options()->Data().iMultiPlayers++;
		if (iParent->Container()->Options()->Data().iMultiPlayers>KMaxLocalPlayers)
			iParent->Container()->Options()->Data().iMultiPlayers=1;
		return EMenuKeyProcessed;
	}
	if (LeftKey( key ))
	{
		iParent->Container()->Options()->Data().iMultiPlayers--;
		if (iParent->Container()->Options()->Data().iMultiPlayers<1)
			iParent->Container()->Options()->Data().iMultiPlayers=KMaxLocalPlayers;
		return EMenuKeyProcessed;
	}

	return EMenuKeyNotProcessed;
}

void CGameMenuItemSplitSelect::EnableSplit()
{
	if (iParent->Container()->Options()->Data().iMultiPlayers<2)
	{
		iParent->Container()->Options()->Data().iMultiPlayers=2;
		UpdateText();
		iParent->Container()->DoFullRedraw();
	}
}

CRect<int> CGameMenuItemSplitSelect::Draw(CGraphicsBuffer* aDst,CGameGraphicsInterface* aGGI, int aX, int aY, int aActive)
{
	UpdateText();
	return CGameMenuItem::Draw(aDst,aGGI,aX,aY,aActive);
}
/***************************************************\
*	Class CGameMenuItemBoolean						*
\***************************************************/


CGameMenuItemBoolean::CGameMenuItemBoolean(CGameMenuBase* aParent,const char *aStr, bool* aBool): CGameMenuItem(aParent,NULL)
{
	iParamStr=strdup(aStr);
	iBool=aBool;
	UpdateText();
}

void CGameMenuItemBoolean::UpdateText()
{
	char *tmp;
	tmp=(char*)malloc(strlen(iParamStr)+KBoolCatMax+2);
	ASSERT(EF_SNPRINTF(tmp,strlen(iParamStr)+KBoolCatMax+2,"%s %s",iParamStr,KBoolCatStr[(*iBool)? 1:0])>0);
	SetText(tmp);
	free(tmp);
}

TMenuKeyActions CGameMenuItemBoolean::HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI)
{

	if (AnyKey( key ))
	{
		(*iBool)=!(*iBool);
		return EMenuKeyProcessed;
	}

	return EMenuKeyNotProcessed;
}

CRect<int> CGameMenuItemBoolean::Draw(CGraphicsBuffer* aDst,CGameGraphicsInterface* aGGI, int aX, int aY, int aActive)
{
	UpdateText();
	return CGameMenuItem::Draw(aDst,aGGI,aX,aY,aActive);
}

/***************************************************\
*	Class CGameMenuItemFullScreen						*
\***************************************************/
CGameMenuItemFullScreen::CGameMenuItemFullScreen(CGameMenuBase* aParent,const char *aStr, bool* aBool): CGameMenuItemBoolean(aParent,aStr,aBool)
{
}

TMenuKeyActions CGameMenuItemFullScreen::HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI)
{
	bool booli=*iBool;
	TMenuKeyActions retval;

	retval=CGameMenuItemBoolean::HandleKeyPress(key,aMenu,aGGI);
	if (*iBool != booli) 
		aGGI->UpdateMode();
	return retval;
}


/***************************************************\
*	Class CGameMenuItemResolution						*
\***************************************************/

CGameMenuItemResolution::CGameMenuItemResolution(CGameMenuBase* aParent, const char *aStr, CCoord<int>* aGameMode) : CGameMenuItem( aParent, NULL )
{
	iParamStr = strdup( aStr );
	iGGI = iParent->Container()->StateController()->GGI();
	iGameMode = aGameMode;
	UpdateText();
}

TMenuKeyActions CGameMenuItemResolution::HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI)
{
	COptions* opt = iParent->Container()->Options();

	if (ActionKey( key ) || RightKey( key ))
	{
		iGGI->GD()->NextRes( *iGameMode );
		UpdateText();
		return EMenuKeyProcessed;
	}

	if (LeftKey( key ))
	{
		iGGI->GD()->PrevRes( *iGameMode );
		UpdateText();
		return EMenuKeyProcessed;
	}
	return EMenuKeyNotProcessed;
}

void CGameMenuItemResolution::UpdateText()
{
	char tmp[ 150 ];

	ASSERT(EF_SNPRINTF(tmp,150,"%s%dx%d",iParamStr,iGameMode->X(),iGameMode->Y() )>0);
	SetText( tmp );
}

/***************************************************\
*	Class CGameMenuItemString						*
\***************************************************/


CGameMenuItemString::CGameMenuItemString(CGameMenuBase* aParent,const char *aTitleStr, char *aParamStr, unsigned int maxlen): CGameMenuItem(aParent, aTitleStr)
{
	iParamStr=aParamStr;
	iMaxLen=maxlen-1;
	iInputMode=0;
}

CGameMenuItemString::~CGameMenuItemString()
{
	iParamStr=NULL; // We don't want to free this....
}

TMenuKeyActions CGameMenuItemString::HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI)
{
	if (!iInputMode)
	{
		if (ActionKey( key ))
		{
			iInputMode=1;
			return EMenuKeyProcessed;
		}	
	}
	else
	{
		if (key.sym==SDLK_RETURN||key.sym==SDLK_ESCAPE)
		{
			iInputMode=0;
			return EMenuKeyProcessed;
		}
		if (key.sym==SDLK_BACKSPACE)
		{
			int end=(int)strlen(iParamStr);
			if (end>0)
			{
				iParamStr[end-1]=0;
				return EMenuKeyProcessed;
			}
			else return EMenuKeyNotProcessed;
		}

		if (strlen(iParamStr)<iMaxLen)
		{
			if (key.unicode==0)
				return EMenuKeyNotProcessed;

			int end=(int)strlen(iParamStr);
			iParamStr[end+1]=0;
			iParamStr[end]=(char)key.unicode;
			return EMenuKeyProcessed;
		}
	}

	return EMenuKeyNotProcessed;
}

CRect<int> CGameMenuItemString::Draw(CGraphicsBuffer* aDst,CGameGraphicsInterface* aGGI, int aX, int aY, int aActive)
{
	int xadd,yadd;
	CRect<int> rect;
	std::string str = iParamStr;

	iDisplaceCounter++;
	xadd=iInputMode?0:MenuXDisplace(aActive);
	yadd=iInputMode?0:MenuYDisplace(aActive);

	if (iInputMode && iDisplaceCounter % 100 > 50)
		str+= "|";

	if (!iText) return rect;
	if (aActive||iNotActiveItem)
	{
		rect.Combine(aGGI->Font(CGameGraphicsInterface::EBigMenuFont)->Write(aX-PARAM_SEPARATION+xadd,aY+yadd,iText, CFonts::EDrawBelow, CFonts::EDrawToLeft,aDst));
		rect.Combine(aGGI->Font(CGameGraphicsInterface::EBigMenuFont)->Write(aX+PARAM_SEPARATION+xadd,aY+yadd,str.c_str(), CFonts::EDrawBelow, CFonts::EDrawToRight,aDst));
	}
	else 
	{
		rect.Combine(aGGI->Font(CGameGraphicsInterface::EBigMenuFontDark)->Write(aX-PARAM_SEPARATION,aY,iText, CFonts::EDrawBelow, CFonts::EDrawToLeft,aDst));
		rect.Combine(aGGI->Font(CGameGraphicsInterface::EBigMenuFontDark)->Write(aX+PARAM_SEPARATION,aY,iParamStr, CFonts::EDrawBelow, CFonts::EDrawToRight,aDst));
	}

	return rect;
}

/***************************************************\
*	Class CGameMenuItemPercent						*
\***************************************************/


CGameMenuItemPercent::CGameMenuItemPercent(CGameMenuBase* aParent,const char *aStr, float* percent,float lowlimit, float highlimit, float step): CGameMenuItem(aParent,NULL)
{
	iParamStr=strdup(aStr);
	iPercent=percent;
	iLow=lowlimit;
	iHigh=highlimit;
	iStep=step;
	UpdateText();
}

void CGameMenuItemPercent::UpdateText()
{
	char tmp[100];

	if (*iPercent<iLow) (*iPercent)=iLow;
	if (*iPercent>iHigh) (*iPercent)=iHigh;
	ASSERT(EF_SNPRINTF(tmp,100,"%s %.0f%%",iParamStr,(*iPercent)*100)>0);
	SetText(tmp);
}

TMenuKeyActions CGameMenuItemPercent::HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI)
{
	if (LeftKey( key ))
	{
		(*iPercent)-=iStep;
		UpdateText();
		iParent->ValueUpdated( this );
		return EMenuKeyProcessed;
	}

	if (RightKey( key ))
	{
		(*iPercent)+=iStep;
		UpdateText();
		iParent->ValueUpdated( this );
		return EMenuKeyProcessed;
	}

	return EMenuKeyNotProcessed;
}

CRect<int> CGameMenuItemPercent::Draw(CGraphicsBuffer* aDst,CGameGraphicsInterface* aGGI, int aX, int aY, int aActive)
{
	UpdateText();
	return CGameMenuItem::Draw(aDst, aGGI, aX, aY, aActive);
}

/***************************************************\
*	Class CGameMenuItemChangeKey						*
\***************************************************/


CGameMenuItemChangeKey::CGameMenuItemChangeKey(CGameMenuBase* aParent,CKeys* kptr, int index): CGameMenuItem(aParent,k_text[index])
{
	ASSERT( kptr );
	iKeys=kptr;
	iIndex=index;
	iKeyInputMode=0;
}

TMenuKeyActions CGameMenuItemChangeKey::HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI)
{
	if (!iKeyInputMode)
	{
		if (ActionKey( key ))
		{
			iKeyInputMode=1;
			return EMenuKeyProcessed;
		}
		if (key.sym==SDLK_BACKSPACE)
		{
			iKeys->keys[iIndex]=0;
			return EMenuKeyProcessed;
		}
	}
	else
	{
		for (int a=0;a<KMaxLocalPlayers;a++)
		{
			CKeys* keys=&iParent->Container()->Options()->Data().iKeys[ a ];

			for (int b=0;b<EKeyAmount;b++)
			{
				if ((keys != iKeys || b != iIndex)&&
					 keys->keys[ b ] == key.sym )
						keys->keys[ b ] = 0;
			}
		}

		iKeys->keys[iIndex]=key.sym;

		iKeyInputMode=0;
		return EMenuKeyProcessed;
	}

	return EMenuKeyNotProcessed;
}

CRect<int> CGameMenuItemChangeKey::Draw(CGraphicsBuffer* aDst,CGameGraphicsInterface* aGGI, int aX, int aY, int aActive)
{
	const char *text2;
	int xadd,yadd;
	CRect<int> rect;

	iDisplaceCounter++;

	if (!iText) return rect;

	text2=CEventHandler::KeyToStr(iKeys->keys[iIndex]);
	xadd=iKeyInputMode?0:MenuXDisplace(aActive);
	yadd=iKeyInputMode?0:MenuYDisplace(aActive);

	if (iKeyInputMode)
		text2=KKeySelectionText;

	if (aActive||iNotActiveItem)
	{
		rect = aGGI->Font(CGameGraphicsInterface::EBigMenuFont)->Write(aX-PARAM_SEPARATION+xadd,aY+yadd,iText, CFonts::EDrawBelow, CFonts::EDrawToLeft,aDst);
		rect.Combine(aGGI->Font(CGameGraphicsInterface::EBigMenuFont)->Write(aX+PARAM_SEPARATION+xadd,aY+yadd,text2, CFonts::EDrawBelow, CFonts::EDrawToRight,aDst));
	}
	else 
	{
		rect = aGGI->Font(CGameGraphicsInterface::EBigMenuFontDark)->Write(aX-PARAM_SEPARATION,aY,iText, CFonts::EDrawBelow, CFonts::EDrawToLeft,aDst);
		rect.Combine(aGGI->Font(CGameGraphicsInterface::EBigMenuFontDark)->Write(aX+PARAM_SEPARATION,aY,text2, CFonts::EDrawBelow, CFonts::EDrawToRight,aDst));
	}

	return rect;
}

/***************************************************\
*	Class CGameMenuItemMouseMode						*
\***************************************************/

CGameMenuItemMouseMode::CGameMenuItemMouseMode(CGameMenuBase* aParent, const char *aStr): CGameMenuItem(aParent,aStr)
{
}

TMenuKeyActions CGameMenuItemMouseMode::HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI)
{
	if (ActionKey( key )||RightKey( key ))
	{
		++aMenu->Container()->Options()->Data().iMouseMode%=5;
		return EMenuKeyProcessed;
	}
	if (LeftKey( key ))
	{
		--aMenu->Container()->Options()->Data().iMouseMode;
		if (aMenu->Container()->Options()->Data().iMouseMode<0)
			aMenu->Container()->Options()->Data().iMouseMode=4;

		return EMenuKeyProcessed;
	}

	return EMenuKeyNotProcessed;
}

CRect<int> CGameMenuItemMouseMode::Draw(CGraphicsBuffer* aDst,CGameGraphicsInterface* aGGI, int aX, int aY, int aActive)
{
	int xadd,yadd;
	char *tmp;
	CRect<int> rect;

	iDisplaceCounter++;

	xadd=MenuXDisplace(aActive);
	yadd=MenuYDisplace(aActive);
	tmp=iParent->Container()->Options()->MouseModeText();

	if (!iText) return rect;
	if (aActive||iNotActiveItem)
	{
		rect =aGGI->Font(CGameGraphicsInterface::EBigMenuFont)->Write(aX-PARAM_SEPARATION+xadd,aY+yadd,iText, CFonts::EDrawBelow, CFonts::EDrawToLeft,aDst);
		rect.Combine(aGGI->Font(CGameGraphicsInterface::EBigMenuFont)->Write(aX+PARAM_SEPARATION+xadd,aY+yadd,tmp, CFonts::EDrawBelow, CFonts::EDrawToRight,aDst));
	}
	else 
	{
		rect = aGGI->Font(CGameGraphicsInterface::EBigMenuFontDark)->Write(aX-PARAM_SEPARATION,aY,iText, CFonts::EDrawBelow, CFonts::EDrawToLeft,aDst);
		rect.Combine(aGGI->Font(CGameGraphicsInterface::EBigMenuFontDark)->Write(aX+PARAM_SEPARATION,aY,tmp, CFonts::EDrawBelow, CFonts::EDrawToRight,aDst));
	}

	return rect;
}

/***************************************************\
*	Class CGameMenuItemMusicTheme					*
\***************************************************/


CGameMenuItemMusicTheme::CGameMenuItemMusicTheme(CGameMenuBase* aParent, const char *aStr): CGameMenuItem(aParent,aStr)
{
	COptions* opt=iParent->Container()->Options();
	iSelected=-1;
	int a;

	for (a=0;a<opt->MusicThemes() && (iSelected==-1 || strcmp(opt->Data().iMusicTheme,opt->MusicTheme(iSelected))!=0);a++)
	{
		if (strcmp(opt->Data().iMusicTheme,opt->MusicTheme(a))==0)
			iSelected=a;
		if (iSelected==-1)
		  if (strcmp(DEFAULT_MUSICTHEME,opt->MusicTheme(a))==0)
			iSelected=a;
	}
}

void CGameMenuItemMusicTheme::UpdateText()
{
	COptions* opt=iParent->Container()->Options();

	if (iSelected<0) 
		iSelected=opt->MusicThemes()-1;
	if (iSelected>=opt->MusicThemes()) 
		iSelected=0;

	strcpy(opt->Data().iMusicTheme, opt->MusicTheme(iSelected));
	opt->UpdateAudioChanges();
}

TMenuKeyActions CGameMenuItemMusicTheme::HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI)
{
	if (LeftKey(key))
	{
		iSelected--;
		UpdateText();
		return EMenuKeyProcessed;
	}

	if (RightKey(key)||ActionKey(key))
	{
		iSelected++;
		UpdateText();
		return EMenuKeyProcessed;
	}

	return EMenuKeyNotProcessed;

}

CRect<int> CGameMenuItemMusicTheme::Draw(CGraphicsBuffer* aDst,CGameGraphicsInterface* aGGI, int aX, int aY, int aActive)
{	
	int xadd,yadd;
	CRect<int> rect;
	if (!iText) return rect;

	iDisplaceCounter++;
	xadd=MenuXDisplace(aActive);
	yadd=MenuYDisplace(aActive);

	if (aActive||iNotActiveItem)
	{
		rect.Combine(aGGI->Font(CGameGraphicsInterface::EBigMenuFont)->Write(aX-PARAM_SEPARATION+xadd,aY+yadd,iText, CFonts::EDrawBelow, CFonts::EDrawToLeft,aDst));
		rect.Combine(aGGI->Font(CGameGraphicsInterface::EBigMenuFont)->Write(aX+PARAM_SEPARATION+xadd,aY+yadd,iParent->Container()->Options()->Data().iMusicTheme, CFonts::EDrawBelow, CFonts::EDrawToRight,aDst));
	}
	else 
	{
		rect.Combine(aGGI->Font(CGameGraphicsInterface::EBigMenuFontDark)->Write(aX-PARAM_SEPARATION,aY,iText, CFonts::EDrawBelow, CFonts::EDrawToLeft,aDst));
		rect.Combine(aGGI->Font(CGameGraphicsInterface::EBigMenuFontDark)->Write(aX+PARAM_SEPARATION,aY,iParent->Container()->Options()->Data().iMusicTheme, CFonts::EDrawBelow, CFonts::EDrawToRight,aDst));
	}
	return rect;
}

/***************************************************\
*	Class CGameMenuItemLevelSelect					*
\***************************************************/

CGameMenuItemLevelSelect::CGameMenuItemLevelSelect(CGameMenuBase* aParent, const char *aStr): CGameMenuItem(aParent,NULL)
{
	iParamStr=strdup(aStr);
	iSelfDisabled = false;
	iOptions=iParent->Container()->Options();
	UpdateText();
}

TMenuKeyActions CGameMenuItemLevelSelect::HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI)
{
	if (LeftKey(key))
	{
		iOptions->Data().iMultiPlayLevel--;

		UpdateText();
		return EMenuKeyProcessed;
	}

	if (RightKey(key)||ActionKey(key))
	{
		iOptions->Data().iMultiPlayLevel++;

		UpdateText();
		return EMenuKeyProcessed;
	}
	return EMenuKeyNotProcessed;
}

void CGameMenuItemLevelSelect::ResetLevel()
{
	iParent->Container()->DoFullRedraw();
	iOptions->Data().iMultiPlayLevel = 0;
	UpdateText();
}

void CGameMenuItemLevelSelect::UpdateText()
{
	char tmp[256];
	CEpisode* epi = iOptions->EpisodeList()->Episode(iOptions->Data().iMultiPlayEpisode, (bool)(iOptions->Data().iMultiPlayGameMode) );

	if (iOptions->Data().iMultiPlayLevel>=(int)epi->Amount())
		iOptions->Data().iMultiPlayLevel = 0;

	if (iOptions->Data().iMultiPlayLevel<0)
		iOptions->Data().iMultiPlayLevel = epi->Amount()-1;

	CheckDisable();

	ASSERT(EF_SNPRINTF(tmp,256,"%s: %s",iParamStr,
		epi->LevelName(iOptions->Data().iMultiPlayLevel))>0);
	SetText(tmp);
}

void CGameMenuItemLevelSelect::CheckDisable()
{
	CEpisode* epi = iOptions->EpisodeList()->Episode(iOptions->Data().iMultiPlayEpisode,  iOptions->Data().iMultiPlayGameMode );

	if ( epi->Amount() == 0 && iEnabled )
	{
		iEnabled = false;
		iNotActiveItem = true;
		iSelfDisabled = true;
	}

	if ( epi->Amount() > 0 && iSelfDisabled )
	{
		iEnabled = true;
		iNotActiveItem = false;
		iSelfDisabled = false;
	}
}

void CGameMenuItemLevelSelect::SetEnabled( bool aEnabled )
{
	iSelfDisabled = false;
	iEnabled = aEnabled;
	if ( !iEnabled )
	{
		iNotActiveItem = true;	
	}
	else
	{
		iNotActiveItem = false;
		CheckDisable();
	}
}

/***************************************************\
*	Class CGameMenuItemEpisodeSelect				*
\***************************************************/

CGameMenuItemEpisodeSelect::CGameMenuItemEpisodeSelect(CGameMenuBase* aParent, const char *aStr,CGameMenuItemLevelSelect* aLevelSelect): CGameMenuItem(aParent,NULL)
{
	iLevelSelect = aLevelSelect;
	iParamStr=strdup(aStr);
	iOptions = iParent->Container()->Options();
	UpdateText();
}

TMenuKeyActions CGameMenuItemEpisodeSelect::HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI)
{
	if (LeftKey(key))
	{
		iOptions->Data().iMultiPlayEpisode--;
		UpdateText();
		iLevelSelect->ResetLevel();
		return EMenuKeyProcessed;
	}

	if (RightKey(key)||ActionKey(key))
	{
		iOptions->Data().iMultiPlayEpisode++;
		UpdateText();
		iLevelSelect->ResetLevel();
		return EMenuKeyProcessed;
	}
	return EMenuKeyNotProcessed;
}

void CGameMenuItemEpisodeSelect::UpdateText()
{
	char tmp[256];

	if (iOptions->Data().iMultiPlayEpisode>=(int)iOptions->EpisodeList()->Amount( iOptions->Data().iMultiPlayGameMode ))
		iOptions->Data().iMultiPlayEpisode = 0;

	if (iOptions->Data().iMultiPlayEpisode<0)
		iOptions->Data().iMultiPlayEpisode = iOptions->EpisodeList()->Amount( iOptions->Data().iMultiPlayGameMode )-1;

	ASSERT(EF_SNPRINTF(tmp,256,"%s: %s",iParamStr,iOptions->EpisodeList()->Episode( iOptions->Data().iMultiPlayEpisode,  iOptions->Data().iMultiPlayGameMode )->Name() )>0);

	SetText(tmp);
}
