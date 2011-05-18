#include "StateAddObjects.h"
#include "EditorDrawer.h"
#include "EditorBrush.h"
#include "StateHandler.h"
#include "../common/CColorTable.h"
#include "../common/CEditableLevel.h"
#include "../common/CMath.h"
#include "../common/CEventHandler.h"
#include "editorapp.h"

const int KMoneyAdd = 50; // Amount of money increase for placed money crates when changed

CStateAddObjects::CStateAddObjects(CStateHandler* aStateHandler,CEditorBrush* aBrush,CEditableLevel* aLevel): CEditorState(aBrush,aLevel,aStateHandler)
{
	iPlayerToPlace = 0;
	iSpotSize = 3;
	iSpotSetSizeMode = 0;
	iSteamAngle = 45;
	iSteamSpeed = 3;
	iSteamAdjustMode = 0;
	iCrateType1 = 0;
	iCrateType2 = 0;
	iEnemyType = 0;
}

CStateAddObjects::~CStateAddObjects(void)
{
}

void CStateAddObjects::SetMode(int aEditMode)
{
	iCurrentMode = (TEditObjectMode)aEditMode;
}

void CStateAddObjects::Draw(CEditorDrawer* aDrawer)
{
	int SBS = aDrawer->ScrBlockSize();
	// Mouse pointer position in blocks at level
	int MBX = iMouseX/SBS+aDrawer->ScrXOffs();
	int MBY = iMouseY/SBS+aDrawer->ScrYOffs();
	int AMX = (iMouseX+aDrawer->ScrXOffs()*SBS)/aDrawer->Zoom();
	int AMY = (iMouseY+aDrawer->ScrYOffs()*SBS)/aDrawer->Zoom();
	int rX1 = iMouseX/SBS*SBS;
	int rY1 = iMouseY/SBS*SBS;
	int rX2 = iMouseX/SBS*SBS+SBS-1;
	int rY2 = iMouseY/SBS*SBS+SBS-1;
	char pl_num[255];
	itoa(iPlayerToPlace+1,pl_num,10);
	char text[255];

	aDrawer->DrawLevel(0,0,0);
		
	switch (iCurrentMode) {
		case EEditObjectModePlaceSpotLight:
			if (Level()->InLevel(MBX,MBY))
				if (! iSpotSetSizeMode)				
					aDrawer->DrawSpot(iMouseX,iMouseY,iSpotSize);
				else
					aDrawer->DrawSpot(iSpotSizeX,iSpotSizeY,iSpotSize);
			if (iSpotSetSizeMode)
				aDrawer->Write(iSpotSizeX,iSpotSizeY,iSpotSize);
			aDrawer->Write(5,5,"Add objects mode - add spotlight");
			aDrawer->Write(5,5+aDrawer->FontHeight(),"Hold down RIGHT MOUSE BUTTON to ADJUST SIZE");
		break;
		case EEditObjectModePlaceSteam:
			if (Level()->InLevel(MBX,MBY))
				if (! iSteamAdjustMode)				
					aDrawer->DrawSteam(iMouseX,iMouseY,iSteamAngle,iSteamSpeed);
				else
					aDrawer->DrawSteam(iSteamX,iSteamY,iSteamAngle,iSteamSpeed);
			if (iSteamAdjustMode)
			{
				aDrawer->Write(iSteamX,iSteamY,iSteamAngle);
				aDrawer->Write(iSteamX,iSteamY+aDrawer->FontHeight(),iSteamSpeed);
			}
			aDrawer->Write(5,5,"Add objects mode - add steam");
			aDrawer->Write(5,5+aDrawer->FontHeight(),"Hold down RIGHT MOUSE BUTTON to ADJUST ANGLE and LENGHT");
		break;
		case EEditObjectModePlacePL:
			if (Level()->InLevel(MBX,MBY))
			{
				aDrawer->DrawRect(rX1,rY1,rX2,rY2,255);
				strcpy(text,"PL ");
				strcat(text,pl_num);
				aDrawer->Write((MBX-aDrawer->ScrXOffs())*SBS+SBS/2-8,(MBY-aDrawer->ScrYOffs())*SBS+SBS/2-5,text);
			}
			aDrawer->Write(5,5,"Add objects mode - place player start point");
			aDrawer->Write(5,5+aDrawer->FontHeight(),"Use PGUP & PGDOWN keys (or mouse wheel) to change player number");
		break;
		case EEditObjectModePlaceNormalCrate:
			if (Level()->InLevel(MBX,MBY))
			{
				aDrawer->DrawCrate(iMouseX,iMouseY,iCrateType1,iCrateType2,16);
			}
			aDrawer->Write(5,5,"Add objects mode - place normal game crate");
			aDrawer->Write(5,5+aDrawer->FontHeight(),"Use PGUP,PGDOWN (or mouse wheel),HOME and END keys to select type");
		break;
		case EEditObjectModePlaceDMCrate:
			if (Level()->InLevel(MBX,MBY))
			{
				aDrawer->DrawCrate(iMouseX,iMouseY,iCrateType1,iCrateType2,16*9);
			}
			aDrawer->Write(5,5,"Add objects mode - place deathmatch crate");
			aDrawer->Write(5,5+aDrawer->FontHeight(),"Use PGUP,PGDOWN (or mouse wheel),HOME and END keys to select type");
		break;
		case EEditObjectModePlaceEnemy:
			if (Level()->InLevel(MBX,MBY))
			{
				aDrawer->DrawEnemy(iMouseX-KEnemyRealRad*aDrawer->Zoom(),iMouseY-KEnemyRealRad*aDrawer->Zoom(),iEnemyType);
			}
			aDrawer->Write(5,5,"Add objects mode - place enemy");
			aDrawer->Write(5,5+aDrawer->FontHeight(),"Use PGUP & PGDOWN (or mouse wheel) keys to select type");
			aDrawer->DrawMouseCoords(AMX-KEnemyRealRad,AMY-KEnemyRealRad);
		break;
	}
}

void CStateAddObjects::HandleKeys(CEventHandler* aEventHandler,CEditorDrawer* aDrawer)
{
	if (aEventHandler->State(SDLK_KP_PLUS)) {aDrawer->ZoomIn();aEventHandler->State(SDLK_KP_PLUS)=0;}
	if (aEventHandler->State(SDLK_KP_MINUS)) {aDrawer->ZoomOut();aEventHandler->State(SDLK_KP_MINUS)=0;}
	if (aEventHandler->State(SDLK_UP)) aDrawer->SetScrYOffs(aDrawer->ScrYOffs()-1);
	if (aEventHandler->State(SDLK_DOWN)) aDrawer->SetScrYOffs(aDrawer->ScrYOffs()+1);
	if (aEventHandler->State(SDLK_LEFT)) aDrawer->SetScrXOffs(aDrawer->ScrXOffs()-1);
	if (aEventHandler->State(SDLK_RIGHT)) aDrawer->SetScrXOffs(aDrawer->ScrXOffs()+1);
	if (aEventHandler->State(SDLK_SPACE)) {StateHandler()->SetState(EEditModeGetBlock);aEventHandler->State(SDLK_SPACE)=0;}
	if (aEventHandler->State(SDLK_RETURN)) {StateHandler()->SetState(EEditModeEditHeights);aEventHandler->State(SDLK_RETURN)=0;}
	if (aEventHandler->State(SDLK_ESCAPE)) {StateHandler()->SetPrevious();aEventHandler->State(SDLK_ESCAPE)=0;}
	if (iCurrentMode == EEditObjectModePlaceNormalCrate ||
		iCurrentMode == EEditObjectModePlaceDMCrate)
		{
			if (aEventHandler->State(SDLK_PAGEUP))
			{
				if (iCrateType1<CLevel::ECrateAmount-1) iCrateType1++;
				aEventHandler->State(SDLK_PAGEUP) = 0;
			}
			if (aEventHandler->State(SDLK_PAGEDOWN))
			{
				if (iCrateType1>0) iCrateType1--;
				aEventHandler->State(SDLK_PAGEDOWN) = 0;
			}
			if (aEventHandler->State(SDLK_HOME))
			{
				switch (iCrateType1){
					case CLevel::EMoneyCrate:
						iCrateType2+=KMoneyAdd;
					break;
					default: iCrateType2++;
				}
				aEventHandler->State(SDLK_HOME) = 0;
			}
			if (aEventHandler->State(SDLK_END))
			{
				switch (iCrateType1){
					case CLevel::EMoneyCrate:
						if (iCrateType2>0) iCrateType2-=KMoneyAdd;
						if (iCrateType2<KMoneyAdd) iCrateType2=KMoneyAdd;
					break;
					default: if (iCrateType2>0) iCrateType2--;
				}
				aEventHandler->State(SDLK_END) = 0;
			}
			switch (iCrateType1){
				case CLevel::EWeaponCrate:				
					if (iCrateType2>=KWeaponCrates-1) iCrateType2=KWeaponCrates-1;
				break;
				case CLevel::EBulletCrate:				
					if (iCrateType2>=KBulletCrates-1) iCrateType2=KBulletCrates-1;
				break;
				case CLevel::EMoneyCrate:				
					if (iCrateType2>=KMaxMoneyInCrate) iCrateType2=KMaxMoneyInCrate;
					if (iCrateType2<KMoneyAdd) iCrateType2=KMoneyAdd;
				break;
				default: iCrateType2 = 0;
			}
		}
	if (iCurrentMode == EEditObjectModePlaceEnemy)
	{
		if (aEventHandler->State(SDLK_PAGEUP))
		{
			if (iEnemyType<EEnemyAmount-1) iEnemyType++;
			aEventHandler->State(SDLK_PAGEUP) = 0;
		}
		if (aEventHandler->State(SDLK_PAGEDOWN))
		{
			if (iEnemyType>0) iEnemyType--;
			aEventHandler->State(SDLK_PAGEDOWN) = 0;
		}
	}
	if (iCurrentMode ==	EEditObjectModePlacePL)
	{
		if (aEventHandler->State(SDLK_PAGEUP))
		{
			if (iPlayerToPlace < Level()->StartPositions()) iPlayerToPlace++;
			aEventHandler->State(SDLK_PAGEUP) = 0;
		}
		if (aEventHandler->State(SDLK_PAGEDOWN))
		{
			if (iPlayerToPlace > 0) iPlayerToPlace--;
			aEventHandler->State(SDLK_PAGEDOWN) = 0;
		}
	}
}
void CStateAddObjects::HandleMouse(CEventHandler* aEventHandler,CEditorDrawer* aDrawer)
{
	int SBS = aDrawer->ScrBlockSize();
	iMouseX = aEventHandler->GetMouse().XPos();
	iMouseY = aEventHandler->GetMouse().YPos();
	int AMX = (iMouseX+aDrawer->ScrXOffs()*SBS)/aDrawer->Zoom();
	int AMY = (iMouseY+aDrawer->ScrYOffs()*SBS)/aDrawer->Zoom();
	// Mouse pointer position in blocks at level
	int MBX = iMouseX/SBS+aDrawer->ScrXOffs();
	int MBY = iMouseY/SBS+aDrawer->ScrYOffs();
	bool occupied=false;
	
	if (aEventHandler->GetMouse().Button(CMouse::EButtonWheelDown))
	{
		if (iCurrentMode == EEditObjectModePlaceNormalCrate ||
			iCurrentMode == EEditObjectModePlaceDMCrate)
			switch (iCrateType1){
					case CLevel::EMoneyCrate:
						if (iCrateType2>0) iCrateType2-=KMoneyAdd;
						if (iCrateType2<KMoneyAdd) iCrateType2=KMoneyAdd;
					break;
					default: if (iCrateType2>0) iCrateType2--;
			}
		if (iCurrentMode == EEditObjectModePlaceEnemy)
			if (iEnemyType>0) iEnemyType--;
		if (iCurrentMode ==	EEditObjectModePlacePL)
			if (iPlayerToPlace > 0) iPlayerToPlace--;
		aEventHandler->GetMouse().DecButton(CMouse::EButtonWheelDown);
	}

	if (aEventHandler->GetMouse().Button(CMouse::EButtonWheelUp))
	{
		if (iCurrentMode == EEditObjectModePlaceNormalCrate ||
			iCurrentMode == EEditObjectModePlaceDMCrate)
		{
			switch (iCrateType1){
					case CLevel::EMoneyCrate:
						iCrateType2+=KMoneyAdd;
					break;
					default: iCrateType2++;
			}

			switch (iCrateType1){
				case CLevel::EWeaponCrate:				
					if (iCrateType2>=KWeaponCrates-1) iCrateType2=KWeaponCrates-1;
				break;
				case CLevel::EBulletCrate:				
					if (iCrateType2>=KBulletCrates-1) iCrateType2=KBulletCrates-1;
				break;
				case CLevel::EMoneyCrate:				
					if (iCrateType2>=KMaxMoneyInCrate) iCrateType2=KMaxMoneyInCrate;
				break;
				default: iCrateType2=0;
			}
		}
		if (iCurrentMode == EEditObjectModePlaceEnemy)
			if (iEnemyType<EEnemyAmount-1) iEnemyType++;
		if (iCurrentMode ==	EEditObjectModePlacePL)
			if (iPlayerToPlace < Level()->StartPositions()) iPlayerToPlace++;
		aEventHandler->GetMouse().DecButton(CMouse::EButtonWheelUp);
	}

	if (aEventHandler->GetMouse().Button(CMouse::EButtonRight))
	{
		switch (iCurrentMode){
			case EEditObjectModePlaceSpotLight:
				if (! iSpotSetSizeMode)
				{
					iSpotSetSizeMode = 1;
					iSpotSizeX = iMouseX;
					iSpotSizeY = iMouseY;
				} else
				{
					iSpotSize = int(CMath::Static()->GetDist(iMouseX,iMouseY,iSpotSizeX,iSpotSizeY)/(aDrawer->Zoom()*3));
					if (iSpotSize > KLightAmount - 1) iSpotSize = KLightAmount - 1;
				}
			break;
			case EEditObjectModePlaceSteam:
				if (! iSteamAdjustMode)
				{
					iSteamAdjustMode = 1;
					iSteamX = iMouseX;
					iSteamY = iMouseY;
				} else
				{
					iSteamSpeed = int(CMath::Static()->GetDist(iMouseX,iMouseY,iSteamX,iSteamY)/(aDrawer->Zoom()*18));
					iSteamAngle = 180+int(CMath::Static()->GetAngle(iMouseX,iMouseY,iSteamX,iSteamY));
					ANGLEFIX(iSteamAngle);
					if (iSteamSpeed > 6) iSteamSpeed = 6;
				}
			break;
		}
	} 
	else
	{
		iSpotSetSizeMode = 0;
		iSteamAdjustMode = 0;
	}

	if (aEventHandler->GetMouse().Button(CMouse::EButtonLeft))
		switch (iCurrentMode){
			case EEditObjectModePlaceSpotLight:
				if (Level()->InLevel(MBX,MBY))
					Level()->AddSpotLight((iMouseX+aDrawer->ScrXOffs()*SBS)/aDrawer->Zoom(),
										 (iMouseY+aDrawer->ScrYOffs()*SBS)/aDrawer->Zoom(),iSpotSize);
				aEventHandler->GetMouse().DecButton(CMouse::EButtonLeft);
			break;
			case EEditObjectModePlaceSteam:
				if (Level()->InLevel(MBX,MBY))
					Level()->AddSteam((iMouseX+aDrawer->ScrXOffs()*SBS)/aDrawer->Zoom(),
										 (iMouseY+aDrawer->ScrYOffs()*SBS)/aDrawer->Zoom(),iSteamAngle,iSteamSpeed);
				aEventHandler->GetMouse().DecButton(CMouse::EButtonLeft);
			break;
			case EEditObjectModePlacePL:
				if (Level()->InLevel(MBX,MBY))
				{
					CCoord<int> newPlace(MBX,MBY);
					if (iPlayerToPlace<Level()->StartPositions())
						Level()->ModifyStartPoint(iPlayerToPlace,newPlace);
					else
					{
						Level()->AddStartPoint(newPlace);
						iPlayerToPlace++;
					}
				}
				aEventHandler->GetMouse().DecButton(CMouse::EButtonLeft);
			break;
			case EEditObjectModePlaceNormalCrate:
				if (Level()->InLevel(MBX,MBY))
					Level()->AddNormalCrate((iMouseX+aDrawer->ScrXOffs()*SBS)/aDrawer->Zoom(), (iMouseY+aDrawer->ScrYOffs()*SBS)/aDrawer->Zoom(), iCrateType1,iCrateType2);
				aEventHandler->GetMouse().DecButton(CMouse::EButtonLeft);
			break;
			case EEditObjectModePlaceDMCrate:
				if (Level()->InLevel(MBX,MBY))
					Level()->AddDMCrate((iMouseX+aDrawer->ScrXOffs()*SBS)/aDrawer->Zoom(), (iMouseY+aDrawer->ScrYOffs()*SBS)/aDrawer->Zoom(), iCrateType1,iCrateType2);
				aEventHandler->GetMouse().DecButton(CMouse::EButtonLeft);
			break;
			case EEditObjectModePlaceEnemy:

				if (Level()->InLevel(MBX,MBY) &&
					Level()->LevelData((AMX-KEnemyRad)/KBlockSpriteSize,(AMY-KEnemyRad)/KBlockSpriteSize).iType==EBlockTypeFloor &&
					Level()->LevelData((AMX-KEnemyRad)/KBlockSpriteSize,(AMY+KEnemyRad)/KBlockSpriteSize).iType==EBlockTypeFloor &&
					Level()->LevelData((AMX+KEnemyRad)/KBlockSpriteSize,(AMY-KEnemyRad)/KBlockSpriteSize).iType==EBlockTypeFloor &&
					Level()->LevelData((AMX+KEnemyRad)/KBlockSpriteSize,(AMY+KEnemyRad)/KBlockSpriteSize).iType==EBlockTypeFloor &&
					Level()->LevelData((AMX-KEnemyRad/2)/KBlockSpriteSize,(AMY-KEnemyRad)/KBlockSpriteSize).iType==EBlockTypeFloor &&
					Level()->LevelData((AMX-KEnemyRad/2)/KBlockSpriteSize,(AMY+KEnemyRad)/KBlockSpriteSize).iType==EBlockTypeFloor &&
					Level()->LevelData((AMX-KEnemyRad)/KBlockSpriteSize,AMY/KBlockSpriteSize).iType==EBlockTypeFloor &&
					Level()->LevelData((AMX+KEnemyRad)/KBlockSpriteSize,AMY/KBlockSpriteSize).iType==EBlockTypeFloor &&
					Level()->LevelData(AMX/KBlockSpriteSize,AMY/KBlockSpriteSize).iType==EBlockTypeFloor)
				{
					// Test that there isn't enemies already
					for (int a = 0;a < Level()->iEnemy.size();a++)
						if (AMX-KEnemyRealRad > Level()->iEnemy.at(a).iX-KEnemyRad*2 &&
							AMY-KEnemyRealRad > Level()->iEnemy.at(a).iY-KEnemyRad*2 &&
							AMX-KEnemyRealRad < Level()->iEnemy.at(a).iX+KEnemyRad*2 &&
							AMY-KEnemyRealRad < Level()->iEnemy.at(a).iY+KEnemyRad*2
							)
						{
							occupied = true;
							break;
						}
					if (! occupied)
						Level()->AddEnemy(AMX-KEnemyRealRad, AMY-KEnemyRealRad, iEnemyType);
				}
				aEventHandler->GetMouse().DecButton(CMouse::EButtonLeft);
			break;
		}	
}
