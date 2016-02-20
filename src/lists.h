#ifndef __LISTS_H
#define __LISTS_H

#include <vector>

#include "common/files.h"

const int KForbiddenFileAmount=3;
extern const char* KForbiddenFiles[];

class CEpisode 
{
public:
	CEpisode();
	~CEpisode();
	const char* Level(unsigned int aAt);
	const char* LevelName(unsigned int aAt);
	void Reset();
	unsigned int Amount();
	virtual void ListFiles(char *filenames);
	const char* Name();
	const char* DirName();
protected:
	friend class CEpisodeList;
	std::vector<char *> iLevelnames;
	std::vector<char *> iFilenames;
	char *iName;
	char *iDirName;
};

class CDeathMatchEpisode : public CEpisode
{
public:
	void ListFiles(char *filenames);
};

class CEpisodeList
{
public:
	~CEpisodeList();
	CEpisode* Episode( unsigned int aIndex, bool aDMEpisodes );
	void Reset();
	unsigned int Amount( bool aDMEpisodes );
	void ListFiles(char *dirnames);
private:
	void Sort();
	std::vector<CEpisode*> iEpisodes;
	std::vector<CEpisode*> iDMEpisodes;
};

class CMusicThemeList
{
public:
	CMusicThemeList();
	~CMusicThemeList();
	void Reset();
	inline unsigned int Amount() { return iMusicThemes.size(); };
	void LoadThemes();
	char* MusicTheme( unsigned int aIndex );
private:
	std::vector<char *> iMusicThemes;
};

#endif

