#include <stdio.h>

#include <io.h>

#include <string.h>
#include "lists.h"
#include "common/error.h"
#include "common/CLevel.h"

void CEpisode::ListFiles(char *filenames)
{
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
}

void CEpisodeList::ListFiles(char *dirnames)
{
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
}
