#ifndef __SOUND_H_
#define __SOUND_H_

#define PAN_MIDDLE 0
#define PAN_LEFT -255
#define PAN_RIGHT 255

const float KFullVolume=1.0f;

#include <SDL_mixer.h>
#include "texts.h"
#include "common/files.h"
#include "common/CCoord.h"

class CDynGameData;
class IPositionalSoundObserver;

class CSoundPlayer
{
public:
	CSoundPlayer();
	~CSoundPlayer();

	void PlaySample(Mix_Chunk* aEffect,float aVolume, int aPan);
//	void PlayPlayerSample(enum TSoundEffect aEffect,float aVolume, int aPlayerNo, int aAmountPlayers);
	void PlayPosSample(enum TIngameSoundEffect aEffectEnum, float aVolume, CCoord<int> aPos);
	void SetObserver(IPositionalSoundObserver* aObserver);
	void StopMusic();
	void GameMusic( const char* aSoundtrackDir, int aSongNumber );
	void MenuMusic();
	void SetDynData( CDynGameData* aDynData );

	bool Initialized();

	void SetTheme(const char* aTheme);
	void SetFXVolume(float aVolume);
	void SetMusicVolume(float aVolume);
	void SetReverseStereo(int aBool);

	float FXVolume();
	float MusicVolume();
	int ReverseStereo();
	const char* Theme();

private:
	void StereoShaper(const CCoord<float>& aObserver, const CCoord<float>& aEffect, int& aLeft, int& aRight );
	void Initialize();
	void LoadMusic(const char *aFilename);
	void PlayMusic(int aLooping);
	char* GetSongFilename( const char* aSoundtrackDir, int aSongNumber );

	bool iInitialized;
	bool iSubsystemInitialized;
	Mix_Music* iMusic;
	//Mix_Chunk* iEffects[AmountOfSoundEffects];
	int iSongCounter;
	char iTheme[FILENAME_MAX];
	float iFXVolume, iMusicVolume;
	int iReverseStereo;
	IPositionalSoundObserver* iObserver;
	CDynGameData* iDynData;
};

#endif


