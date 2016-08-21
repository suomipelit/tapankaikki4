#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <SDL.h> 

#include "COptions.h"

#include "common/CGraphicsDevice.h"

#include "CSoundPlayer.h"

namespace
{
	// TODO: CONFIGFILE
	const char* KDefaultName[]={"John","Paul","George","Ringo"};
};

COptionsData::COptionsData()
{
	Reset();
}

void COptionsData::Reset()
{
	iGamma = 1.0;
	iMusicVolume = 1.0;
	iEffectsVolume = 1.0;
	iEnemiesOnGame = true;
	iDMSpeed = KPlayerRunSpeed;
	iStereo = false;
	iFullScreen = true;
	iMouseSensitivity = 0.50f; // in percents
	iMouseMode = 1;
	iInGameVideoMode = CCoord<int>(640,480);
	iProxyEnabled = false;
	iWebVersionCheckingEnabled = false;
	iMultiPlayGameMode = 0;
	iMultiPlayEpisode = 1;
	iMultiPlayNetworkMode = ENetworkModeNone;
	strcpy(iMusicTheme,DEFAULT_MUSICTHEME);
	iProxyAddr[0]=0;
	iProxyPort[0]=0;
	iDMTargetSystem = false;

	for (unsigned int a=0;a<KMaxLocalPlayers;a++)
		strcpy( iName[a], KDefaultName[a] ); 

	iKeys[ 0 ].keys[ EKeyLEFT ] = 0 ;
	iKeys[ 0 ].keys[ EKeyRIGHT ] = 0 ;
	iKeys[ 0 ].keys[ EKeyUP ] = SDLK_w ;
	iKeys[ 0 ].keys[ EKeyDOWN ] = SDLK_s ;
	iKeys[ 0 ].keys[ EKeySHOOT ] = SDLK_SPACE ;
	iKeys[ 0 ].keys[ EKeySHIFT ] = 0 ;
	iKeys[ 0 ].keys[ EKeySTRAFE ] = 0 ;
	iKeys[ 0 ].keys[ EKeyLSTRAFE ] = SDLK_a ;
	iKeys[ 0 ].keys[ EKeyRSTRAFE ] = SDLK_d ;

	iKeys[ 1 ].keys[ EKeyLEFT ] = SDLK_j ;
	iKeys[ 1 ].keys[ EKeyRIGHT ] = SDLK_l ;
	iKeys[ 1 ].keys[ EKeyUP ] = SDLK_i ;
	iKeys[ 1 ].keys[ EKeyDOWN ] = SDLK_k ;
	iKeys[ 1 ].keys[ EKeySHOOT ] = SDLK_y ;
	iKeys[ 1 ].keys[ EKeySHIFT ] = SDLK_t ;
	iKeys[ 1 ].keys[ EKeySTRAFE ] = SDLK_h ;
	iKeys[ 1 ].keys[ EKeyLSTRAFE ] = SDLK_u ;
	iKeys[ 1 ].keys[ EKeyRSTRAFE ] = SDLK_o ;
}


COptions::COptions(CSoundPlayer* aSP): iSP(aSP)
{
	ASSERT(aSP);

	iEpisodeList=new CEpisodeList;
	iEpisodeList->ListFiles("levs");

	iMusicThemeList=new CMusicThemeList;

    Load();

	// some sanity checks
	if ( iEpisodeList->Amount( iData.iMultiPlayGameMode ) <= iData.iMultiPlayEpisode )
	{
		printf("iEpisodeList->Amount( iData.iMultiPlayGameMode ) <= iData.iMultiPlayEpisode   true\n");
		iData.iMultiPlayEpisode = 0;
		iData.iMultiPlayLevel = 0;
	}

	if ( iEpisodeList->Episode( iData.iMultiPlayEpisode, iData.iMultiPlayGameMode )->Amount() <= iData.iMultiPlayLevel )
	{
		printf("iEpisodeList->Episode( iData.iMultiPlayEpisode, iData.iMultiPlayGameMode )->Amount() <= iData.iMultiPlayLevel   true\n");
		iData.iMultiPlayLevel = 0;
	}
}

COptions::~COptions()
{
	unsigned int a;

	Save();
	delete iEpisodeList;
	delete iMusicThemeList;
}

COptionsData& COptions::Data()
{
	return iData;
}

void COptions::Save()
{
	FILE *cfg;

	for (int a=0;a<KMaxLocalPlayers;a++)
	{
		int len = strlen(iData.iName[ a ]);
		while ( len > 0 && iData.iName[ a ][ len - 1 ] == ' ' )
		{
			iData.iName[ a ][ len - 1 ] = 0; // trim from end
			len = strlen(iData.iName[ a ]);
		}
	};
	cfg = fopen(getsavepath(KOptionsFilename).c_str(), "wb");
	fwrite(&iData,sizeof(COptionsData), 1,cfg);
	fclose(cfg);
}

void COptions::Load() 
{
	FILE *cfg;

	if (filelen(getsavepath(KOptionsFilename).c_str())==sizeof(COptionsData))
	{
		cfg = fopen(getsavepath(KOptionsFilename).c_str(), "rb");
		if (cfg) 
		{
			fread(&iData,sizeof(COptionsData), 1,cfg);
			fclose(cfg);
		}
	}

	iMusicThemeList->LoadThemes();
	UpdateAudioChanges();
}

void COptions::UpdateGammaChanges(CGraphicsDevice& aGD)
{
	if (iData.iGamma < KMinGamma)
		iData.iGamma = KMinGamma;
	if (iData.iGamma > KMaxGamma)
		iData.iGamma = KMaxGamma;

	if (SDL_SetWindowBrightness(aGD.GetSDLwindow(), iData.iGamma) < 0)
		iData.iGamma = 1;
}

void COptions::UpdateAudioChanges()
{
	if (iSP->FXVolume()!=iData.iEffectsVolume)
		iSP->SetFXVolume(iData.iEffectsVolume);
	if (iSP->MusicVolume()!=iData.iMusicVolume)
		iSP->SetMusicVolume(iData.iMusicVolume);
	if (strcmp(iSP->Theme(),iData.iMusicTheme))
		iSP->SetTheme(iData.iMusicTheme);
}

const char* COptions::MouseModeText()
{
	switch (iData.iMouseMode)
	{
		case 0: return "None";
		case 1: return "Player 1";
		case 2: return "Player 2";
		case 3: return "Player 3";
		case 4: return "Player 4";
		default: error("COptions::MouseModeText: Invalid mousemode!");
	}

	return NULL;
}

void COptions::Reset()
{
	iData.Reset();
}
