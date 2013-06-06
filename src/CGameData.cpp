#include <string.h>

#include "common/CGameTypeData.h"

#include "CGameData.h"
#include "CGameModeCooperative.h"
#include "CGameModeDeathmatch.h"

namespace
{
	struct TMenuSpriteData
	{
		char *filename; // filename
		int palette;	// Number of palette, -1 if none
	};

	TMenuSpriteData KMenuSpriteFiles[EMenuSpriteAmount]=
	{
		{"efps/eflogo.pcx",		CGameData::EPaletteEFLogo},
		{"efps/menu.pcx",		CGameData::EPaletteMenu},
		{"efps/bslogo.pcx",		CGameData::EPaletteTKLogo},
		{"efps/staticsback.efp",CGameData::EPaletteStatistics},
		{"efps/shopback_normal.efp",CGameData::EPaletteShop},
		{"efps/shopback_hilite.efp",-1},
		{"efps/shopback_hollow.efp",-1},
		{"efps/rectangle.pcx",	-1},
		{"efps/victory.pcx",CGameData::EPaletteVictory},
	};
}

CGameData::~CGameData()
{
	Mix_FreeChunk(iClick);
	delete iTypeData;
	std::vector< IGameMode* >::iterator iter = iGameModes.begin();

	while ( iter != iGameModes.end() )
	{
		delete *iter;
		++iter;
	}

	iGameModes.clear();
}

CGameData::CGameData(COptions* aOptions, IGUIStateController* aGUIStateController): iOptions(aOptions)
{
	int a;
	memset(iPalette,0,sizeof(iPalette));
	iTypeData = new CGameTypeData;

	// Load sprites & palettes
	for (a=0;a<EMenuSpriteAmount;a++)
	{
		if (KMenuSpriteFiles[a].palette==-1)
			iMenuSprite[a].Load(KMenuSpriteFiles[a].filename, NULL);
		else
		{
			iMenuSprite[a].Load(KMenuSpriteFiles[a].filename, &iPalette[KMenuSpriteFiles[a].palette]);

#ifdef _DEBUG
			bool n = false;
			for (int x=0;x<256&&!n;x++)
				if (iPalette[KMenuSpriteFiles[a].palette].Color(x).r!=0||
					iPalette[KMenuSpriteFiles[a].palette].Color(x).g!=0||
					iPalette[KMenuSpriteFiles[a].palette].Color(x).b!=0)
					n = true;

			DEBUG_ASSERT( n );
#endif
		}
	}

	iClick = Mix_LoadWAV("wavs/klik.wav");
	ASSERTPTR( iClick );

	iGameModes.push_back( new CGameModeCooperative( aGUIStateController ));
	iGameModes.push_back( new CGameModeDeathmatch( aGUIStateController ));
}

Mix_Chunk* CGameData::Click()
{
	return iClick;
}


IGameMode* CGameData::GameMode( int aIndex )
{
	return iGameModes[ aIndex ];
}

int CGameData::GameModes()
{
	return iGameModes.size();
}

