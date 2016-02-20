// CDynShopData.h: interface for the CDynShopData class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __CDYNSHOPDATA_H
#define __CDYNSHOPDATA_H

#include "common/CGameTypeData.h"

class CGameData;

const int KCategories=3;
const int KOtherStuffInShop=2;
extern const int KItemsInCategories[3];

class CDynShopData  
{
public:
	CDynShopData();

	void BigIncItem();
	void BigDecItem();
	void IncItem();
	void DecItem();

	void ResetSelection();
	
	inline int Cat(){ return iCatSelect;}
	inline int Item(){ return iItemSelect;}

	inline int Player(){ return iPlayerNo;}
	inline bool PlayerDone(){ return iPlayerDone;}

	inline void SetPlayerDone(bool aDone){ iPlayerDone=aDone; }
	inline void SetPlayer(int aNo){ iPlayerNo=aNo; }

	void SetSkipOnce();
	bool SkipOnce();

	void UpdatePrices(CGameData* aGD);

	int iWeaponSellPrice[EWeaponAmount-1]; // Fist is not in SHOP
	int iShieldSellPrice;
	int iTargetSellPrice;
	int iTargetPrice;

private:
    void AlignItemAndCat(int iBigStep);

	bool iSkipOnce;
	bool iPlayerDone;
	int iPlayerNo;
	int iCatSelect;
	int iItemSelect;
};

#endif

