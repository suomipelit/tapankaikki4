#include <stdio.h>

#include <string.h>
#include "lists.h"
#include "common/error.h"
#include "common/CLevel.h"

const char* KForbiddenFiles[]={"..",".","CVS"};

namespace
{
	const char* KNoLevels="*No levels*";
};

unsigned int CEpisode::Amount()
{
	return (unsigned int)iFilenames.size();
}

CEpisode::~CEpisode()
{
	Reset();
}

CEpisode::CEpisode()
{
	iName=0;
	iDirName=0;
}

void CEpisode::Reset()
{
	unsigned int a;

	for (a=0;a<iFilenames.size();a++)
	{
		free(iFilenames.at(a));
	}
	iFilenames.clear();
	free(iName);
	iName=NULL;
	free(iDirName);
	iDirName=NULL;
}

const char* CEpisode::Name()
{
	return iName;
}

const char* CEpisode::DirName()
{
	return iDirName;
}

const char* CEpisode::Level(unsigned int aAt)
{
	ASSERT(aAt<iFilenames.size());
	return iFilenames.at(aAt);
}

const char* CEpisode::LevelName(unsigned int aAt)
{
	if  (aAt >= iLevelnames.size() )
	{
		return KNoLevels;
	}
	return iLevelnames.at(aAt);
}

CEpisodeList::~CEpisodeList()
{
	Reset();
}

CEpisode* CEpisodeList::Episode( unsigned int aIndex, bool aDMEpisodes )
{
	if ( !aDMEpisodes )
	{
		ASSERT( aIndex<iEpisodes.size() );
		return iEpisodes.at( aIndex );
	}
	else
	{
		ASSERT( aIndex<iDMEpisodes.size() );
		return iDMEpisodes.at( aIndex );
	}
}

void CEpisodeList::Reset()
{
	unsigned int a;

	for (a=0;a<iDMEpisodes.size();a++)
	{
		delete (iDMEpisodes.at(a));
	}
	iDMEpisodes.clear();
	iEpisodes.clear(); // this was only a subset of iDMEpisodes
}

unsigned int CEpisodeList::Amount( bool aDMEpisodes )
{
	if ( !aDMEpisodes )
		return (unsigned int)iEpisodes.size();
	else
		return (unsigned int)iDMEpisodes.size();
}

CMusicThemeList::CMusicThemeList()
{
}

CMusicThemeList::~CMusicThemeList()
{
	Reset();
}

char* CMusicThemeList::MusicTheme( unsigned int aIndex )
{
	ASSERT( aIndex<iMusicThemes.size() );
	return iMusicThemes.at( aIndex );
}

void CMusicThemeList::Reset()
{
	unsigned int a;

	for (a=0;a<iMusicThemes.size();a++)
	{
		free (iMusicThemes.at(a));
	}
	iMusicThemes.clear();
}

