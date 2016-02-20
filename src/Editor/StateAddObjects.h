#ifndef __STATEADDOBJECTS_H
#define __STATEADDOBJECTS_H

#include "EditorState.h"
#include "editorapp.h"

class CStateAddObjects : public CEditorState
{
public:
	CStateAddObjects(CStateHandler* aStateHandler,CEditorBrush* aBrush,CEditableLevel* aLevel);
	~CStateAddObjects(void);
public:
	void Draw(CEditorDrawer* aDrawer);
	void HandleKeys(CEventHandler* aEventHandler,CEditorDrawer* aDrawer);
	void HandleMouse(CEventHandler* aEventHandler,CEditorDrawer* aDrawer);
	void SetMode(int aEditMode);
protected:
	TEditObjectMode iCurrentMode;
	// SpotLight variables
	int iSpotSize;
	int iSpotSetSizeMode; // boolean
	int iSpotSizeX,iSpotSizeY;
	// Steam variables
	int iSteamSpeed;
	int iSteamAngle;
	int iSteamAdjustMode; // boolean
	int iSteamX,iSteamY;
	// Crates variables
	int iCrateType1;
	int iCrateType2;
	// Enemytype
	int iEnemyType;
	int iMouseX;
	int iMouseY;
	// Number of player to place startpoint
	int iPlayerToPlace;
};

#endif
