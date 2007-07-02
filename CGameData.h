#ifndef __GAMEDATA_H_
#define __GAMEDATA_H_

#include <SDL_mixer.h>

#include "common/defines.h"
#include "common/CGraphicsBuffer.h"
#include "common/CGraphicsDevice.h"
#include "common/CPalette.h"

class CGameTypeData;
class COptions;
class IGameMode;
class IGUIStateController;

class CGameData
{
public:
	enum TPaletteNo
	{
		EPaletteMenu=0,
		EPaletteEFLogo,
		EPaletteTKLogo,
		EPaletteShop,
		EPaletteStatistics,
		EPaletteVictory,
		EPaletteAmount
	};

public:
	CGameData(COptions* aOptions, IGUIStateController* aGUIStateController);
	~CGameData();

	inline const CGraphicsBuffer* MenuSprite(TMenuSpriteNo iSpriteNo) const 
	{
		DEBUG_ASSERT( iSpriteNo >= 0);
		DEBUG_ASSERT( iSpriteNo < EMenuSpriteAmount );
		return(&iMenuSprite[(int)(iSpriteNo)]);
	}

	inline const CPalette* Palette(int iPaletteNo) const 
	{
		return(&iPalette[iPaletteNo]);
	}

	inline const CGameTypeData* TypeData() const  
	{
		DEBUG_ASSERT(iTypeData);
		return iTypeData; 
	}

	inline COptions* Options()
	{
		DEBUG_ASSERT(iOptions);
		return iOptions;
	}

	Mix_Chunk* Click();
	IGameMode* GameMode( int aIndex );
	int GameModes();
protected:

	Mix_Chunk* iClick;
	COptions* iOptions;
	CGameTypeData* iTypeData;
	CPalette iPalette[EPaletteAmount];
	CGraphicsBuffer iMenuSprite[EMenuSpriteAmount];
	std::vector< IGameMode* > iGameModes;
};


#endif
