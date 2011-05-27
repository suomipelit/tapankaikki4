#include "StateEditHeights.h"
#include "EditorDrawer.h"
#include "EditorBrush.h"
#include "EditorGraphics.h"
#include "StateHandler.h"
#include "EditorSelection.h"
#include "EditorMagicSelection.h"
#include "EditorTools.h"
#include "../common/CEditableLevel.h"
#include "../common/CEventHandler.h"
#include "../common/CMath.h"

CStateEditHeights::CStateEditHeights(CStateHandler* aStateHandler,CEditorTools* aTools,CEditableLevel* aLevel): CEditorState(aTools->Brush(),aLevel,aStateHandler)
{
	iSelectionMode = 0;
	iTools = aTools;
}

CStateEditHeights::~CStateEditHeights(void)
{
}

void CStateEditHeights::Draw(CEditorDrawer* aDrawer)
{
	int SBS = aDrawer->ScrBlockSize();
	// Mouse pointer position in blocks at level
	int MBX = iMouseX/SBS+aDrawer->ScrXOffs();
	int MBY = iMouseY/SBS+aDrawer->ScrYOffs();
	int MRX = iMouseX/SBS*SBS;
	int MRY = iMouseY/SBS*SBS;
	int rX1,rX2,rY1,rY2;

	if (iSelectionMode==2)
	{
		if (iSelectionX<=MBX)
		{
			rX1 = (iSelectionX-aDrawer->ScrXOffs())*SBS;
			rX2 = MRX+SBS-1;
		} else 
		{
			rX1 = MRX;
			rX2 = (iSelectionX-aDrawer->ScrXOffs())*SBS+SBS-1;
		}
		if (iSelectionY<=MBY)
		{
			rY1 = (iSelectionY-aDrawer->ScrYOffs())*SBS;
			rY2 = MRY+SBS-1;
		} else 
		{
			rY1 = MRY;
			rY2 = (iSelectionY-aDrawer->ScrYOffs())*SBS+SBS-1;
		}
	} 
	else 
	{
		rX1 = MRX;
		rY1 = MRY;
		rX2 = MRX+SBS-1;
		rY2 = MRY+SBS-1;
	}

	aDrawer->DrawLevel(1,MBX,MBY);
	
	if (iSelectionMode == 0){
	if (Level()->InLevel(MBX,MBY))
		aDrawer->DrawRect(rX1,rY1,rX2,rY2,255);
	else
		aDrawer->DrawRect(rX1,rY1,rX2,rY2,160);
	}
	else
		aDrawer->DrawRect(rX1,rY1,rX2,rY2,255);

	if (iSelectionMode > 0)
	{
		if (iRemoveSelectionMode)
			aDrawer->Write(5,5,"Edit heights - Remove from selection (right mouse button = magic wand)");
		else 
		if (iAddSelectionMode)
			aDrawer->Write(5,5,"Edit heights - Add to selection (right mouse button = magic wand)");
		else
			aDrawer->Write(5,5,"Edit heights - Make a selection (right mouse button = magic wand)");
	}
	else
		aDrawer->Write(5,5,"Edit heights");

	aDrawer->DrawMouseCoords(MBX,MBY);
}

void CStateEditHeights::HandleKeys(CEventHandler* aEventHandler,CEditorDrawer* aDrawer)
{
	if (aEventHandler->State(SDLK_F5)){
		aDrawer->ToggleMap();
		aEventHandler->State(SDLK_F5)=0;
	}

	if (aEventHandler->State(SDLK_F1)){
		iStateHandler->SetState(EEditModeHelp);
		aEventHandler->State(SDLK_F1)=0;
	}

	if (aEventHandler->State(SDLK_KP_PLUS)) {aDrawer->ZoomIn();aEventHandler->State(SDLK_KP_PLUS)=0;}
	if (aEventHandler->State(SDLK_KP_MINUS)) {aDrawer->ZoomOut();aEventHandler->State(SDLK_KP_MINUS)=0;}
	if (aEventHandler->State(SDLK_UP)) aDrawer->SetScrYOffs(aDrawer->ScrYOffs()-1);
	if (aEventHandler->State(SDLK_DOWN)) aDrawer->SetScrYOffs(aDrawer->ScrYOffs()+1);
	if (aEventHandler->State(SDLK_LEFT)) aDrawer->SetScrXOffs(aDrawer->ScrXOffs()-1);
	if (aEventHandler->State(SDLK_RIGHT)) aDrawer->SetScrXOffs(aDrawer->ScrXOffs()+1);
	if (aEventHandler->State(SDLK_SPACE)) {StateHandler()->SetState(EEditModeGetBlock);aEventHandler->State(SDLK_SPACE)=0;}
	if (aEventHandler->State(SDLK_RETURN)) {StateHandler()->SetState(EEditModeEditLevel);aEventHandler->State(SDLK_RETURN)=0;}
	if (aEventHandler->State(SDLK_LSHIFT) || aEventHandler->State(SDLK_RSHIFT) || 
		aEventHandler->State(SDLK_LCTRL) || aEventHandler->State(SDLK_RCTRL) || 
		aEventHandler->State(SDLK_LALT)) 
		{
			if (iSelectionMode == 0) 
				iSelectionMode = 1;
		}
	else iSelectionMode = 0;
	if (aEventHandler->State(SDLK_LSHIFT) || aEventHandler->State(SDLK_RSHIFT)) iAddSelectionMode = true;
	else iAddSelectionMode = false;
	if (aEventHandler->State(SDLK_LALT)) iRemoveSelectionMode = true;
	else iRemoveSelectionMode = false;
	// Object adds
	if (aEventHandler->State(KKeyPlaceSpot)) 
	{
		StateHandler()->SetState(EEditModeAddObjects);
		StateHandler()->CurrentState()->SetMode(EEditObjectModePlaceSpotLight);
		aEventHandler->State(KKeyPlaceSpot)=0;
	}
	if (aEventHandler->State(KKeyPlaceSteam)) 
	{
		StateHandler()->SetState(EEditModeAddObjects);
		StateHandler()->CurrentState()->SetMode(EEditObjectModePlaceSteam);
		aEventHandler->State(KKeyPlaceSteam)=0;
	}
	if (aEventHandler->State(KKeyPlacePL)) 
	{
		StateHandler()->SetState(EEditModeAddObjects);
		StateHandler()->CurrentState()->SetMode(EEditObjectModePlacePL);
		aEventHandler->State(KKeyPlacePL)=0;
	}
	if (aEventHandler->State(KKeyPlaceNormalCrate)) 
	{
		StateHandler()->SetState(EEditModeAddObjects);
		StateHandler()->CurrentState()->SetMode(EEditObjectModePlaceNormalCrate);
		aEventHandler->State(KKeyPlaceNormalCrate)=0;
	}
	if (aEventHandler->State(KKeyPlaceDMCrate)) 
	{
		StateHandler()->SetState(EEditModeAddObjects);
		StateHandler()->CurrentState()->SetMode(EEditObjectModePlaceDMCrate);
		aEventHandler->State(KKeyPlaceDMCrate)=0;
	}
	if (aEventHandler->State(KKeyPlaceEnemy)) 
	{
		StateHandler()->SetState(EEditModeAddObjects);
		StateHandler()->CurrentState()->SetMode(EEditObjectModePlaceEnemy);
		aEventHandler->State(KKeyPlaceEnemy)=0;
	}
	if (aEventHandler->State(KKeyRemoveObject)) 
	{
		StateHandler()->SetState(EEditModeRemoveObjects);
		aEventHandler->State(KKeyRemoveObject)=0;
	}
}

void CStateEditHeights::HandleMouse(CEventHandler* aEventHandler,CEditorDrawer* aDrawer)
{
	int SBS = aDrawer->ScrBlockSize();
	iMouseX = aEventHandler->GetMouse().XPos();
	iMouseY = aEventHandler->GetMouse().YPos();
	// Mouse pointer position in blocks at level
	int MBX = iMouseX/SBS+aDrawer->ScrXOffs();
	int MBY = iMouseY/SBS+aDrawer->ScrYOffs();

	// MOUSE ZOOOM
	if (aEventHandler->GetMouse().Button(CMouse::EButtonWheelDown)) 
		{
			aDrawer->ZoomIn();
			aEventHandler->GetMouse().DecButton(CMouse::EButtonWheelDown);
		}
	if (aEventHandler->GetMouse().Button(CMouse::EButtonWheelUp))
		{
			aDrawer->ZoomOut();
			aEventHandler->GetMouse().DecButton(CMouse::EButtonWheelUp);
		}

	if (iSelectionMode==0)
	{
		if (aEventHandler->GetMouse().Button(CMouse::EButtonLeft)) 
			Brush()->ModifyHeight(MBX,MBY,1);
		if (aEventHandler->GetMouse().Button(CMouse::EButtonRight)) 
			Brush()->ModifyHeight(MBX,MBY,-1);
		aEventHandler->GetMouse().DecButton(CMouse::EButtonLeft);
		aEventHandler->GetMouse().DecButton(CMouse::EButtonRight);
	} else
	{
		if (iSelectionMode==1){
			if (aEventHandler->GetMouse().Button(CMouse::EButtonLeft)) 
				if (Level()->InLevel(MBX,MBY))
				{
					iSelectionX = MBX;
					iSelectionY = MBY; 
					iSelectionMode++;
				}
			if (aEventHandler->GetMouse().Button(CMouse::EButtonRight))
			{
				if (iRemoveSelectionMode)
					iTools->MagicSelection()->DeSelectArea(MBX,MBY);
				else
				if (iAddSelectionMode)			
					iTools->MagicSelection()->SelectArea(MBX,MBY);
				else {
					iTools->Selection()->Clear();				
					iTools->MagicSelection()->SelectArea(MBX,MBY);
				}
			}
		}
		if (iSelectionMode==2 && ! aEventHandler->GetMouse().Button(CMouse::EButtonLeft))
		{
			int sx = iSelectionX;
			int sy = iSelectionY;
			int ex = MBX;
			int ey = MBY;
			if (sx > ex) CMath::Swap(sx,ex);
			if (sy > ey) CMath::Swap(sy,ey);
			if (iLevel->InLevel(sx,sy) || iLevel->InLevel(ex,ey))
			{
				if (sx < 0) sx = 0;
				if (sy < 0) sy = 0;
				if (ex >= iLevel->Width()) ex = iLevel->Width() - 1;
				if (ey >= iLevel->Height()) ey = iLevel->Height() - 1;
				if (iRemoveSelectionMode)
					iTools->Selection()->RemoveRect(sx,sy,ex,ey);
				else
					if (iAddSelectionMode)
						iTools->Selection()->AddRect(sx,sy,ex,ey);
					else {
						iTools->Selection()->Clear();
						iTools->Selection()->AddRect(sx,sy,ex,ey);
					}
			}
			iSelectionMode = 0;
		}
	}
}
