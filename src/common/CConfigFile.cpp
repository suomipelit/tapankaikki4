#include "CConfigFile.h"
#include "error.h"

namespace
{
	const int TMPSIZE = 256;
}

void CConfigFile::Load( const std::string& aDefaultsFile, const std::string& aSecondaryFile, const std::string& aPrimaryFile )
{
	iConfig.clear();
	LOG3("CConfigFile: Loading configs: %s, %s and %s\n", aPrimaryFile.c_str(), aSecondaryFile.c_str(), aDefaultsFile.c_str());
	LoadFile( aPrimaryFile );
	LoadFile( aSecondaryFile );
	LoadFile( aDefaultsFile );

	std::map< std::string, std::string>::iterator iter = iConfig.begin();

	while (iter != iConfig.end() )
	{
		LOG2( "%s = %s\n", iter->first.c_str(), iter->second.c_str() );
		iter++;
	}
}

void CConfigFile::LoadFile( const std::string& aFile )
{
	FILE *fp;

	fp = fopen( aFile.c_str(), "r" );
	if (!fp)
		return;

	while (!feof( fp ))
	{
		char tmp[TMPSIZE];
		int idx=0;

		fgets( tmp, TMPSIZE, fp );

		while ( isspace(tmp[idx]) )
			idx++;

		if ( tmp[idx]!='#' && tmp[idx]!=';' )
		{ // not a comment
			int eqIdx = idx;


			while (tmp[eqIdx]!='=' &&
				   tmp[eqIdx]!=0)
			   eqIdx++;

			if (tmp[eqIdx]=='=' && eqIdx!=idx)
			{
				// key value pair expected here:
				tmp[eqIdx] = 0;
				eqIdx++;

				while ( isspace(tmp[eqIdx]) ) eqIdx++;
				while ( isspace(tmp[idx]) ) idx++;

				while ( (tmp+eqIdx)[strlen(tmp+eqIdx)-1]=='\n'||
					    isspace((tmp+eqIdx)[strlen(tmp+eqIdx)-1]))
					(tmp+eqIdx)[strlen(tmp+eqIdx)-1] = 0;
				while ( isspace((tmp+idx)[strlen(tmp+idx)-1]) )
					(tmp+idx)[strlen(tmp+idx)-1] = 0;


				if (strlen(tmp+idx)>0 &&
					strlen(tmp+eqIdx)>0 )
					iConfig.insert( std::pair<std::string, std::string>( tmp+idx, tmp+eqIdx ) );
			}
		}
	}

	fclose(fp);
}

std::string CConfigFile::Value( std::string aKey ) const
{
	std::map< std::string, std::string>::const_iterator iter = iConfig.find( aKey );

	if (iter != iConfig.end())
	{
		ASSERT( iter->second.length() > 0 );
		return iter->second;
	}
	else
	{
		LOG1("No config value for %s\n",aKey.c_str() );
		return std::string();
	}
}

EXPORT std::string CConfigFile::Value( const char* aPrefix, int aNumber ) const
{
	char tmp[10];
	std::string str( aPrefix );

	str+='[';
	sprintf(tmp, "%d", aNumber);
	str+=tmp;
	str+=']';

	return Value( str );
}

