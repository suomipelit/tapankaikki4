#include "CGameVersion.h"
#include "stdlib.h"
#include "string.h"
#include "common/error.h"

const char *CGameVersion::Version(void)
{
	return "4.09";
}

int CGameVersion::Compare(const char* aVersion)
{
	if (ExtractMajor(Version())>ExtractMajor(aVersion)) return -1;
	if (ExtractMajor(Version())<ExtractMajor(aVersion)) return 1;
	if (ExtractMinor(Version())>ExtractMinor(aVersion)) return -1;
	if (ExtractMinor(Version())<ExtractMinor(aVersion)) return 1;
	return 0;
}

int CGameVersion::ExtractMajor(const char* aVersion)
{
	char *comma;
	char *dup;
	int a;
	ASSERT(aVersion);
	ASSERT(strlen(aVersion)>3);
	ASSERT(strlen(aVersion)<256);

	dup = strdup(aVersion);
	comma=strchr(dup,'.');
    comma[0]=0;
	a=atoi(dup);
	free( dup );
	return a;
}

int CGameVersion::ExtractMinor(const char* aVersion)
{
	char *comma;
	ASSERT(aVersion);
	ASSERT(strlen(aVersion)>3);
	ASSERT(strlen(aVersion)<256);

	comma=strchr(aVersion,'.');
	return atoi(comma+1); // return integer after '.'
}

