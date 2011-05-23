#include "StateEditLevel.h"
#include "EditorDrawer.h"
#include "StateGetBlock.h"
#include "EditorBrush.h"
#include "StateHandler.h"
#include "../common/CEventHandler.h"

CStateGetBlock::CStateGetBlock(CStateHandler* aStateHandler,CEditorBrush* aBrush,CEditableLevel* aLevel): CEditorState(aBrush,aLevel,aStateHandler)
{
	iType = EBlockTypeWall;
}

CStateGetBlock::~CStateGetBlock(void)
{
}

void CStateGetBlock::HandleMouse(CEventHandler* aEventHandler,CEditorDrawer* aDrawer)
{
	int SBS = aDrawer->ScrBlockSize();
	iMouseX = aEventHandler->GetMouse().XPos();
	iMouseY = aEventHandler->GetMouse().YPos();
	// Mouse pointer position in blocks at blocks area
	int MBX = (iMouseX-aDrawer->BlocksXOffs())/SBS+aDrawer->BlockScrXOffs();
	int MBY = iMouseY/SBS+aDrawer->BlockScrYOffs();

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

	if (aEventHandler->GetMouse().Button(CMouse::EButtonLeft)) 
	if (iMouseX>=aDrawer->BlocksXOffs())
		if (aDrawer->InBlocksBuffer(MBX,MBY))
		{
			Brush()->SetLeftBlock(iType,MBX+MBY*aDrawer->GetBlocksInRow());
			StateHandler()->SetState(EEditModeEditLevel);
			aEventHandler->GetMouse().DecButton(CMouse::EButtonLeft);
		}
	if (aEventHandler->GetMouse().Button(CMouse::EButtonRight)) 
	if (iMouseX>=aDrawer->BlocksXOffs())
		if (aDrawer->InBlocksBuffer(MBX,MBY))
		{
			Brush()->SetRightBlock(iType,MBX+MBY*aDrawer->GetBlocksInRow());
			StateHandler()->SetState(EEditModeEditLevel);
			aEventHandler->GetMouse().DecButton(CMouse::EButtonRight);
		}
}

void CStateGetBlock::HandleKeys(CEventHandler* aEventHandler,CEditorDrawer* aDrawer)
{
	if (aEventHandler->State(SDLK_PAGEUP) || aEventHandler->State(SDLK_PAGEDOWN))
	{
		if (iType == EBlockTypeWall) iType = EBlockTypeFloor;
		else iType = EBlockTypeWall;
		aEventHandler->State(SDLK_PAGEUP)=0;
		aEventHandler->State(SDLK_PAGEDOWN)=0;
	}
	if (aEventHandler->State(SDLK_SPACE))
	{
		StateHandler()->SetState(EEditModeEditLevel);
		aEventHandler->State(SDLK_SPACE) = 0;
	}
	
	if (aEventHandler->State(SDLK_KP_PLUS)) {aDrawer->ZoomIn();aEventHandler->State(SDLK_KP_PLUS)=0;}
	if (aEventHandler->State(SDLK_KP_MINUS)) {aDrawer->ZoomOut();aEventHandler->State(SDLK_KP_MINUS)=0;}
	if (aEventHandler->State(SDLK_UP)) aDrawer->SetBlockScrYOffs(aDrawer->BlockScrYOffs()-1);
	if (aEventHandler->State(SDLK_DOWN)) aDrawer->SetBlockScrYOffs(aDrawer->BlockScrYOffs()+1);
	if (aEventHandler->State(SDLK_LEFT)) aDrawer->SetBlockScrXOffs(aDrawer->BlockScrXOffs()-1);
	if (aEventHandler->State(SDLK_RIGHT)) aDrawer->SetBlockScrXOffs(aDrawer->BlockScrXOffs()+1);

}

void CStateGetBlock::Draw(CEditorDrawer* aDrawer)
{
	int SBS = aDrawer->ScrBlockSize();
	// Mouse pointer position in blocks at blocks area
	int MBX = (iMouseX-aDrawer->BlocksXOffs())/SBS+aDrawer->BlockScrXOffs();
	int MBY = iMouseY/SBS+aDrawer->BlockScrYOffs();
	int rX1,rX2,rY1,rY2;

	rX1 = (iMouseX-aDrawer->BlocksXOffs())/SBS*SBS+aDrawer->BlocksXOffs();
	rY1 = iMouseY/SBS*SBS;
	rX2 = (iMouseX-aDrawer->BlocksXOffs())/SBS*SBS+SBS-1+aDrawer->BlocksXOffs();
	rY2 = iMouseY/SBS*SBS+SBS-1;

	aDrawer->DrawBlocks(iType);
	if (iMouseX>=aDrawer->BlocksXOffs())
		if (aDrawer->InBlocksBuffer(MBX,MBY))
			aDrawer->DrawRect(rX1,rY1,rX2,rY2,255);

	if (iType==EBlockTypeWall)
		aDrawer->Write(5,5,"Get wall block");
	else 
		aDrawer->Write(5,5,"Get floor block");
}
