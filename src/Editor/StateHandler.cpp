#include "StateHandler.h"
#include "StateEditLevel.h"
#include "StateGetBlock.h"
#include "StateEditHeights.h"
#include "StateAddObjects.h"
#include "StateRemoveObjects.h"
#include "StateNewLevel.h"
#include "StateEditRCData.h"
#include "StateEditGeneralData.h"
#include "StateHelp.h"
#include "EditorTools.h"
#include "EditorClipBoard.h"
#include "../common/error.h"

//class CEditorSelection;

CStateHandler::CStateHandler(CEditorTools* aTools,CEditableLevel* aLevel,CEditorClipBoard* aClipBoard)
{
	int a;
	for (a=0;a<EEditModes;a++)
		iStates[a] = 0;
	iStates[EEditModeEditLevel]= new CStateEditLevel(this,aTools,aLevel,aClipBoard);
	iStates[EEditModeGetBlock]= new CStateGetBlock(this,aTools->Brush(),aLevel);
	iStates[EEditModeEditHeights]= new CStateEditHeights(this,aTools,aLevel);
	iStates[EEditModeAddObjects]= new CStateAddObjects(this,aTools->Brush(),aLevel);
	iStates[EEditModeRemoveObjects]= new CStateRemoveObjects(this,aTools->Brush(),aLevel);
	iStates[EEditModeNewLevel]= new CStateNewLevel(this,aTools->Brush(),aLevel);
	iStates[EEditModeEditRCData]= new CStateEditRCData(this,aTools->Brush(),aLevel);
	iStates[EEditModeEditGeneralData]= new CStateEditGeneralData(this,aTools->Brush(),aLevel);
	iStates[EEditModeHelp]= new CStateHelp(this,aTools->Brush(),aLevel);

    iCurrentEditMode=(TEditMode)0;
}

CStateHandler::~CStateHandler(void)
{
	int a;
	for (a=0;a<EEditModes;a++)
		if (iStates[a]) delete iStates[a];
}

void CStateHandler::SetPrevious()
{
	iCurrentEditMode = iPreviousEditMode;
}

void CStateHandler::SetState(TEditMode aEditMode)
{
	// Don't set mode if it's already set
	if (aEditMode == iCurrentEditMode) return;

	if (iStates[aEditMode])
	{
		iPreviousEditMode = iCurrentEditMode;
		iCurrentEditMode = aEditMode;
	}
	else 
		error("State not initialized !");
}

CEditorState* CStateHandler::CurrentState()
{
	return iStates[iCurrentEditMode];
}
