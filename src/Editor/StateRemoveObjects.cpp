#include "StateRemoveObjects.h"
#include "EditorDrawer.h"
#include "StateHandler.h"
#include "../common/CEditableLevel.h"
#include "../common/CMath.h"
#include "../common/CEventHandler.h"
#include "../common/CSpotlight.h"
#include "../common/CSteam.h"
#include "editorapp.h"

CStateRemoveObjects::CStateRemoveObjects(CStateHandler* aStateHandler,CEditorBrush* aBrush,CEditableLevel* aLevel): CEditorState(aBrush,aLevel,aStateHandler)
{
}

CStateRemoveObjects::~CStateRemoveObjects(void)
{
}

void CStateRemoveObjects::Draw(CEditorDrawer* aDrawer)
{
	int SBS = aDrawer->ScrBlockSize();
	// Mouse pointer position in blocks at level
	int MBX = iMouseX/SBS+aDrawer->ScrXOffs();
	int MBY = iMouseY/SBS+aDrawer->ScrYOffs();
	
	int rX1 = iMouseX/SBS*SBS;
	int rY1 = iMouseY/SBS*SBS;
	int rX2 = iMouseX/SBS*SBS+SBS-1;
	int rY2 = iMouseY/SBS*SBS+SBS-1;

	aDrawer->DrawLevel(0,0,0);

	aDrawer->Write(5,5,"Remove objects mode");
}

void CStateRemoveObjects::HandleKeys(CEventHandler* aEventHandler,CEditorDrawer* aDrawer)
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
}

void CStateRemoveObjects::HandleMouse(CEventHandler* aEventHandler,CEditorDrawer* aDrawer)
{
	int SBS = aDrawer->ScrBlockSize();
	iMouseX = aEventHandler->GetMouse().XPos();
	iMouseY = aEventHandler->GetMouse().YPos();
	// Mouse pointer position in blocks at level
	int MBX = iMouseX/SBS+aDrawer->ScrXOffs();
	int MBY = iMouseY/SBS+aDrawer->ScrYOffs();
	// Objectshit
	CSpotLight* SpotPtr = Level()->iSpotLights;
	CSteam* SteamPtr = Level()->iSteams;
	int a;
	
	if (aEventHandler->GetMouse().Button(CMouse::EButtonLeft))
		if (Level()->InLevel(MBX,MBY))
		{
			// Delete Spotlight
			while (SpotPtr)
			{
				if (CMath::Static()->GetDist(SpotPtr->Location().X()*aDrawer->Zoom()-aDrawer->ScrXOffs()*SBS,
								   SpotPtr->Location().Y()*aDrawer->Zoom()-aDrawer->ScrYOffs()*SBS,
								   (float)iMouseX,(float)iMouseY) <  (15+5*SpotPtr->Size())/2*aDrawer->Zoom())
					{
					   Level()->DeleteSpotLight(SpotPtr); 
					   SpotPtr = 0; // break the loop, only one delete at the time
					}
				else 
					SpotPtr = SpotPtr->Next();
			}
			// Delete Steam
			while (SteamPtr)
			{
				if (CMath::Static()->GetDist(SteamPtr->X()*aDrawer->Zoom()-aDrawer->ScrXOffs()*SBS,
								   SteamPtr->Y()*aDrawer->Zoom()-aDrawer->ScrYOffs()*SBS,
								   iMouseX,iMouseY) <  5*aDrawer->Zoom())
					{
					   Level()->DeleteSteam(SteamPtr); 
					   SteamPtr = 0; // break the loop, only one delete at the time
					}
				else 
					SteamPtr = SteamPtr->Next();
			}
			// Delete Normal Crate
			for (a=0;a < Level()->iPlacedCratesNormal.size();a++)
				if (CMath::Static()->GetDist(Level()->iPlacedCratesNormal[a].iX*aDrawer->Zoom()-aDrawer->ScrXOffs()*SBS,
								   Level()->iPlacedCratesNormal[a].iY*aDrawer->Zoom()-aDrawer->ScrYOffs()*SBS,
								   iMouseX-(KWeaponCrateSize/2)*aDrawer->Zoom(),
								   iMouseY-(KWeaponCrateSize/2)*aDrawer->Zoom()) <  7*aDrawer->Zoom())
					   Level()->DeleteNormalCrate(a); 
			// Delete DM Crate
			for (a=0;a < Level()->iPlacedCratesDM.size();a++)
				if (CMath::Static()->GetDist(Level()->iPlacedCratesDM[a].iX*aDrawer->Zoom()-aDrawer->ScrXOffs()*SBS,
								   Level()->iPlacedCratesDM[a].iY*aDrawer->Zoom()-aDrawer->ScrYOffs()*SBS,
								   iMouseX-(KWeaponCrateSize/2)*aDrawer->Zoom(),
								   iMouseY-(KWeaponCrateSize/2)*aDrawer->Zoom()) <  7*aDrawer->Zoom())
					   Level()->DeleteDMCrate(a); 
			// Delete Enemy
			for (a=0;a < Level()->iEnemy.size();a++)
				if (CMath::Static()->GetDist(Level()->iEnemy[a].iX*aDrawer->Zoom()-aDrawer->ScrXOffs()*SBS,
								   Level()->iEnemy[a].iY*aDrawer->Zoom()-aDrawer->ScrYOffs()*SBS,

								   iMouseX-KEnemyRad*aDrawer->Zoom(),
								   iMouseY-KEnemyRad*aDrawer->Zoom()) <  KEnemyRad*aDrawer->Zoom())

					   Level()->DeleteEnemy(a); 
			
			aEventHandler->GetMouse().DecButton(CMouse::EButtonLeft);
		}
}

