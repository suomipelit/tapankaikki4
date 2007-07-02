#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include "common/error.h"
#include "common/CMath.h"

#include "CDynGameData.h"
#include "CSoundPlayer.h"
#include "IPositionalSoundObserver.h"
#include "CIngameData.h"
#include "CLevelRuntimeData.h"

namespace
{
	const int KFadeOutTime = 600;
	const int KFadeInTime = 300;
};

CSoundPlayer::CSoundPlayer(): 
	iSongCounter(0), iFXVolume(-1), iMusicVolume(-1), iMusic(NULL), iReverseStereo(-1),
	iInitialized(false), iSubsystemInitialized(false)
{
	DEBUG0("CSoundPlayer::CSoundPlayer()\n");
	memset(iTheme,0,sizeof(iTheme));

    Initialize();
}

CSoundPlayer::~CSoundPlayer()
{
	DEBUG0("CSoundPlayer::~CSoundPlayer()\n");

	StopMusic();
	Mix_FreeMusic(iMusic);
	
	Mix_CloseAudio();
}

void CSoundPlayer::Initialize()
{
	DEBUG0("CSoundPlayer::Initialize()\n");
	if (iInitialized)
		return;

	LOG0("Trying to init. SDL_INIT_AUDIO\n");
	if (!iSubsystemInitialized)
	{
		if (SDL_InitSubSystem(SDL_INIT_AUDIO)<0) 
		{
			LOG1("CSoundPlayer::Initialize: Unable to init SDL_INIT_AUDIO subsystem: %s\n",SDL_GetError());
			return;
		}
		else 
			iSubsystemInitialized = true;
	}

	int audio_rate=44100;
	Uint16 audio_format=MIX_DEFAULT_FORMAT;
	int audio_channels=2;

	LOG0("Trying to Mix_OpenAudio\n");
	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels,2048) < 0) 
	{
		LOG1("CSoundPlayer::Initialize: Couldn't open audio: %s\n", SDL_GetError());
		return;
	}

	LOG0("Trying to Mix_QuerySpec\n");
	if (Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels)==0)
	{
		error("CSoundPlayer::Initialize: Mix_QuerySpec failed!");
	}

	LOG0("Trying to Mix_AllocateChannels\n");
	if (Mix_AllocateChannels(16)!=16)
		LOG0("CSoundPlayer::Initialize: Mix_AllocateChannels could not give as many channels as desired\n");

	iInitialized = true;
	LOG0("CSoundPlayer::Initialize() done successfully.\n");
}

bool CSoundPlayer::Initialized()
{
	return iInitialized;
}

void CSoundPlayer::LoadMusic(const char *aFileName)
{
	DEBUG0("CSoundPlayer::LoadMusic()\n");
	if (!iInitialized)
		return;

	if (iMusic)
		Mix_FreeMusic(iMusic);

	if (exists(aFileName))
	{
		iMusic=Mix_LoadMUS(aFileName);

		if (iMusic==NULL) 
			error("CSoundPlayer::CSoundPlayer: Musicmodule NULL!\n");
	}
}

void CSoundPlayer::PlayMusic(int aLooping)
{
	if (!iInitialized)
		return;

	StopMusic();
	Mix_FadeInMusic(iMusic,aLooping, KFadeInTime );
	SetMusicVolume(iMusicVolume);
}

//void CSoundPlayer::PlayPlayerSample(enum TSoundEffect aEffect, float aVolume, int aPlayerNo, int aAmountPlayers)
//{
//	if (!iInitialized)
//		return;
//
//	if (aAmountPlayers==1)
//	{
//		PlaySample(aEffect,aVolume,PAN_MIDDLE);
//		return;
//	}
//
//	if (aAmountPlayers==2||aAmountPlayers==4)
//	{
//		if (aPlayerNo%2==0) 
//			PlaySample(aEffect,aVolume,PAN_LEFT);
//		else 
//			PlaySample(aEffect,aVolume,PAN_RIGHT);
//		return;
//	}
//
//	if (aAmountPlayers==3)
//	{
//		if (aPlayerNo==0) 
//			PlaySample(aEffect,aVolume,PAN_LEFT);
//		else if (aPlayerNo==1)
//			PlaySample(aEffect,aVolume,PAN_RIGHT);
//		else
//			PlaySample(aEffect,aVolume,PAN_MIDDLE);
//
//		return;
//	}
//
//	ASSERT(0);
//}

void CSoundPlayer::PlaySample(Mix_Chunk* aEffect,float aVolume, int aPan)
{
	ASSERTPTR( aEffect );
	if (!iInitialized)
		return;

	int chan;

	chan=Mix_PlayChannel(-1,aEffect,0);
	Mix_Volume(chan,(int)(aVolume*iFXVolume*127.0));
	if (aPan<0)
		Mix_SetPanning(chan,255,255+aPan);
	if (aPan>0)
		Mix_SetPanning(chan,255-aPan,255);
}

void CSoundPlayer::PlayPosSample(enum TIngameSoundEffect aEffect,float aVolume, CCoord<int> aPos)
{
	ASSERTPTR( iDynData );
	if (aEffect==ESoundEffectNoSound) return;

	if (iObserver)
		iObserver->SoundHeard(aEffect,aVolume,aPos);

	if (!iInitialized)
		return;

	if (iDynData->LocalPlayers().size()!=1)
	{
		PlaySample(iDynData->LevelRuntime()->IngameData()->SoundFX(aEffect),aVolume,PAN_MIDDLE);
		return;
	}
	
	int chan=Mix_PlayChannel(-1,iDynData->LevelRuntime()->IngameData()->SoundFX(aEffect),0);
	Mix_Volume(chan,(int)(aVolume*iFXVolume*127.0));

	int left, right;

	StereoShaper( iDynData->LocalPlayers()[0]->Location(),aPos, left, right );

	Mix_SetPanning(chan, left, right );
}

void CSoundPlayer::StereoShaper(const CCoord<float>& aObserver, const CCoord<float>& aEffect, int& aLeft, int& aRight )
{
	const float dist = CMath::GetDist(aObserver, aEffect)/2;
	const float xdist = (aObserver.X()-aEffect.X());
	const float ydist = (aObserver.Y()-aEffect.Y())*0.7;

	// let's quit in easy cases
	if (dist <= 0)
	{
		aLeft = 128;
		aRight = 128;
		return;
	};

	// Formula from excel (models from this formula look and sound ok)
	//=MIN(SQRT(MAX(128*128-B$2^2-($A3-64)^2;0))+10000/SQRT(B$2^2+($A3-64)^2);255)

	aLeft = int (min( sqrt( max(128*128-ydist*ydist-(xdist-64)*(xdist-64),0.0f) ) + 
			10000/sqrt( ydist*ydist+(xdist-64)*(xdist-64)), 255.0 ));
	aRight = int (min( sqrt( max(128*128-ydist*ydist-(xdist+64)*(xdist+64),0.0f) ) + 
			10000/sqrt( ydist*ydist+(xdist+64)*(xdist+64)), 255.0 ));

	// Let's cut the heaps...
	if (aLeft+aRight>256)
	{
		int dif = aLeft+aRight - 256;
		aLeft-=dif/2;
		aRight-=dif/2;
	}
}

void CSoundPlayer::SetMusicVolume(float aValue)
{
	iMusicVolume=aValue;
	if (iMusicVolume<0.0) iMusicVolume=0.0;
	if (iMusicVolume>1.0) iMusicVolume=1.0;

	Mix_VolumeMusic((int)(127*iMusicVolume));
}

void CSoundPlayer::SetFXVolume(float aValue)
{
	iFXVolume=aValue;
	if (iFXVolume<0.0) iFXVolume=0.0;
	if (iFXVolume>1.0) iFXVolume=1.0;
}

void CSoundPlayer::SetReverseStereo(int aBool)
{
	iReverseStereo=aBool;
}

void CSoundPlayer::SetTheme(const char* aTheme)
{
	strcpy(iTheme,aTheme);
}

float CSoundPlayer::FXVolume()
{
	return iFXVolume;
}

float CSoundPlayer::MusicVolume()
{
	return iMusicVolume;
}

int CSoundPlayer::ReverseStereo()
{
	return iReverseStereo;
}

const char* CSoundPlayer::Theme()
{
	return iTheme;
}

void CSoundPlayer::StopMusic()
{
	if (!iInitialized)
		return;

	Mix_FadeOutMusic( KFadeOutTime );
};

char* CSoundPlayer::GetSongFilename( const char* aSoundtrackDir, int aSongNumber )
{	
	DEBUG0("CSoundPlayer::GetSongFilename()\n");
	char buf[ FILENAME_MAX ];

	int a;
	int OK=0;
	int no = iSongCounter;

	if ( aSongNumber != -1 )
		no = aSongNumber;
	else
		iSongCounter++;

	do
	{
		for (a=0;a<KAmountMusicFilenameExtensions&&!OK;a++)
		{
			if ( aSoundtrackDir && strlen( aSoundtrackDir ) > 0 )
				ASSERT(_snprintf(buf,FILENAME_MAX,"%s/%d%s",aSoundtrackDir,no,KMusicFilenameExtensions[a])>0);
			else
				ASSERT(_snprintf(buf,FILENAME_MAX,"music/%s/%d%s",iTheme,no,KMusicFilenameExtensions[a])>0);
			if (exists(buf)) OK=1;
		}

		if (no==0&&!OK)
			return NULL;

		if (no!=0&&!OK)
		{
			no=0;
			if ( aSongNumber == -1 )
				iSongCounter = 0;
		}

	} while (!OK);

	return strdup(buf);
}

void CSoundPlayer::GameMusic( const char* aSoundtrackDir, int aSongNumber )
{	
	DEBUG0("CSoundPlayer::GameMusic()\n");
	if (!iInitialized)
		return;

	StopMusic();

	char *fname=GetSongFilename( aSoundtrackDir, aSongNumber );

	if (fname)
		LoadMusic(fname);
	free(fname);

	PlayMusic(-1);
}

void CSoundPlayer::MenuMusic() 
{	
	DEBUG0("CSoundPlayer::MenuMusic()\n");
	if (!iInitialized)
		return;

	StopMusic();
	LoadMusic("music/menu.ogg");
	PlayMusic(-1);
}

void CSoundPlayer::SetObserver(IPositionalSoundObserver* aObserver)
{
	iObserver = aObserver;
}

/*
int CSoundPlayer::GetPan( int x1, int x2)
{	
	int pan;
 	int diff;
 	// stereo effect
  	diff=  - ( x1 - x2 ); 
  	if (diff!=0){
  	pan = diff  / 2;
  	if ( pan <  - 255 ) pan =  - 255;
   	if ( pan > 255 ) pan = 255;
	if (ReverseStereo()) pan*=-1;
	} else pan=0;
	return(pan);
} */

void CSoundPlayer::SetDynData( CDynGameData* aDynData )
{
	iDynData = aDynData;
}

