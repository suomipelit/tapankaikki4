#include "common/CEventHandler.h"
#include "common/CGameTypeData.h"
#include "common/IWeaponType.h"
#include "common/IBulletType.h"
#include "common/CFonts.h"
#include "common/CDrawArea.h"

#include "CGameSubStateShop.h"
#include "IGUIStateController.h"
#include "CSoundPlayer.h"
#include "CDynShopData.h"
#include "IGameSubStateController.h"
#include "CGameState.h"
#include "CGameGraphicsInterface.h"
#include "CDynGameData.h"

namespace
{
	// TODO: CONFIGFILE
	const char* KSHOP_INSTRUCTION_LINE0="Press ESC to exit the shop.";
	const char* KSHOP_INSTRUCTION_LINE1="Press ARROWS to navigate. RETURN to buy item, BACKSPACE to sell item";
	const char* KSHOP_INSTRUCTION_LINE2="Press PAGEUP to buy 25 items and PAGEDOWN to sell 25 items";
	const char* KSHOP_TITLE="The Shop";

	const int KSHOP_DESCRIPTION_START_Y=10;

	const int KSHOPITEMS_START_X=5;
	const int KSHOPITEMS_START_Y=78;
	const int KSHOPITEM_SPRITE_WIDTH=70;
	const int KSHOPITEM_SPRITE_HEIGHT=50;
	const int KSHOPITEM_NEXT_SPRITE_START_X=(KSHOPITEM_SPRITE_WIDTH+10);
	const int KSHOPITEM_NEXT_SPRITE_START_Y=(KSHOPITEM_SPRITE_HEIGHT+10);
	const int KSHOP_NEXT_CATEGORY_START=126;
}

CGameSubStateShop::CGameSubStateShop(IGameSubStateController* aSubStateController): iStateController (aSubStateController),iBackgroundPicture(NULL)
{
	iEventHandler = aSubStateController->GUIStateController()->EventHandler();
	iSP = aSubStateController->GUIStateController()->SoundPlayer();
	iShopData = aSubStateController->ShopData();
	iGameData = aSubStateController->GUIStateController()->GameData();
	ASSERT(iEventHandler);
	ASSERT(iSP);
	ASSERT(iShopData);
	ASSERT(iGameData);
}

CGameSubStateShop::~CGameSubStateShop()
{
	delete iBackgroundPicture;
}

// return true if draw is needed
bool CGameSubStateShop::RunFrame()
{
	bool drawNeeded = RunKeys();

	if (iStateController->GUIStateController()->State()->First())
		iStateController->GUIStateController()->StartFadeIn(CGameData::EPaletteShop);

	if (iShopData->PlayerDone())
	{
		iShopData->SetPlayer(iShopData->Player()+1);
		
		if (iShopData->Player()>=iStateController->DynData()->LocalPlayers().size())
		{
			iStateController->GUIStateController()->StartFadeOut(CState(CState::EMainStateGame,EGameStateOnGoing));
			iShopData->SetPlayer(0);

		}
		else
			iStateController->GUIStateController()->StartFadeOut(CState(CState::EMainStateGame,EGameStateShop));
		
		iShopData->SetSkipOnce();
		iShopData->SetPlayerDone( false );
	}

	return drawNeeded;
}

bool CGameSubStateShop::RunKeys()
{
	CPlayer* current = iStateController->DynData()->LocalPlayers()[iShopData->Player()];
	ASSERT(current);
	bool retval = false;

	/* Lets check keyboard input */
	while (iEventHandler->Kbhit())
	{
		SDLKey sym = iEventHandler->Getch().sym;

		if ( sym == SDLK_ESCAPE )
		{
			iShopData->SetPlayerDone(true);
			retval = true;
		}

		if ( sym == SDLK_DOWN || sym == current->iKeys.keys[ EKeyDOWN ])
		{
			iShopData->BigIncItem();
			retval = true;
		}

		if ( sym == SDLK_UP || sym == current->iKeys.keys[ EKeyUP ] )
		{
			iShopData->BigDecItem();
			retval = true;
		}

		if ( sym == SDLK_LEFT || sym == current->iKeys.keys[ EKeyLEFT ] || sym == current->iKeys.keys[ EKeyLSTRAFE ])
		{
			iShopData->DecItem();
			retval = true;
		}

		if ( sym == SDLK_RIGHT || sym == current->iKeys.keys[ EKeyRIGHT ] || sym == current->iKeys.keys[ EKeyRSTRAFE ] )
		{
			iShopData->IncItem();
			retval = true;
		}

		if ( sym == SDLK_RETURN ||( sym == current->iKeys.keys[ EKeySHOOT ] && current->iKeys.keys[ EKeySHIFT ] != SDLK_BACKSPACE))
		{
			current->ShopBuy(1,iShopData);
			retval = true;
		}

		if ( sym == SDLK_BACKSPACE || (sym == current->iKeys.keys[ EKeySHIFT ] && current->iKeys.keys[ EKeySHIFT ] != SDLK_RETURN))
		{
			current->ShopSell(1,iShopData);
			retval = true;
		}

		if ( sym == SDLK_PAGEDOWN )
		{
			current->ShopSell(25,iShopData);
			retval = true;
		}

		if ( sym == SDLK_PAGEUP )
		{
			current->ShopBuy(25,iShopData);
			retval = true;
		}
	}

	if (retval)
		iSP->PlaySample( iGameData->Click(), KFullVolume, PAN_MIDDLE);

	return retval;
}

void CGameSubStateShop::Draw(CDrawArea& aDirtyArea,CDrawArea& aDrawArea,CGameGraphicsInterface* aGGI)
{
	const int KTempStringLength = 256;
	const CGraphicsBuffer* hilite_picture = iGameData->MenuSprite(EMenuSpriteShopBackHilite);
	const CGraphicsBuffer* hollow_picture = iGameData->MenuSprite(EMenuSpriteShopBackHollow);
	const CGraphicsBuffer* selection_picture = iGameData->MenuSprite(EMenuSpriteShopSelection);

	const CFonts* smallfont= aGGI->Font(CGameGraphicsInterface::ESmallMenuFontDark);
	CPlayer* current = iStateController->DynData()->LocalPlayers()[iShopData->Player()];
	int item=0,cat=0;
	char str[KTempStringLength];
	int cur_amount=0;

	if ( iShopData->SkipOnce() )
		return;

	DEBUG_ASSERT(hilite_picture);
	DEBUG_ASSERT(hollow_picture);
	DEBUG_ASSERT(selection_picture);
	DEBUG_ASSERT(smallfont);
	DEBUG_ASSERT(current);

	iCleanArea.Combine(aDirtyArea);

	for (int a=0;a<iCleanArea.Size();a++)
	{
		const CRect<int>& rect=iCleanArea.Rect(a,aGGI->DrawBuffer()->Rect());
		aDrawArea.Combine(aGGI->DrawBuffer()->Copy(
			rect.iLeft,
			rect.iTop,
			iBackgroundPicture,
			rect.iLeft,
			rect.iTop,
			rect.Width(),
			rect.Height()));
	}

	iCleanArea.Reset();

	for (cat=0;cat<KCategories;cat++)
	 for (item=0;item<KItemsInCategories[cat];item++)
	 {
		int x,y;
		y=KSHOPITEMS_START_Y+cat*KSHOP_NEXT_CATEGORY_START+(item>>3)*KSHOPITEM_NEXT_SPRITE_START_Y;
		x=KSHOPITEMS_START_X+(item&7)*KSHOPITEM_NEXT_SPRITE_START_X;

		switch(cat)
		{
			case 0: cur_amount = current->iWeapons[item+1];break;
			case 1: cur_amount = current->iBullets[item+1]/iGameData->TypeData()->iBulletType[item+1]->Multiplier();break;
			case 2: if (item==0) 
						cur_amount = current->iShield;
					else
						cur_amount = current->iTargetSystem;
				break;			
		}

		if (cur_amount>0)
		{
			//We got it, hilite it. 
			iCleanArea.Combine(
				aGGI->DrawBuffer()->Copy(x,y,hilite_picture,x,y,KSHOPITEM_SPRITE_WIDTH,KSHOPITEM_SPRITE_HEIGHT)
				);
		}
		else
			if ((cat==0 && iGameData->TypeData()->iWeaponType[item+1]->Cost()> current->iCash)||
				(cat==1 && iGameData->TypeData()->iBulletType[item+1]->Cost()> current->iCash)||
				(cat==2 && item==0 && current->ShieldPrice()>current->iCash)||
				(cat==2 && item==1 && iShopData->iTargetPrice>current->iCash))
			{
				//We can buy it, draw it hollow.
				iCleanArea.Combine(
					aGGI->DrawBuffer()->Copy(x,y,hollow_picture,x,y,KSHOPITEM_SPRITE_WIDTH,KSHOPITEM_SPRITE_HEIGHT)
				);
			}

		//If item was selected draw rectangle around it
		if (cat==iShopData->Cat() && item==iShopData->Item())
			iCleanArea.Combine(aGGI->DrawBuffer()->TransCopy(x-5,y-5,selection_picture,0,0,selection_picture->Width(),selection_picture->Height(),0));

		DrawWareInfo(KSHOPITEMS_START_X,KSHOP_DESCRIPTION_START_Y,iCleanArea,aGGI);

		if (cur_amount>0)
		{
			ASSERT(EF_SNPRINTF(str,KTempStringLength,"%d",cur_amount)>0);

			aDrawArea.Combine(smallfont->Write(
				x+KSHOPITEM_SPRITE_WIDTH,
				y+KSHOPITEM_SPRITE_HEIGHT,
				str,
				CFonts::EDrawAbove, CFonts::EDrawToLeft, 
				aGGI->DrawBuffer()));
		}

	 }

	ASSERT(EF_SNPRINTF(str,KTempStringLength,"Your cash: %d",current->iCash)>0);
	iCleanArea.Combine(smallfont->Write(
		aGGI->DrawBuffer()->Width()-KSHOPITEMS_START_X,
		KSHOP_DESCRIPTION_START_Y,
		str,
		CFonts::EDrawBelow, CFonts::EDrawToLeft, 
		aGGI->DrawBuffer()));

	aDrawArea.Combine(iCleanArea);
}

void CGameSubStateShop::DrawWareInfo(int aX, int aY,CDrawArea& aDrawArea,CGameGraphicsInterface* aGGI) 
{
	const int KTempStringLength = 256;
	CPlayer* current = iStateController->DynData()->LocalPlayers()[iShopData->Player()];
	const CFonts* font=aGGI->Font(CGameGraphicsInterface::ESmallMenuFontDark);
	char txt[KTempStringLength];
	int a;

	if (iShopData->Cat() == 0) 
	{
		aDrawArea.Combine(font->Write(aX, aY, iGameData->TypeData()->iWeaponType[iShopData->Item()+1]->Name(),CFonts::EDrawBelow, CFonts::EDrawToRight,  aGGI->DrawBuffer()));

		ASSERT(EF_SNPRINTF(txt,KTempStringLength,"Cost: %d",iGameData->TypeData()->iWeaponType[iShopData->Item()+1]->Cost())>0);
		aDrawArea.Combine(font->Write(aX, aY+20, txt,CFonts::EDrawBelow, CFonts::EDrawToRight,  aGGI->DrawBuffer()));

		ASSERT(EF_SNPRINTF(txt,KTempStringLength,"Sell price: %d",iShopData->iWeaponSellPrice[iShopData->Item()])>0);
		aDrawArea.Combine(font->Write(aX, aY+30, txt,CFonts::EDrawBelow, CFonts::EDrawToRight,  aGGI->DrawBuffer()));
		
		ASSERT(EF_SNPRINTF(txt,KTempStringLength,"Bullet type: %s",iGameData->TypeData()->iBulletType[iGameData->TypeData()->iWeaponType[iShopData->Item()+1]->BulletType()]->Name())>0);
		aDrawArea.Combine(font->Write(aX, aY+40, txt,CFonts::EDrawBelow, CFonts::EDrawToRight,  aGGI->DrawBuffer()));

		return;
	}
	if (iShopData->Cat() == 1) 
	{
		aDrawArea.Combine(font->Write(aX, aY, iGameData->TypeData()->iBulletType[iShopData->Item()+1]->Name(),CFonts::EDrawBelow, CFonts::EDrawToRight,  aGGI->DrawBuffer()));

		ASSERT(EF_SNPRINTF(txt,KTempStringLength,"Cost: %d",iGameData->TypeData()->iBulletType[iShopData->Item()+1]->Cost())>0);
		aDrawArea.Combine(font->Write(aX, aY+20, txt,CFonts::EDrawBelow, CFonts::EDrawToRight,  aGGI->DrawBuffer()));

		return;
	}
	if (iShopData->Cat() == 2) 
	{
		if (iShopData->Item() == 0) 
		{
			aDrawArea.Combine(font->Write(aX, aY, "Shield",CFonts::EDrawBelow, CFonts::EDrawToRight,  aGGI->DrawBuffer()));

			ASSERT(EF_SNPRINTF(txt,KTempStringLength,"Cost: %d",current->ShieldPrice())>0);
			aDrawArea.Combine(font->Write(aX, aY+20, txt,CFonts::EDrawBelow, CFonts::EDrawToRight,  aGGI->DrawBuffer()));
			a=current->iShield - 1;
			if (a<0) a=0;

			ASSERT(EF_SNPRINTF(txt,KTempStringLength,"Sell price: %d",iShopData->iShieldSellPrice+(current->ShieldPrice()>> 1))>0);
			aDrawArea.Combine(font->Write(aX, aY+30, txt,CFonts::EDrawBelow, CFonts::EDrawToRight,  aGGI->DrawBuffer()));
			return;
		}
		if (iShopData->Item() == 1) 
		{
			aDrawArea.Combine(font->Write(aX, aY, "Target system", CFonts::EDrawBelow, CFonts::EDrawToRight,aGGI->DrawBuffer()));
			ASSERT(EF_SNPRINTF(txt,KTempStringLength,"Cost: %d",iShopData->iTargetPrice)>0);
			aDrawArea.Combine(font->Write(aX, aY+20, txt,CFonts::EDrawBelow, CFonts::EDrawToRight,  aGGI->DrawBuffer()));

			ASSERT(EF_SNPRINTF(txt,KTempStringLength,"Sell price: %d",iShopData->iTargetSellPrice)>0);
			aDrawArea.Combine(font->Write(aX, aY+30, txt,CFonts::EDrawBelow, CFonts::EDrawToRight,  aGGI->DrawBuffer()));
			return;
		}
	}
}
void CGameSubStateShop::DrawStaticToBackground()
{
	const int KTempStringLength = 256;
	const CGraphicsBuffer* normal_picture = iGameData->MenuSprite(EMenuSpriteShopBackNormal);
	const CFonts* bigfont=iStateController->GUIStateController()->GGI()->Font(CGameGraphicsInterface::EBigMenuFont);
	const CFonts* smallfont= iStateController->GUIStateController()->GGI()->Font(CGameGraphicsInterface::ESmallMenuFontDark);
	CPlayer* current = iStateController->DynData()->LocalPlayers()[iShopData->Player()];
	char str[KTempStringLength];

	DEBUG_ASSERT(normal_picture);
	DEBUG_ASSERT(bigfont);
	DEBUG_ASSERT(smallfont);

	// put background picture
	iBackgroundPicture->Copy(normal_picture);

	// draw shoptitle
	bigfont->Write(iBackgroundPicture->Width()/2,10,KSHOP_TITLE,CFonts::EDrawBelow, CFonts::EDrawInMiddle, iBackgroundPicture);

	// draw instruction lines
	smallfont->Write(
		KSHOPITEMS_START_X,
		iBackgroundPicture->Height()-smallfont->Height()*3,
		KSHOP_INSTRUCTION_LINE0,
		CFonts::EDrawBelow, CFonts::EDrawToRight, 
		iBackgroundPicture);

	smallfont->Write(
		KSHOPITEMS_START_X,
		iBackgroundPicture->Height()-smallfont->Height()*2,
		KSHOP_INSTRUCTION_LINE1,
		CFonts::EDrawBelow, CFonts::EDrawToRight, 
		iBackgroundPicture);

	smallfont->Write(
		KSHOPITEMS_START_X,
		iBackgroundPicture->Height()-smallfont->Height(),
		KSHOP_INSTRUCTION_LINE2,
		CFonts::EDrawBelow, CFonts::EDrawToRight, 
		iBackgroundPicture);

	if (strlen(current->iName)>0)
		ASSERT(EF_SNPRINTF(str,KTempStringLength,"Player %d: %s",iShopData->Player()+1,current->iName)>0);
	else
		ASSERT(EF_SNPRINTF(str,KTempStringLength,"Player %d",iShopData->Player()+1)>0);

	smallfont->Write(
		iBackgroundPicture->Width()-KSHOPITEMS_START_X,
		KSHOP_DESCRIPTION_START_Y+smallfont->Height(),
		str,
		CFonts::EDrawBelow, CFonts::EDrawToLeft, 
		iBackgroundPicture);

}

void CGameSubStateShop::EnterState()
{
	iShopData->SetPlayerDone(false);
	if (iShopData->Player()==0)
	{
		iShopData->UpdatePrices(iGameData);
	}
	iEventHandler->ResetStack();
	iShopData->ResetSelection();

	iBackgroundPicture = new CGraphicsBuffer(iStateController->GUIStateController()->GGI()->DrawBuffer()->Rect());
	DrawStaticToBackground();
}

void CGameSubStateShop::ExitState()
{
	delete iBackgroundPicture;
	iBackgroundPicture = NULL;
}

