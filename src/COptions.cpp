#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <SDL.h>

#include "common/defines.h"

#ifndef __unix__
#include <io.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#endif


#include "COptions.h"

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
	iWebVersionCheckingEnabled = true;
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

	for(a=0;a<iMusicThemes.size();a++)
	{
		free (iMusicThemes.at(a));
	}
	iMusicThemes.clear();
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
	cfg = fopen(KOptionsFilename, "wb");
	fwrite(&iData,sizeof(COptionsData), 1,cfg);
	fclose(cfg);
}

void COptions::Load() 
{
	FILE *cfg;

	if (filelen(KOptionsFilename)==sizeof(COptionsData))
	{
		cfg = fopen(KOptionsFilename, "rb");
		if (cfg) 
		{
			fread(&iData,sizeof(COptionsData), 1,cfg);
			fclose(cfg);
		}
	}

	LoadThemes();
	UpdateAudioChanges();
}

void COptions::LoadThemes()
{
#ifndef __LINUX__
	struct _finddata_t fbuf;

	int er,error=0;
	bool ok;

	er = _findfirst("music/*",&fbuf);
	while(er != 0&&error==0)
	{
		if ((fbuf.attrib&_A_SUBDIR)== _A_SUBDIR)
		{
			ok=true;
			for (int a=0;ok&&a<KForbiddenFileAmount;a++)
				if (stricmp(fbuf.name,KForbiddenFiles[a])==0)
					ok=false;

			if (ok)
			{
				char* level=strdup(fbuf.name);
				iMusicThemes.push_back(level);
			}
		}
		error=_findnext(er,&fbuf);
	}
	_findclose(er);

#else	// __LINUX__

	if (chdir("music") == 0) {
		DIR* fbuf = opendir(".");
		dirent* cur;
		struct stat info;
		bool ok;
		while ((cur = readdir(fbuf))) {
			if (stat(cur->d_name, &info) == 0) {
				if (S_ISDIR(info.st_mode)) {
					ok = true;
					for(int a=0; a<KForbiddenFileAmount; a++)
						if (strcasecmp(cur->d_name, KForbiddenFiles[a]) == 0) {
							ok = false;
							break;
						}
					if (ok) {
						char* level = strdup(cur->d_name);
						iMusicThemes.push_back(level);
					}
				}
			}
		}
		closedir(fbuf);

		chdir("..");
	}
#endif	// __LINUX__
}

void COptions::UpdateGammaChanges()
{
	if (iData.iGamma < KMinGamma)
		iData.iGamma = KMinGamma;
	if (iData.iGamma > KMaxGamma)
		iData.iGamma = KMaxGamma;

	int err = SDL_SetGamma( iData.iGamma, iData.iGamma, iData.iGamma );

	if ( err == -1 )
	{
		iData.iGamma = 1;
		LOG0("Gamma is not supported by the hardware\n");
	}
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

char* COptions::MouseModeText()
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
