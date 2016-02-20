#ifndef __CGAMEVERSION_H
#define __CGAMEVERSION_H

class CGameVersion
{
public:
	static const char* Version();
	// Return 1 if aVersion bigger
	//        0 if equal
	//        -1 if smaller
	static int Compare(const char* aVersion);
	static int ExtractMinor(const char* aVersion);
	static int ExtractMajor(const char* aVersion);

};

#endif

