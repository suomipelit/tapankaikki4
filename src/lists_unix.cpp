#include <stdio.h>

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include <ctype.h>
#include <stdlib.h>

#include <string.h>
#include "lists.h"
#include "common/error.h"
#include "common/CLevel.h"

namespace
{
	inline int filter_dir(const dirent* dir)
	{
		struct stat info;
		if (stat(dir->d_name, &info) == 0)
			if (S_ISDIR(info.st_mode))
				return true;
		return false;
	}
	inline int filter_norm_lev(const dirent* dir)
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
	inline int filter_level(const dirent* file)
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

void CEpisode::ListFiles(char *filenames)
{
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
}

void CEpisodeList::ListFiles(char *dirnames)
{
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
}

void CEpisodeList::Sort()
{
	int a,b;
	CEpisode* tmp;

	for (a=1;a<iEpisodes.size()-1;a++)
		for (b=a;b<iEpisodes.size();b++)
			if (strcasecmp(iEpisodes[a]->Name(),iEpisodes[b]->Name())>0) // a "bigger" than b
			{
				tmp=iEpisodes[a];
				iEpisodes[a]=iEpisodes[b];
				iEpisodes[b]=tmp;
			}
}

void CDeathMatchEpisode::ListFiles(char *filenames)
{
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
}

void CMusicThemeList::LoadThemes()
{
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
}
