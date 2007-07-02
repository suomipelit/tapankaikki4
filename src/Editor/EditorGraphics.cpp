#include <string.h>
#include <stdlib.h>
#include "EditorGraphics.h"
#include "../common/CGraphicsGenerator.h"
#include "../common/CGraphicsBuffer.h"
#include "../common/CPalette.h"
#include "../common/CFonts.h"
#include "../common/CConfigFile.h"
#include "../common/files.h"

CEditorGraphics::CEditorGraphics(void)
{
	int a;
	for (a=0;a<KWallSprites;a++)
		iWallBlocks[a] = new CGraphicsBuffer();
	iFloorBlocks = new CGraphicsBuffer();
	iWallColors = new CGraphicsBuffer();
	iFonts = new CFonts("fnts/8x8b.fnt",1);
	iPalette = new CPalette();

}

CEditorGraphics::~CEditorGraphics(void)
{
	int a;
	for (a=0;a<KWallSprites;a++)
		delete iWallBlocks[a];
	delete iFloorBlocks;
	delete iWallColors;
	delete iFonts;
	delete iPalette;
}

void CEditorGraphics::Load(const char* aEpisodeConfig, const char* aLevelConfig)
{
	int a;
	char str[255];
	CConfigFile cfgFile;

	cfgFile.Load("default.ini", aEpisodeConfig, aLevelConfig);
    iWallBlocks[0]->Load(cfgFile.Value("GameSpriteWalls").c_str(),iPalette);
	
	for (a=1;a<KWallSprites;a++)
	{
		sprintf(str,"GameSpriteWalls_%d",a);
		if (filetimestamp(cfgFile.Value("GameSpriteWalls").c_str())>
			filetimestamp(cfgFile.Value(str).c_str()))
		{
				CGraphicsGenerator::ZoomWall( *iWallBlocks[0] ,*iWallBlocks[a] , a-1);
				iWallBlocks[a]->Save(cfgFile.Value(str).c_str(),iPalette);
		}
		else
			iWallBlocks[a]->Load(cfgFile.Value(str).c_str(),iPalette);
	}
	iFloorBlocks->Load(cfgFile.Value("GameSpriteFloor").c_str(),0);
	iWallColors->Load(cfgFile.Value("GameSpriteWallClr").c_str(),0);
}

CPalette* CEditorGraphics::Palette()
{
	return iPalette;
}

CGraphicsBuffer* CEditorGraphics::FloorBlocks()
{
	return iFloorBlocks;
}

CGraphicsBuffer* CEditorGraphics::WallBlocks(int aSize)
{
	return iWallBlocks[aSize];
}

CGraphicsBuffer* CEditorGraphics::WallColors()
{
	return iWallColors;
}

CFonts* CEditorGraphics::Fonts()
{
	return iFonts;
}
