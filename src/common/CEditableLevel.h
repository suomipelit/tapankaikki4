#ifndef __CEDITABLELEVEL_H
#define __CEDITABLELEVEL_H

#include "CLevel.h"

const int KMaxLevelWidth=320;
const int KMaxLevelHeight=200;
const int KMinLevelWidth=16;
const int KMinLevelHeight=12;

class CEditableLevel : public CLevel
{
public:
	void ModifyBorderBlocks();
	EXPORT int  Save(const char* aName);
	EXPORT void AddStartPoint( CCoord<int>& aStartLocation );
	EXPORT void ModifyStartPoint(int aIndex, CCoord<int>& aStartLocation);
    EXPORT void New(int aXsize,int aYsize,TBlockType aType,int aNum,int aHeight);
	EXPORT void DeleteSteam(CSteam* aPtr);
	EXPORT void DeleteSpotLight(CSpotLight* aPtr);
	EXPORT void DeleteEnemy(int aIndex);
	EXPORT void DeleteNormalCrate(int aIndex);
	EXPORT void DeleteDMCrate(int aIndex);
	EXPORT void ModifyRandomCrateAmount(int aType1,int aType2, int aAdd, bool aDM );

	EXPORT CCoord<int>& StartPosition(int aIndex);

	// some utilities
	EXPORT void WriteInt( int aValue, FILE* aFP );
	EXPORT void WriteChunkHeader( TChunk aBlockID, int aLength, FILE* aFP );

protected:
	EXPORT void MakeBorderWalls();
	EXPORT static void TrimRandomCrates( std::vector< TRandomCrateInfo >& iCrates );
};

#endif


