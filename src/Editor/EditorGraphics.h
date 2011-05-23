#ifndef __EDITORGRAPHICS_H_
#define __EDITORGRAPHICS_H_

#define KWallSprites 6

class CGraphicsBuffer;
class CPalette;
class CFonts;

class CEditorGraphics
{
public:
	CEditorGraphics(void);
	~CEditorGraphics(void);
	void Load(const char* aEpisodeConfig="default.ini", const char* aLevelConfig="deafult.ini");
	CPalette* Palette();
	CGraphicsBuffer* FloorBlocks();
	CGraphicsBuffer* WallBlocks(int aSize);
	CGraphicsBuffer* WallColors();
	CFonts* Fonts();

protected:
	CGraphicsBuffer*  iFloorBlocks;
	CGraphicsBuffer*  iWallBlocks[KWallSprites];
	CGraphicsBuffer*  iWallColors;

	CFonts* iFonts;
	CPalette* iPalette;
};

#endif
