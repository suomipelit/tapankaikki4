#ifndef __EDITORDRAWER_H_
#define __EDITORDRAWER_H_
#include <vector>

#include "../common/CFonts.h"

class CGraphicsDevice;
class CEditorGraphics;
class CGraphicsBuffer;
class CEditableLevel;
class CFonts;
class CMath;
class CGameTypeData;
class CEditorSelection;
class CEditorBrush;
class CEditorClipBoardBlock;

#define KEnemyRad 9 // used in drawing
#define KEnemyRealRad 14 // this is real radius of enemy (sprite)
#define KEnemyColor 16*9
#define KWeaponCrateSize 14
#define KBulletCrateSize 10

// 3D Walls are drawn in four different sections
enum EDrawSect  {
	UpLeft=0,UpRight,DownLeft,DownRight
};

class CEditorDrawer
{
public:
	CEditorDrawer(CGraphicsDevice* aGD,CEditorGraphics* aGraphics,CEditableLevel* aLevel, CEditorSelection* aLevelSelection, CEditorBrush* aBrush);
	~CEditorDrawer(void);
	void ResetAnimationFlag(); // Call this at the start at every loop to save processor time
	void ToggleMap();
	void Resize(int aWidth,int aHeight);
	void DrawBlock(int aX,int aY,int AType, int aNumber,int aSize,int aZoom);
	void DrawLevelType(int aXOffs,int aYOffs,int aType);
	void Draw3DWalls(int aSelX,int aSelY);
	void DrawCircle(int aX,int aY,int aR,unsigned char aCol);
	void DrawRect(int aX1,int aY1,int aX2,int aY2, unsigned char aColor);
	void DrawLiveRect(int aX1,int aY1,int aX2,int aY2, unsigned char aColor, int aCounter, int aLineLength);
	void DrawClipBoard(int aX,int aY,std::vector<CEditorClipBoardBlock>* aBlocks);
	void DrawEnemy(int aX,int aY,int aType);
	void DrawSpot(int aX,int aY,int aSize);
	void DrawSteam(int aX,int aY,int aAngle,int aSpeed);
	void DrawCrate(int aX,int aY,int aType1,int aType2,unsigned char aColor);
	void DrawObjects();
	void DrawNewLevelPrompt(char* aWidth,char* aHeight);
	void DrawRCData(int aCursorXPos,int aCursorYPos,int aMode);
	void DrawGeneralData(int aCursorXPos,int aCursorYPos,bool aCommentMode);
	bool UpdateScreen(); // Returns true if screen needs to be refreshed at next loop
	void DrawBrush(int aX,int aY);
	void DrawLevel(int a3D,int aSelX,int aSelY);
	void DrawInfo();
	int BlocksXOffs();
	void DrawBlocks(int aType);
	int ScrBlockSize() {return iScrBlockSize;};
	void SetZoom(int aZoom);
	void ZoomIn();
	void ZoomOut();
	int Zoom() {return iZoom;};
	void UpdateScrBlockDimensions();
	void UpdateCaption();
	int ScrXOffs() {return iScrXOffs;};
	int ScrYOffs() {return iScrYOffs;};
	void SetScrXOffs(int aScrXOffs);
	void SetScrYOffs(int aScrYOffs);
	int BlockScrXOffs() {return iBlockScrXOffs;};
	int BlockScrYOffs() {return iBlockScrYOffs;};
	void SetBlockScrXOffs(int aScrXOffs);
	void SetBlockScrYOffs(int aScrYOffs);
	void Write(int aX,int aY,const char* aText,CFonts::TVertAlign aVAlign = CFonts::EDrawBelow, CFonts::THorizAlign aHAlign = CFonts::EDrawToRight);
	void Write(int aX,int aY,int aNumber,CFonts::TVertAlign aVAlign = CFonts::EDrawBelow, CFonts::THorizAlign aHAlign = CFonts::EDrawToRight);
	void DrawFileInfo();
	void DrawMouseCoords(int aX,int aY);
	int FontHeight();
	int InBlocksBuffer(int aX,int aY); // returns 1 if coords in blocksbuffer
	int GetBlocksInRow();
private:
	void DrawSelection(int a3D,int aXOffs,int aYOffs);
	void ZoomPixel(int aX,int aY,unsigned char aColor,int aZoom);
	void DrawMap();
	void Draw3DBlockRow(int aX1,int aX2,int aY,EDrawSect aSection,int aSelX,int aSelY);
	void DrawFace(int aX1,int aY1,int aX2,int aY2,int aX3,int aY3,int aX4,int aY4,unsigned char aColor);
	int NeedToDraw(int aX,int aY,int aXAdd,int aYAdd);
private:
	CGameTypeData* iTypeData;
	CGraphicsDevice* iGD;
	CEditorGraphics* iGraphics;
	CGraphicsBuffer* iDrawBuffer;
	CEditableLevel* iLevel;
	CEditorSelection* iLevelSelection;
	CEditorBrush* iBrush;

	int iZoom,iScrBlockSize; // Zoom multiplier, Block size on screen
	int iScrBlockWidth,iScrBlockHeight; // Screen size in blocks
	int iScrXOffs,iScrYOffs; // Level edit offset
	int iBlockScrXOffs,iBlockScrYOffs; // Get Block offset
	bool iMap; // Map toggle
	bool iAnimation; // Toggle this flag on if there is animation on screen
};
#endif
