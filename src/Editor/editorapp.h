#ifndef __EDITORAPP_H
#define __EDITORAPP_H

// Editor

#define KVersion "v2.8"   // Version of editor 3.12.2004

//Dialogs colors

#define KDialogBorderColor 255
#define KDialogColor (8*16+5)

//Object add key definitions

#define KKeyPlaceSteam SDLK_w
#define KKeyPlaceSpot SDLK_q
#define KKeyPlaceNormalCrate SDLK_a
#define KKeyPlaceDMCrate SDLK_z
#define KKeyPlacePL SDLK_p
#define KKeyPlaceEnemy SDLK_e

//Object delete

#define KKeyRemoveObject SDLK_DELETE

// Edit(State) modes

const int KMoneyCrates = 4; // Random money crate types !!!
const int KMoneyAmounts[KMoneyCrates]={50,100,200,500};

enum TEditMode
{
	EEditModeEditLevel=0,			// kent„n editointi mode
	EEditModeGetBlock,				// blockin valinta mode
	EEditModeEditHeights,			// korkeus muokkaus
	EEditModeAddObjects,			// objektin lisäys mode
	EEditModeRemoveObjects,			// objektin poisto mode
	EEditModeNewLevel,              // uuden kentän luonti mode
	EEditModeEditRCData,			// randomcratedata edit mode
	EEditModeEditGeneralData,
	EEditModeHelp,
	EEditModes
};

// Random cratedata editmodes

enum TEditRCDataMode
{
	EEditNormalGameData=0,
	EEditDMGameData
};

// Object add submodes

enum TEditObjectMode
{
	EEditObjectModePlacePL=0,				// player1 asetus mode
	EEditObjectModePlaceSpotLight,		// valon asetus mode
	EEditObjectModePlaceSteam,			// savun asetus mode
	EEditObjectModePlaceNormalCrate,		// asettaa lootan
	EEditObjectModePlaceDMCrate,	// asettaa lootan
	EEditObjectModePlaceEnemy,			// asettaa vihulaisen
	EEditObjectModes
};

class CGraphicsDevice;
class CEventHandler;
class CEventFilter;
class CEditorGraphics;
class CEditorDrawer;
class CEditableLevel;
class CStateHandler;
class CEditorLevelIO;
class CEditorBrush;
class CMath;
class CEditorSelection;
class CEditorMagicSelection;
class CEditorTools;
class CEditorClipBoard;

class CEditorApp
{
public:
	CEditorApp();
	~CEditorApp();
	void Run();
	
protected:
	CEditorSelection* iLevelSelection;
    CGraphicsDevice* iGD;
	CEventHandler* iEH;
	CEventFilter* iEF;
	CEditableLevel* iLevel;
	CEditorGraphics* iGraphics;
	CEditorDrawer* iDrawer;
	CStateHandler* iStateHandler;
	CEditorLevelIO* iLevelIO;
	CEditorBrush* iBrush;
	CEditorMagicSelection* iMagicSelection;
	CEditorTools* iTools;
	CEditorClipBoard* iClipBoard;
	int iQuit;
};

#endif
