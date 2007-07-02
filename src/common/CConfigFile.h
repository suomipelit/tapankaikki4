#ifndef __CCONFIGFILE_H_
#define __CCONFIGFILE_H_

#include <map>
#include <string>
#include "defines.h"

class CConfigFile
{
public:
	EXPORT void Load( const std::string& aDefaultsFile, const std::string& aSecondaryFile, const std::string& aPrimaryFile );
	EXPORT std::string Value( std::string aKey ) const;

	// this queries always string of this format: " aPrefix[aNumber] = value "
	// i.e. brackets are added automatically
	EXPORT std::string Value( const char* aPrefix, int aNumber ) const;

protected:
	void LoadFile( const std::string& aFile );

protected:
	std::map<std::string, std::string> iConfig;
};

#endif

