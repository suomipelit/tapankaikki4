#include "CIngameData.h"
#include "common/CGraphicsGenerator.h"
#include "common/CConfigFile.h"
#include "common/files.h"
#include "timer.h"
#include "CCharacter.h"

namespace
{
	enum TCalcMethod
	{
		ENone=0,
		EDarken,
		EZoom1,
		EZoom2,
		EZoom3,
		EZoom4,
		EZoom5,
	};
	enum TAfterProcess
	{
		ENoProcess=0,
		ERotate40,
		ERotate80
	};

	struct TGameSpriteData
	{
		const char *iKeyName; // filename
		const int src_gbuf;	// if this buf has to be calculted, this is source picture. -1 if not
		const enum TCalcMethod calc_method;
		const enum TAfterProcess after_process;
	};

	const char* KIngamePaletteKey = "GameSpriteWalls";
	const char* KKillTextAmount = "FragTextAmount";
	const char* KKillTextPrefix = "FragText";
	const char* KSuicideTextAmount = "SuicideTextAmount";
	const char* KSuicideTextPrefix = "SuicideText";
	const char* KForceSoundtrack = "ForceSoundTrack";
	const char* KForceSongNumber = "ForceSongNumber";

	const TGameSpriteData KGameSpriteFiles[EGameSpriteAmount]=
	{
		{"GameSpriteWallClr",-1,ENone,ENoProcess},
		{"GameSpriteCrate",-1,ENone,ENoProcess},
		{"GameSpriteShadow",-1,ENone,ENoProcess},
		{"GameSpriteTarget",-1,ENone,ENoProcess},
		{"GameSpriteFloor",-1,ENone,ENoProcess},
		{"GameSpriteFloor2",EGameSpriteFloor,EDarken,ENoProcess},
		{"GameSpriteBodyPart",-1,ENone,ENoProcess},
		{"GameSpriteEffect",-1,ENone,ENoProcess},
		{"GameSpriteWall2",EGameSpriteWalls,EDarken,ENoProcess},
		{"GameSpriteWalls",-1,ENone,ENoProcess},
		{"GameSpriteWalls_1",EGameSpriteWalls,EZoom1,ENoProcess},
		{"GameSpriteWalls_2",EGameSpriteWalls,EZoom2,ENoProcess},
		{"GameSpriteWalls_3",EGameSpriteWalls,EZoom3,ENoProcess},
		{"GameSpriteWalls_4",EGameSpriteWalls,EZoom4,ENoProcess},
		{"GameSpriteWalls_5",EGameSpriteWalls,EZoom5,ENoProcess},
		{"GameSpriteRambo1",-1,ENone,ERotate80},
		{"GameSpriteRambo2",-1,ENone,ERotate80},
		{"GameSpriteRambo3",-1,ENone,ERotate80},
		{"GameSpriteRambo4",-1,ENone,ERotate80},
		{"GameSpriteEnemy0",-1,ENone,ERotate40},
		{"GameSpriteEnemy1",-1,ENone,ERotate40},
		{"GameSpriteEnemy2",-1,ENone,ERotate40},
		{"GameSpriteEnemy3",-1,ENone,ERotate40},
		{"GameSpriteEnemy4",-1,ENone,ERotate40},
		{"GameSpriteEnemy5",-1,ENone,ERotate40},
		{"GameSpriteEnemy6",-1,ENone,ERotate40},
		{"GameSpriteEnemy7",-1,ENone,ERotate40},
		{"GameSpriteEnemy8",-1,ENone,ERotate40},
		{"GameSpriteEnemy9",-1,ENone,ERotate40},
		{"GameSpriteBulletC4",-1,ENone,ENoProcess},
		{"GameSpriteBulletGrenade",-1,ENone,ENoProcess},
		{"GameSpriteBulletMine",-1,ENone,ENoProcess},
	};

	const char *KEffectFiles[ESoundEffectAmount] = 
	{
		"GameSoundEffectPistol",
		"GameSoundEffectShotgun",
		"GameSoundEffectUzi",
		"GameSoundEffectAutorifle",
		"GameSoundEffectBazooka",
		"GameSoundEffectHit",
		"GameSoundEffectSmallExplosion",
		"GameSoundEffectExplosion",
		"GameSoundEffectBigExplosion",
		"GameSoundEffectClick",
		"GameSoundEffectDeath",
		"GameSoundEffectWoosh",
		"GameSoundEffectFlame",
		"GameSoundEffectMetalhit",
		"GameSoundEffectGrenadeLauncher",
		"GameSoundEffectKickHead"
	};
}

CIngameData::CIngameData( CDynGameData* aDynData ) : iIngameFilesLoaded( false )
{
	for (int a=0;a<ESoundEffectAmount;a++)
		iSoundEffects[a] = NULL;
}

CIngameData::~CIngameData()
{

}

void CIngameData::LoadIngameData(const std::string& aEpisodeConfig, const std::string& aLevelConfig)
{
	CConfigFile tmp;
	int a;

	LOG0("Loading ingame config\n");

	tmp.Load("default.ini", aEpisodeConfig, aLevelConfig );

	LOG0("Loading ingame graphics");
	// Load sprites & palettes
	for (a=0;a<EGameSpriteAmount;a++)
	{
		LOG0(".");
		if (KGameSpriteFiles[a].src_gbuf!=-1) continue;
		if (strcmp(KGameSpriteFiles[a].iKeyName,KIngamePaletteKey)!=0)
		{
			iGameSprite[a].Load( tmp.Value( KGameSpriteFiles[a].iKeyName ).c_str(), NULL);
		}
		else 
		{
			iGameSprite[a].Load(tmp.Value( KGameSpriteFiles[a].iKeyName ).c_str(), &iPalette);

#ifdef _DEBUG
			bool n = false;
			for (int x=0;x<256&&!n;x++)
				if (iPalette.Color(x).r!=0||
					iPalette.Color(x).g!=0||
					iPalette.Color(x).b!=0)
					n = true;

			DEBUG_ASSERT( n );
#endif 
		}
	}

	DEBUG1("\nStarting generation at %d\n",TimerCounter);
	// Load generated files or generate them if older than source or source doesn't exist.
	for (a=0;a<EGameSpriteAmount;a++)
	{
		LOG0(".");
		if (KGameSpriteFiles[a].src_gbuf==-1) continue;

		if (filetimestamp(tmp.Value(KGameSpriteFiles[KGameSpriteFiles[a].src_gbuf].iKeyName).c_str())>
			filetimestamp(tmp.Value(KGameSpriteFiles[a].iKeyName).c_str()))
		{
			if (KGameSpriteFiles[a].calc_method == EDarken)
				CGraphicsGenerator::MakeFloors(iPalette,iGameSprite[KGameSpriteFiles[a].src_gbuf],iGameSprite[a]);
			else
				CGraphicsGenerator::ZoomWall( iGameSprite[KGameSpriteFiles[a].src_gbuf],iGameSprite[a],(int)(KGameSpriteFiles[a].calc_method - EZoom1) );
			iGameSprite[a].Save(tmp.Value(KGameSpriteFiles[a].iKeyName).c_str(),&iPalette);
		}
		else 
			iGameSprite[a].Load(tmp.Value(KGameSpriteFiles[a].iKeyName).c_str(),NULL);
	}
	DEBUG1("\nfinished generation at %d\n",TimerCounter);

	LOG0("\nProcessing...");

	for (a=0;a<EGameSpriteAmount;a++)
	{
		if ( KGameSpriteFiles[a].after_process == ERotate40 )
		{
			CGraphicsGenerator::RotateSprites( iGameSprite[a], KCharacterSpriteSize, KCharacterSpriteSize, 40, iPalette );
		}

		else 
		if ( KGameSpriteFiles[a].after_process == ERotate80 )
		{
			CGraphicsGenerator::RotateSprites( iGameSprite[a], KCharacterSpriteSize, KCharacterSpriteSize, 80, iPalette);
		}
	}

	LOG0("\nLoading ingame sound effects");
	for (int a=0;a<ESoundEffectAmount;a++)
	{
		LOG0(".");
		iSoundEffects[a]=Mix_LoadWAV( tmp.Value( KEffectFiles[a] ).c_str());
		ASSERT(iSoundEffects[a]);
	}
	LOG0 ( KNewLine );

	int tmp2 = atoi( tmp.Value( KKillTextAmount ).c_str() );
	for (int a=0;a<tmp2;a++)
	{
		std::string tmpstr =  tmp.Value( KKillTextPrefix, a );
		if (tmpstr.length() <= 0)
			throw CFailureException("CIngameData loading failed! Kill text was empty.");
		iKillTexts.push_back( tmpstr );
	}

	tmp2 = atoi( tmp.Value( KSuicideTextAmount ).c_str() );
	for (int a=0;a<tmp2;a++)
	{
		std::string tmpstr =  tmp.Value( KSuicideTextPrefix, a );
		if (tmpstr.length() <= 0)
			throw CFailureException("CIngameData loading failed! Suicide text was empty.");
		iSuicideTexts.push_back( tmpstr );
	}

	if (tmp.Value( KForceSongNumber ).length() == 0)
		iSongNumber = -1;
	else
        iSongNumber	= atoi( tmp.Value( KForceSongNumber ).c_str() );

	iSoundtrack = tmp.Value( KForceSoundtrack );

	iIngameFilesLoaded = true;
}

void CIngameData::UnloadIngameData()
{
	int a;

	for (a=0;a<EGameSpriteAmount;a++)
		iGameSprite[a].Reset();
	for (a=0;a<ESoundEffectAmount;a++)
		if (iSoundEffects[a])
		{
			Mix_FreeChunk(iSoundEffects[a]);
			iSoundEffects[a] = NULL;
		}

	iKillTexts.clear();
	iSuicideTexts.clear();

	iIngameFilesLoaded = false;
}

const char* CIngameData::RandomKillText() const
{
	return iKillTexts[ rand() % iKillTexts.size() ].c_str();
}

const char* CIngameData::RandomSuicideText() const
{
	return iSuicideTexts[ rand() % iSuicideTexts.size() ].c_str();
}

std::string CIngameData::Soundtrack() const
{
	return iSoundtrack;
}

int CIngameData::SongNumber() const
{
	return iSongNumber;
}


