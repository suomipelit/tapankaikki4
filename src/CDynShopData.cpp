// CDynShopData.cpp: implementation of the CDynShopData class.
//
//////////////////////////////////////////////////////////////////////

#include "CDynShopData.h"
#include "common/IShield.h"
#include "common/IWeaponType.h"
#include "CGameData.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const int KItemsInCategories[3]={EWeaponAmount-1,EBulletAmount-2,KOtherStuffInShop};

CDynShopData::CDynShopData()
{
	iSkipOnce = false;
	iPlayerDone=0;
	iPlayerNo=0;
	iCatSelect=0;
	iItemSelect=0;
	iTargetPrice=500;

	for (int a=0;a<EWeaponAmount-1;a++)
		iWeaponSellPrice[a]=0;
	iShieldSellPrice=0;
	iTargetSellPrice=0;	
}

void CDynShopData::ResetSelection()
{
	iCatSelect=0;
	iItemSelect=0;
}

void CDynShopData::IncItem()
{
	iItemSelect++;
	AlignItemAndCat(0);
}

void CDynShopData::DecItem()
{
	iItemSelect--;
	AlignItemAndCat(0);
}

void CDynShopData::BigIncItem()
{
	iItemSelect+=8;
	AlignItemAndCat(1);
}

void CDynShopData::BigDecItem()
{
	iItemSelect-=8;
	AlignItemAndCat(1);
}

void CDynShopData::AlignItemAndCat(int iBigStep)
{

	if (iItemSelect >= KItemsInCategories[iCatSelect])
	{
		if (iCatSelect<KCategories-1)
		{
			if (!(iBigStep&&
				(iItemSelect&7)>=(KItemsInCategories[iCatSelect]&7)))
			{
				iCatSelect++;
			
			if (iBigStep)
				iItemSelect&=0x7;
			else
				iItemSelect=0;
			}

			if (iItemSelect >= KItemsInCategories[iCatSelect])
				iItemSelect= KItemsInCategories[iCatSelect]-1;
		}
		else
		{
			iItemSelect= KItemsInCategories[iCatSelect]-1;
		}
	}

	if (iItemSelect <0 )
	{
		if (iCatSelect)
		{
			iCatSelect--;
			iItemSelect = 8+iItemSelect+(KItemsInCategories[iCatSelect]&0xf8);
			if (iItemSelect >= KItemsInCategories[iCatSelect])
				iItemSelect= KItemsInCategories[iCatSelect]-1;
		}
		else
			iItemSelect=0;
	}
}

void CDynShopData::UpdatePrices(CGameData* aGD) 
{
	int a;
	for(a=0; a<EWeaponAmount-1; a++) 
		iWeaponSellPrice[a]=(int)((0.8f*aGD->TypeData()->iWeaponType[a + 1]->Cost())  -(rand()%(aGD->TypeData()->iWeaponType[a + 1]->Cost() / 2)));
	iShieldSellPrice=(int)((0.8f*aGD->TypeData()->iShieldType->Cost())  -(rand()%(aGD->TypeData()->iShieldType->Cost() / 2)));
	iTargetSellPrice=(int)((0.8f*iTargetPrice)  -(rand()%(iTargetPrice / 2)));
}

void CDynShopData::SetSkipOnce()
{
	iSkipOnce = true;
}

bool CDynShopData::SkipOnce()
{
	if (iSkipOnce)
	{
		iSkipOnce = false;
		return true;
	}

	return false;
}
