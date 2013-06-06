#include <stdio.h>

#include "common/defines.h"
#ifndef __unix__
#include <io.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include <ctype.h>
#include <stdlib.h>
#endif

#include <string.h>
#include "lists.h"
#include "common/error.h"
#include "common/CLevel.h"

const char* KForbiddenFiles[]={"..",".","CVS"};

#ifdef __LINUX__
namespace
{
	inline int filter_dir(dirent* dir)
	{
		struct stat info;
		if (stat(dir->d_name, &info) == 0)
			if (S_ISDIR(info.st_mode))
				return true;
		return false;
	}
	inline int filter_norm_lev(dirent* dir)
	{
		struct stat info;
		if (stat(dir->d_name, &info) == 0)
			if (S_ISREG(info.st_mode)) {
				const char* tmp = dir->d_name;
				while(*tmp) {
					if (!strcasecmp(tmp, ".lev"))
						return true;
					tmp++;
				}
			}
		return false;
	}
	inline int filter_level(dirent* file)
	{
		if(strncasecmp(file->d_name, "level", 5) == 0) {
			const char* tmp = file->d_name+5;
			if (!isdigit(*(tmp++)))	return false;
			if (*tmp == 0) return false;
			while(*tmp) {
				if (*tmp == '.')
					return !strcasecmp(++tmp, "lev");
				if (!isdigit(*tmp)) return false;
				tmp++;
			}
		}
		return false;
	}
	inline int sort_level(const void* file1, const void* file2)
	{
		return atoi((*reinterpret_cast<const dirent* const*>(file1))->d_name+5) - atoi((*reinterpret_cast<const dirent* const*>(file2))->d_name+5);
	}
};
#endif

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

void CEpisode::ListFiles(char *filenames)
{
#ifndef __LINUX__
	char name[FILENAME_MAX];
	int a;
	Reset();
	iDirName=strdup( filenames );

	for (a=1;;a++)
	{
		ASSERT(_snprintf(name,FILENAME_MAX,"%s\\level%d.lev",filenames,a)>0);
		if (exists(name))
		{
			char tmp[FILENAME_MAX];
			char* tmpptr;
			ASSERT(_snprintf(tmp,FILENAME_MAX,"level%d.lev",a)>0);
			tmpptr = CLevel::ReadLevelName(name);
			ASSERT(tmpptr);
			if (strlen(tmpptr)>0)
				iLevelnames.push_back(tmpptr);
			else
				iLevelnames.push_back(strdup(tmp));
			iFilenames.push_back(strdup(name));
		}
		else break;
	}
#else
	Reset();
	iDirName=strdup( filenames );

	char* tmpptr;
	char name[FILENAME_MAX];
	dirent** filelist;
	int amount = scandir(filenames, &filelist, filter_level, sort_level);
	ASSERT(amount >= 0);
	for(int i=0; i<amount; i++) {
		ASSERT(snprintf(name, FILENAME_MAX, "%s/%s", filenames, filelist[i]->d_name)>0);
		tmpptr = CLevel::ReadLevelName(name);
		ASSERT(tmpptr);
		if (*tmpptr)
			iLevelnames.push_back(tmpptr);
		else
			iLevelnames.push_back(strdup(filelist[i]->d_name));
		iFilenames.push_back(strdup(name));
		free(filelist[i]);
	}
	free(filelist);
#endif
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

void CEpisodeList::ListFiles(char *dirnames)
{
#ifndef __LINUX__
	struct _finddata_t fbuf;
	int er,error=0;
	int a,ok;
	char search[FILENAME_MAX];
	char tmp[FILENAME_MAX];
	CEpisode* epi;

	ASSERT(_snprintf(search,FILENAME_MAX,"%s/*.*",dirnames)>0);
	Reset();

	// First add DM files :)
	epi=new CDeathMatchEpisode();
	epi->ListFiles(dirnames);

	if (epi->Amount()>0)
	{
		epi->iName=strdup("Deathmatch");
		iDMEpisodes.push_back(epi);
	}
	else
		delete epi;

	// Then add all episodes
	er = _findfirst(search,&fbuf);
	while(er != 0&&error==0)
	{
		if((fbuf.attrib&_A_SUBDIR)== _A_SUBDIR)
		{
			for (a=0,ok=1;ok&&a<KForbiddenFileAmount;a++)
				if (stricmp(fbuf.name,KForbiddenFiles[a])==0)
					ok=0;
			if (ok)
			{
				epi=new CEpisode();
				// Add files
				ASSERT(_snprintf(tmp,FILENAME_MAX,"%s/%s",dirnames,fbuf.name)>0);
				epi->ListFiles(tmp);

				if (epi->Amount()>0)
				{
					// Add name
					epi->iName=strdup(fbuf.name);
					// Add episode into list
					iEpisodes.push_back(epi);
					iDMEpisodes.push_back(epi);
				}
				else
					delete epi;
			}
		}
		error=_findnext(er,&fbuf);
	}
	_findclose(er);

	Sort();

#else	// __LINUX__

	char tmp[FILENAME_MAX];
	dirent** filelist;
	int amount;
	CEpisode* epi;
	struct stat info;
	bool ok;

	// First add DM files :)
	epi=new CDeathMatchEpisode();
	epi->ListFiles(dirnames);

	if (epi->Amount()>0)
	{
		epi->iName=strdup("Deathmatch");
		iDMEpisodes.push_back(epi);
	}
	else
		delete epi;

	// Then add all episodes
	chdir(dirnames);
	amount = scandir(".", &filelist, filter_dir, alphasort);
	chdir("..");
	ASSERT(amount >= 0);

	for(int i=0; i<amount; i++) {
		ok = true;
		for(int a=0; a<KForbiddenFileAmount; a++)
			if (strcasecmp(filelist[i]->d_name, KForbiddenFiles[a]) == 0) {
				ok = false;
				break;
			}
		if (ok) {
			epi = new CEpisode();
			// Add files
			ASSERT(snprintf(tmp, FILENAME_MAX, "%s/%s", dirnames, filelist[i]->d_name)>0);
			epi->ListFiles(tmp);

			if (epi->Amount()>0) {
				// Add name
				epi->iName=strdup(filelist[i]->d_name);
				// Add episode into list
				iEpisodes.push_back(epi);
				iDMEpisodes.push_back(epi);
			}
			else
				delete epi;
		}
		free(filelist[i]);
	}
	free(filelist);

	// calling Sort not necessary since scandir quicksorts it's results
#endif	// __LINUX__
}

void CEpisodeList::Sort()
{
	int a,b;
	CEpisode* tmp;

	for (a=1;a<iEpisodes.size()-1;a++)
		for (b=a;b<iEpisodes.size();b++)
			if (stricmp(iEpisodes[a]->Name(),iEpisodes[b]->Name())>0) // a "bigger" than b
			{
				tmp=iEpisodes[a];
				iEpisodes[a]=iEpisodes[b];
				iEpisodes[b]=tmp;
			}
}

void CDeathMatchEpisode::ListFiles(char *filenames)
{
#ifndef __LINUX__
	char name[FILENAME_MAX],levname[FILENAME_MAX];
	bool ok;
	Reset();

	ASSERT(_snprintf(name,FILENAME_MAX,"%s\\*.lev",filenames)>0);
	iDirName = strdup( filenames );

	struct _finddata_t fbuf;
	int er,error=0;
	er = _findfirst(name,&fbuf);
	while(er != 0&&error==0)
	{
		if (((fbuf.attrib&&_A_NORMAL)== _A_NORMAL)&&((fbuf.attrib&_A_SUBDIR)!= _A_SUBDIR))
		{
			ok=true;
			for (int a=0;ok&&a<KForbiddenFileAmount;a++)
				if (stricmp(fbuf.name,KForbiddenFiles[a])==0)
					ok=false;

			if (ok)
			{
				char *tmp;
				ASSERT(_snprintf(levname,FILENAME_MAX,"%s\\%s",filenames,fbuf.name)>0);

				tmp = CLevel::ReadLevelName(levname);
				ASSERT(tmp);
				if (strlen(tmp)>0)
					iLevelnames.push_back(tmp);
				else
					iLevelnames.push_back(strdup(fbuf.name));

				iFilenames.push_back(strdup(levname));
			}
		}
		error=_findnext(er,&fbuf);
	}
	_findclose(er);
#else
	Reset();

	char levname[FILENAME_MAX];
	bool ok;
	iDirName = strdup( filenames );

	dirent** filelist;
	chdir(filenames);
	int amount = scandir(".", &filelist, filter_norm_lev, alphasort);
	chdir("..");
	ASSERT(amount >= 0);

	for(int i=0; i<amount; i++) {
		ok = true;
		for(int a=0; a<KForbiddenFileAmount; a++)
			if (strcasecmp(filelist[i]->d_name, KForbiddenFiles[a]) == 0) {
				ok = false;
				break;
			}

		if (ok) {
			ASSERT(snprintf(levname, FILENAME_MAX, "%s/%s", filenames, filelist[i]->d_name) > 0);
			char* tmp = CLevel::ReadLevelName(levname);
			ASSERT(tmp);
			if (*tmp)
				iLevelnames.push_back(tmp);
			else
				iLevelnames.push_back(strdup(filelist[i]->d_name));

			iFilenames.push_back(strdup(levname));
		}
		free(filelist[i]);
	}
	free(filelist);
#endif
}
