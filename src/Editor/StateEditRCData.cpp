#include "editorapp.h"
#include "StateEditRCData.h"
#include "EditorDrawer.h"
#include "EditorBrush.h"
#include "StateHandler.h"
#include "../common/CEditableLevel.h"
#include "../common/CEventHandler.h"

CStateEditRCData::CStateEditRCData(CStateHandler* aStateHandler,CEditorBrush* aBrush,CEditableLevel* aLevel): CEditorState(aBrush,aLevel,aStateHandler)
{
	iCursorXPos = 0;
	iCursorYPos = 0;
}

CStateEditRCData::~CStateEditRCData(void)
{
}

void CStateEditRCData::SetMode(int aEditMode)
{
	iCurrentMode = (TEditRCDataMode)aEditMode;
}

void CStateEditRCData::Draw(CEditorDrawer* aDrawer)
{
	aDrawer->DrawRCData(iCursorXPos, iCursorYPos, iCurrentMode);
}

void CStateEditRCData::HandleKeys(CEventHandler* aEventHandler,CEditorDrawer* aDrawer)
{
	if (! aEventHandler->Kbhit()) return;
	
	short iCurKey = aEventHandler->Getch().sym;

	aEventHandler->ResetStack();

	if (iCurKey == SDLK_ESCAPE)
	{
		iCursorYPos = 0;
		iCursorXPos = 0;
		iStateHandler->SetPrevious();
		aEventHandler->ResetStack();
		aEventHandler->State(SDLK_ESCAPE)=0;
	}

	if (iCurKey == SDLK_DOWN)
		if (iCursorYPos < KBulletCrates+KWeaponCrates+1+KMoneyCrates) 
			iCursorYPos++; 

	if (iCurKey == SDLK_UP)
		if (iCursorYPos > 0) 
			iCursorYPos--; 

	if (iCurKey == SDLK_RIGHT)
	{
		if (iCursorYPos < KWeaponCrates )
			iLevel->ModifyRandomCrateAmount(CLevel::EWeaponCrate,iCursorYPos, 1, iCurrentMode == EEditDMGameData );
	
		if (iCursorYPos >= KWeaponCrates && iCursorYPos < KWeaponCrates + KBulletCrates)
			iLevel->ModifyRandomCrateAmount(CLevel::EBulletCrate,iCursorYPos-KWeaponCrates, 1, iCurrentMode == EEditDMGameData);
		
		if (iCursorYPos == KWeaponCrates + KBulletCrates)
			iLevel->ModifyRandomCrateAmount(CLevel::EHealthCrate,0, 1, iCurrentMode == EEditDMGameData);
		
		if (iCursorYPos > KWeaponCrates + KBulletCrates && iCursorYPos <= KWeaponCrates + KBulletCrates + KMoneyCrates)
			iLevel->ModifyRandomCrateAmount(CLevel::EMoneyCrate,KMoneyAmounts[iCursorYPos - (KWeaponCrates + KBulletCrates + 1)], 1, iCurrentMode == EEditDMGameData);

		if (iCursorYPos == KWeaponCrates + KBulletCrates + 1 + KMoneyCrates)
			iLevel->ModifyRandomCrateAmount(CLevel::ETargetCrate,0, 1, iCurrentMode == EEditDMGameData);
	}

	if (iCurKey == SDLK_LEFT)
	{
		if (iCursorYPos < KWeaponCrates )
			iLevel->ModifyRandomCrateAmount(CLevel::EWeaponCrate,iCursorYPos, -1, iCurrentMode == EEditDMGameData);
	
		if (iCursorYPos >= KWeaponCrates && iCursorYPos < KWeaponCrates + KBulletCrates)
			iLevel->ModifyRandomCrateAmount(CLevel::EBulletCrate,iCursorYPos-KWeaponCrates, -1, iCurrentMode == EEditDMGameData);
	
		if (iCursorYPos == KWeaponCrates + KBulletCrates)
			iLevel->ModifyRandomCrateAmount(CLevel::EHealthCrate,0, -1, iCurrentMode == EEditDMGameData);
		
		if (iCursorYPos >= KWeaponCrates + KBulletCrates + 1 && iCursorYPos < KWeaponCrates + KBulletCrates + 1 + KMoneyCrates)
			iLevel->ModifyRandomCrateAmount(CLevel::EMoneyCrate,KMoneyAmounts[iCursorYPos - (KWeaponCrates + KBulletCrates + 1)], -1, iCurrentMode == EEditDMGameData);
		
		if (iCursorYPos == KWeaponCrates + KBulletCrates + 1 + KMoneyCrates)
			iLevel->ModifyRandomCrateAmount(CLevel::ETargetCrate,0, -1, iCurrentMode == EEditDMGameData);
	}


}

void CStateEditRCData::HandleMouse(CEventHandler* aEventHandler,CEditorDrawer* aDrawer)
{
}
