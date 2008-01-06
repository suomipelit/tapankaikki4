#ifndef __CLEVEL_H
#define __CLEVEL_H

#include <vector>
#include "CGameTypeData.h"
#include "CCoord.h"

class CFilename;
class CSpotLight;
class CSteam;

enum TBlockType
{
	EBlockTypeFloor=0,EBlockTypeWall
};

const int KBlockSpriteSize=20;
const int KCommentMaxLength=20;
const int KWeaponCrates = EWeaponAmount-1;
const int KBulletCrates = EBulletAmount-2;
const int KBlockHeightTableSize = 16*30;
const int KMaxMoneyInCrate = 1000;

const int KLevelVersion = 11;   // Version of levels made
EXPORT extern const char KBlockHeights[ KBlockHeightTableSize ];

struct TBlock
{
	TBlockType iType;
	int iNumber;
	int iShadow;
	int iHeight;
	int iCastDarkness;
};

class CLevel
{
public:

	enum TCrate
	{
		EWeaponCrate = 0,
		EBulletCrate,
		EHealthCrate,
		EMoneyCrate,
		ETargetCrate,
		ECrateAmount
	};

	enum TChunk
	{
		KUnknown = 0xffffffff,
		KLevelData = 0x1000, // let's start from big numbers, so that debugging is easier
		KPlayerStartPoints,
		KSpotLights,
		KSteams,
		KGeneralLevelInfo,
		KRandomWeaponCrateAmountNormal,
		KRandomBulletCrateAmountNormal,
		KRandomOtherCrateAmountNormal,
		KRandomWeaponCrateAmountDM,
		KRandomBulletCrateAmountDM,
		KRandomOtherCrateAmountDM,
		KPlacedCratesNormal,
		KPlacedCratesDM,
		KOutblockData,
		KPlacedEnemies,
		KOutblockHeight,
		KRandomCratesNormal,
		KRandomCratesDM,
	};

	struct TEnemyCoords
	{
	   int iType; // 1 J‰rjestys on t‰rke‰ ladatessa kentt‰‰
	   int iX;    // 2
	   int iY;    // 3
	};

	struct TCrateInfo
	{
	  int iType1;
	  int iType2;
	  int iX,iY;
	};

	class TRandomCrateInfo
	{
	public:
		inline TRandomCrateInfo( int aType1, int aType2, int aAmount ): iType1( aType1 ), iType2( aType2 ), iAmount( aAmount ){}
		int iType1;
		int iType2;
		int iAmount;
	};

	class TGeneralLevelInfo
	{
	public:
		EXPORT TGeneralLevelInfo();

		char iComment[KCommentMaxLength];
		int iTimeLimit;// seconds
		int iEnemies[EEnemyAmount];
	};

public:
	EXPORT CLevel();
	EXPORT ~CLevel();
	inline int Width(){ return iWidth;};
	inline int Height(){ return iHeight;};
	EXPORT void Load(const char* aName);
	EXPORT static char* ReadLevelName(const char* aName);
	EXPORT void Reset();

	inline bool InLevel(const CCoord<int>& aPos) const;
	inline bool InLevel(int aX,int aY) const {return InLevel(CCoord<int>(aX,aY));};
	inline bool Collision(const CCoord<int>& aPos) const;
	inline bool Collision(int aX,int aY) const {return Collision(CCoord<int>(aX,aY));};

	// How far can object move from src to dst without collision. Coordinates in pixels 
	// 1 pixel accuracy
	// Collision location is stored in aCollisionPoint (optional)
	// return true if two positions are visible each other
	EXPORT bool VisibilityCheck(const CCoord<float>& aSrc,const CCoord<float>& aDst,CCoord<float>* aCollisionPoint = NULL);

	// check if block is on floorlevel
	inline static bool FloorLevel( TBlock& aBlock );

	inline const TBlock& LevelData(const CCoord<int>& aPos) const;
	inline const TBlock& LevelData(int aX,int aY) const {return LevelData(CCoord<int>(aX,aY));};
	inline TBlock& LevelData(const CCoord<int>& aPos);
	inline TBlock& LevelData(int aX,int aY) {return LevelData(CCoord<int>(aX,aY));};

	inline TBlock& OutBlock(){ return iOutBlock;}
	EXPORT void AddSpotLight(int aX,int aY,int aSize);
	EXPORT void AddSteam(int aX,int aY,int aAngle,int aSpeed);
	EXPORT void AddEnemy(int aX,int aY,int aType);
	EXPORT void AddNormalCrate(int aX,int aY,int aType1,int aType2);
	EXPORT void AddDMCrate(int aX,int aY,int aType1,int aType2);
	EXPORT char* LevelFileName() {return iLevelFileName;};

	EXPORT bool Reachable(const CCoord<int>& aPos) const;
	EXPORT const CCoord<int>& StartPosition(int aIndex) const;
	EXPORT int StartPositions() const;

	EXPORT bool DustBlock(const CCoord<int>& aPos) const;
	EXPORT TRandomCrateInfo GetRandomCrateInfo(int aType1,int aType2,std::vector< TRandomCrateInfo >* aRandomCrateInfo) const;

	int iVersion;

	std::vector< TEnemyCoords > iEnemy;

	int iCntSpotLights;
	CSpotLight *iSpotLights;

	int iCntSteams;
	CSteam* iSteams;

	TGeneralLevelInfo iGeneralLevelInfo;
	std::vector< TRandomCrateInfo > iRandomCratesNormal;
	std::vector< TRandomCrateInfo > iRandomCratesDM;

	std::vector< TCrateInfo > iPlacedCratesNormal;
	std::vector< TCrateInfo > iPlacedCratesDM;

protected:
	EXPORT void LoadNewFormat(FILE* aFile);
	EXPORT void LoadOldFormat(FILE* aFile);

	EXPORT void ClearLevelInfo();
	EXPORT void GenerateShadows();
	EXPORT void DarknessCast();
	EXPORT void GenerateReachableMap();

	void ReadChunkHeader( TChunk& aBlockType, int& aBlockLength, FILE* aFptr );
	int ReadInt( FILE* aFptr );
	void AddSpotLight( FILE* fptr );
	void AddSteam( FILE* fptr );
	void RecurseReachable( const CCoord<int>& aPos, bool aHoriz );
	void ModifyBorderBlocks();

protected:
	int iWidth;
	int iHeight;
	CSteam* iSteamsLast;
	CSpotLight* iSpotLightsLast;
	TBlock* iLevelData;
	bool* iReachableBlock;
	TBlock iOutBlock;
	std::vector< CCoord<int> > iPlStart;
	char iLevelFileName[FILENAME_MAX];
};

inline bool CLevel::InLevel(const CCoord<int>& aPos) const
{
	if (aPos.X() >= 0 && aPos.Y()>= 0 && aPos.X() < iWidth && aPos.Y()< iHeight) return true;
	return false;
}

inline const TBlock& CLevel::LevelData(const CCoord<int>& aPos) const
{
	if (!InLevel(aPos))
		return iOutBlock;

	return iLevelData[aPos.Y()*iWidth+aPos.X()];
}

TBlock& CLevel::LevelData(const CCoord<int>& aPos)
{
	if (!InLevel(aPos))
		return iOutBlock;

	return iLevelData[aPos.Y()*iWidth+aPos.X()];
}

inline bool CLevel::Collision(const CCoord<int>& aPos) const
{
	if (InLevel(aPos) && 
		iLevelData[aPos.Y()*iWidth+aPos.X()].iType==EBlockTypeFloor)
		return false;

	return true;
}

bool CLevel::FloorLevel( TBlock& aBlock )
{
	if (aBlock.iType == EBlockTypeFloor ||
		aBlock.iHeight == 0)
		return true;

	return false;
}

#endif


