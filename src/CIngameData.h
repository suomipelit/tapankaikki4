#ifndef __CINGAMEDATA_H
#define __CINGAMEDATA_H

#include <SDL_mixer.h>
#include <vector>

#include "common/CGraphicsBuffer.h"
#include "common/CPalette.h"
#include "common/defines.h"
#include "IGameMode.h"

#define KRamboSprites 4
#define KWallSprites 6
#define KEnemySprites 10

class CDynGameData;

class CIngameData
{
public:
	CIngameData( CDynGameData* aDynData );
	~CIngameData();

	void LoadIngameData(const std::string& aEpisodeConfig, const std::string& aLevelConfig);
	void UnloadIngameData();

	inline const CGraphicsBuffer* EnemySprite(int iSpriteNo) const 
	{
		return GameSprite( TGameSprite(iSpriteNo + EGameSpriteEnemy0) );
	}
	inline const CGraphicsBuffer* RamboSprite(int iSpriteNo) const 
	{
		return GameSprite( TGameSprite(iSpriteNo + EGameSpriteRambo1) );
	}

	inline Mix_Chunk*  SoundFX(TIngameSoundEffect aEffect)
	{
		DEBUG_ASSERT( aEffect >= 0);
		DEBUG_ASSERT( aEffect < ESoundEffectAmount );
		DEBUG_ASSERT( iIngameFilesLoaded );
		return iSoundEffects[ aEffect ];
	}

	inline const CGraphicsBuffer* GameSprite(TGameSprite iSpriteNo) const 
	{
		DEBUG_ASSERT( iSpriteNo >= 0);
		DEBUG_ASSERT( iSpriteNo < EGameSpriteAmount );
		DEBUG_ASSERT( iIngameFilesLoaded );
		return(&iGameSprite[(int)(iSpriteNo)]);
	}

	inline const CPalette& Palette() const
	{
		DEBUG_ASSERT( iIngameFilesLoaded );
		return iPalette;
	}

	const char* RandomKillText() const;
	const char* RandomSuicideText() const;

	std::string Soundtrack() const;
	int SongNumber() const;

protected:

	CGraphicsBuffer iGameSprite[EGameSpriteAmount];
	CPalette iPalette;
	Mix_Chunk* iSoundEffects[ESoundEffectAmount];
	std::vector< std::string > iKillTexts;
    std::vector< std::string > iSuicideTexts;

	bool iIngameFilesLoaded;
	std::string iSoundtrack;
	int iSongNumber;
};

#endif
