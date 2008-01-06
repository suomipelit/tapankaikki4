#ifndef __COPTIONS_H
#define __COPTIONS_H

#include <vector>
#include "CKeys.h"
#include "common/files.h"
#include "lists.h"
#include "CPlayer.h"

class CGameGraphicsInterface;
class CSoundPlayer;

#define DEFAULT_MUSICTHEME "Default"

const float KMusicVolumeMin=0.0f;
const float KMusicVolumeMax=1.0f;
const float KMusicVolumeStep=0.05f;

const float KEffectVolumeMin=0.0f;
const float KEffectVolumeMax=1.0f;
const float KEffectVolumeStep=0.05f;

const float KMouseSensitivityMin=0.0f;
const float KMouseSensitivityMax=10.0f;
const float KMouseSensitivityStep=0.05f;

const float KMinGamma = 0.1f;
const float KMaxGamma = 3.0f;
const float KGammaStep = 0.1f;

const int DNSNAME_LENGTH=255;
const int PROXYPORT_LENGTH=6;
const int KPlayerNameMaxLength=15;

class COptionsData
{
public:
	COptionsData();
	void Reset();

	CKeys iKeys[KMaxLocalPlayers];
	char iName[KMaxLocalPlayers][KPlayerNameMaxLength];

	float iGamma;
	float iMusicVolume;
	float iEffectsVolume;

	bool iEnemiesOnGame;
	int iMultiPlayLevel;
	int iMultiPlayGameMode;
	int iMultiPlayers;
	int iMultiPlayEpisode;
	TNetworkMode iMultiPlayNetworkMode;
	float iDMSpeed;
	bool iDMTargetSystem;
	
	bool iStereo;
	bool iFullScreen;
	float iMouseSensitivity;
	int iMouseMode;
	CCoord<int> iInGameVideoMode;
	char iMusicTheme[FILENAME_MAX];
	char iProxyPort[PROXYPORT_LENGTH];
	bool iProxyEnabled;
	bool iWebVersionCheckingEnabled;
	char iProxyAddr[DNSNAME_LENGTH];
};

class COptions
{

public:
	COptions(CSoundPlayer* aSP);
	~COptions();
	void Save();
	void Load();
	void Reset();
	void UpdateAudioChanges();
	void UpdateGammaChanges();
	inline CEpisodeList* EpisodeList(){return iEpisodeList;};
	COptionsData& Data();
	inline int MusicThemes(){ return (int)iMusicThemes.size();}
	inline char* MusicTheme(int a){ if (a<0 || a>=(signed)iMusicThemes.size()) return NULL;	return iMusicThemes.at(a);}
	char* MouseModeText();

private:
	void LoadThemes();

	CEpisodeList* iEpisodeList;
	CSoundPlayer* iSP;

	COptionsData iData;
	std::vector<char *> iMusicThemes;
};



#endif
