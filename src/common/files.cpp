#include "files.h"

#include <stdio.h>
#ifdef __MINGW32__
#include <sys/stat.h>
#endif
#ifdef __unix__
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#endif

const std::string SaveDataPath=getsavebasedir();
EXPORT std::string getsavebasedir()
{
#ifdef __unix__
	const char *home = getenv("HOME");

	if (home == NULL)
		exit(1);

	std::string path = std::string(home) + std::string("/.tapankaikki/");
	if (mkdir(path.c_str(), 0755) != 0 && errno != EEXIST)
		exit(1);

	return path;
#else
	return "";
#endif
}

EXPORT std::string getsavepath(const std::string& name)
{
	std::string ret = SaveDataPath + name;
	return ret;
}

EXPORT std::string getdatapath(const std::string& name)
{
#ifdef __unix__
	std::string ret = std::string(DATADIR) + std::string("/") + name;
#else
	std::string ret = name;
#endif
	return ret;
}

EXPORT std::string getpath(const char *name)
{
	int len = strlen(name);
	int a=0;
	std::string ret;
	for (a=len-1;a>0 && name[a]!='/' && name[a]!='\\';a--) {};
	if (a>0)
	for (int b=0;b<=a;b++)
		ret+=name[b];
	return (ret);
}

EXPORT int exists(const char *name)
{
    FILE *f;
    f=fopen(name,"rb");
    if (f==NULL) return(0); 
    fclose(f);
    return(1);
}

EXPORT int filelen(const char *name)
{
    FILE *f;
    int len;  
    f=fopen(name,"rb");
    if (f==NULL) return(-1);
    fseek(f,0,SEEK_END);
    len=ftell(f);
    fclose(f);
    return(len);
}

EXPORT int filetimestamp(const char *name)
{
	struct EF_STAT fileinfo;

	if (EF_STAT(name,&fileinfo)==-1) return -1;

	return fileinfo.st_mtime;
}
