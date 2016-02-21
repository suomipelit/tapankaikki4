#include "CLevel.h"

#include "error.h"
#include "CSpotlight.h"
#include "CSteam.h"
#include "CMath.h"
#include "files.h"
#include "SDL_endian.h"

namespace
{
	const int KLastOldFormatVersion = 8;

	// TODO: Joku muu tuon noiden -1 ja -2 tilalle
	// TODO: Jotain tuon LEVEL_VERSION+1 tilalle kans
	const int KVersionWeapons[KLastOldFormatVersion + 1]={9,9,10,11,11,11,12,12,12}; // last must be same as DIFF_WEAPONS
	const int KVersionBullets[KLastOldFormatVersion + 1]={7,7,8,9,9,9,10,10,10};// last must be same as DIFF_BULLETS
	const int KVersionEnemies[KLastOldFormatVersion + 1]={7,7,7,7,8,8,8,10,10};// last must be same as DIFF_ENEMIES

	const int KDustBlockAmount = 18;
	const int KDustBlocks[KDustBlockAmount] = 
	{
		0,		  // 1
		16*4, 
		16*5, 
		16*5 + 2, 
		16*6 + 1, // 5
		16*6 + 1, 
		16*6 + 2, 
		16*7 + 1, 
		16*7 + 2, 
		16*7 + 3, // 10
		21,
		22,
		23,
		24,
		21+16,    // 15
		22+16,
		23+16,
		24+16
	};
};

char const KBlockHeights[ KBlockHeightTableSize ] =
{
	3,3,3,3,3,2,1,0,0,1,1,1,1,1,1,1,
	3,3,3,5,1,2,1,0,0,1,1,1,1,1,1,1,
	3,3,3,3,1,1,1,2,2,1,1,1,1,3,3,3,
    3,3,3,3,3,1,1,3,2,1,1,1,1,3,3,3,
	2,2,2,2,2,2,2,2,3,1,1,1,1,3,3,3,
	2,2,2,2,1,2,2,1,1,1,1,1,1,3,3,3,
	2,2,2,2,1,3,2,2,3,3,3,3,3,3,3,3,
	2,2,2,2,1,1,4,4,5,3,3,3,3,3,3,3,
	1,1,1,1,1,1,0,0,0,3,3,3,3,3,3,3,
	1,1,1,1,1,1,0,0,0,3,3,3,3,3,3,0,
	3,3,3,3,3,3,3,1,1,0,0,1,1,3,3,3,
	3,3,3,4,1,2,1,1,1,0,0,1,1,3,3,3,
	3,3,3,3,1,1,1,1,1,3,3,3,3,3,3,3,
    3,3,3,3,3,1,1,1,1,3,3,3,3,3,3,3,
	2,2,2,2,2,2,2,1,1,3,3,3,3,3,3,3,
	2,2,2,2,2,2,2,1,1,3,3,3,4,3,3,3,
	2,2,2,2,1,3,2,2,3,3,3,3,3,3,3,3,
	2,2,2,2,1,2,4,4,5,3,3,3,3,3,3,3,
	2,2,2,2,2,2,1,1,1,3,3,3,3,3,3,3,
	2,2,2,2,2,2,1,1,1,3,3,3,4,3,3,3,
	2,2,2,2,1,1,0,0,0,0,0,0,0,0,0,0,
	2,2,2,2,1,1,0,0,0,0,0,0,0,0,0,0,
	1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,
	1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,
	0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

CLevel::TGeneralLevelInfo::TGeneralLevelInfo()
{
	iComment[0] = 0;
	iTimeLimit = 0;
	for (int a=0;a<EEnemyAmount;a++)
		iEnemies[a] = 0;

}

void CLevel::Reset()
{
	CSpotLight* ptr1a,*ptr1b;
	CSteam* ptr2a,*ptr2b;

	ClearLevelInfo();

	iEnemy.clear();
	iPlacedCratesDM.clear();
	iPlacedCratesNormal.clear();

	iVersion=0;
	iWidth=0;
	iHeight=0;
	iCntSpotLights=0;
	iCntSteams=0;
	
	iPlStart.clear();

	iOutBlock.iCastDarkness=0;
	iOutBlock.iShadow=0;
	iOutBlock.iHeight=0;
	iOutBlock.iNumber=0;
	iOutBlock.iType=EBlockTypeFloor;
   
	delete[] iReachableBlock;
	iReachableBlock=NULL;
	delete[] iLevelData;
	iLevelData=NULL;

	ptr1a=iSpotLights;
	while (ptr1a)
	{
		ptr1b=ptr1a;
		ptr1a=ptr1a->Next();
		delete(ptr1b);
	}

	ptr2a=iSteams;
	while (ptr2a)
	{
		ptr2b=ptr2a;
		ptr2a=ptr2a->Next();
		delete(ptr2b);
	}

	iSpotLights=NULL;
	iSteams=NULL;
}

CLevel::CLevel(): iSpotLights(NULL), iSpotLightsLast(NULL), iSteams(NULL), iSteamsLast(NULL), iLevelData(NULL), iReachableBlock(NULL)
{
	Reset();
}

CLevel::~CLevel()
{
	Reset();
}
void CLevel::GenerateShadows()
{
    int x,y,a,iOffset=0, lenght;

	for(a=0;a<iHeight*iWidth;a++)
		iLevelData[a].iShadow=0;

	for(y=0;y<iHeight;y++)
	 for(x=0;x<iWidth;x++,iOffset++)
	  if(iLevelData[iOffset].iHeight>0)
	{
		lenght=1;
		if(iLevelData[iOffset].iHeight>2)
			lenght=2;
		if(iLevelData[iOffset].iHeight>4)
			lenght=3;
		for(a=1;a<lenght+1;a++)
		{
			if(x-a>=0&&y+a<iHeight)
			 if(iLevelData[iOffset+(iWidth*a)-a].iHeight<=iLevelData[iOffset].iHeight-a)
			  iLevelData[iOffset+(iWidth*a)-a].iShadow=1;
			if(x-a>=0&&y+a-1<iHeight)
			 if(iLevelData[iOffset+(iWidth*(a-1))-a].iHeight<=iLevelData[iOffset].iHeight-a)
			  switch(iLevelData[iOffset+(iWidth*(a-1))-a].iShadow)
			{
				case 0:
					iLevelData[iOffset+(iWidth*(a-1))-a].iShadow=2;
					break;
				case 1:
					iLevelData[iOffset+(iWidth*(a-1))-a].iShadow=1;
					break;
				case 2:
					iLevelData[iOffset+(iWidth*(a-1))-a].iShadow=2;
					break;
				case 3:
					iLevelData[iOffset+(iWidth*(a-1))-a].iShadow=1;
					break;
			}

			if(x-(a-1)>=0&&y+a<iHeight)
			 if(iLevelData[iOffset+(iWidth*a)-a+1].iHeight<=iLevelData[iOffset].iHeight-a)
			  switch(iLevelData[iOffset+(iWidth*a)-a+1].iShadow)
			{
				case 0:iLevelData[iOffset+(iWidth*a)-a+1].iShadow=3;
				break;
				case 1:iLevelData[iOffset+(iWidth*a)-a+1].iShadow=1;
				break;
				case 2:iLevelData[iOffset+(iWidth*a)-a+1].iShadow=1;
				break;
				case 3:iLevelData[iOffset+(iWidth*a)-a+1].iShadow=3;
				break;
			}
		}
	}
}

void CLevel::Load(const char* aName) 
{
	ASSERT(aName);
	ASSERT(strlen(aName)>0);
	FILE *dat;
	dat=fopen(getdatapath(std::string(aName)).c_str(), "rb");
	if (dat != NULL) 
	{
		Reset();

		strcpy(iLevelFileName,aName);

		try
		{
			iVersion = ReadInt(dat);
		}
		catch ( CEOFException e )
		{
			throw CFailureException("CLevel::Load: Level file too short.");
		}

		fseek( dat, 0, SEEK_SET );

		if ( iVersion > 100 ) 
			throw CFailureException("CLevel::Load: Level file corrupted?");

		try
		{
			if (iVersion>KLastOldFormatVersion)
				LoadNewFormat( dat );
			else
				LoadOldFormat( dat );
		}
		catch ( CGameException e )
		{
			fclose( dat );
			throw;
		}
		
		fclose( dat );

		strcpy(iLevelFileName,aName);
		GenerateReachableMap();

		ModifyBorderBlocks();
		DarknessCast();
		GenerateShadows();
	}
	else
        throw CFailureException("CLevel::Load: File could not be opened!");
}

void CLevel::LoadNewFormat(FILE* aFile)
{
	int version = ReadInt( aFile );

	while ( !feof( aFile ) )
	{
		TChunk blockType;
		int blockLength;

		try
		{
			ReadChunkHeader( blockType, blockLength, aFile);
		}
		catch ( CEOFException& e )
		{
			LOG1("%s\n", e.what() );
			return; // file end reached
		}

		switch ( blockType )
		{
			case KLevelData:
				{
					iWidth = ReadInt( aFile );
					iHeight = ReadInt( aFile );
					iLevelData = new TBlock[iHeight*iWidth];

					for (int a=0;a<iWidth * iHeight;a++)
					{
						iLevelData[a].iType = (TBlockType)(ReadInt( aFile));
						iLevelData[a].iNumber = ReadInt( aFile );
						iLevelData[a].iHeight = ReadInt( aFile );
					}

					break;
				}
			case KPlayerStartPoints:
				{
					int amount = ReadInt( aFile );

					for (int a=0;a<amount;a++)
					{
						int x = ReadInt( aFile );
						int y = ReadInt( aFile );
						iPlStart.push_back( CCoord<int>( x,y ) );
					}
					break;
				}
			case KSpotLights:
				{
					int amount = ReadInt( aFile );
					for (int a=0;a<amount;a++)
					{
						AddSpotLight( aFile );
					}
                    
					break;
				}
			case KSteams:
				{
					int amount = ReadInt( aFile );
					for (int a=0;a<amount;a++)
					{
						AddSteam( aFile );
					}
                    
					break;
				}
			case KGeneralLevelInfo:
				{
					// FIXME: endianness problem
					int ret = fread(&iGeneralLevelInfo,sizeof(iGeneralLevelInfo),1,aFile);
					if ( ret != 1 )
						throw CFailureException("CLevel::LoadNewFormat: KGeneralLevelInfo chunk read failed!");
					break;
				}
			case KRandomWeaponCrateAmountNormal:
				{
					int amount = ReadInt( aFile );

					for (int a=0;a<amount;a++)
					{
						int crateAmount = ReadInt( aFile );
						iRandomCratesNormal.push_back( TRandomCrateInfo(0,a,crateAmount) );
					};
					break;
				}
			case KRandomBulletCrateAmountNormal:
				{
					int amount = ReadInt( aFile );

					for (int a=0;a<amount;a++)
					{
						int crateAmount = ReadInt( aFile );
						iRandomCratesNormal.push_back( TRandomCrateInfo(1,a,crateAmount) );
					};
					break;

				}
			case KRandomOtherCrateAmountNormal:
				{
					int amount = ReadInt( aFile );
					ASSERT( amount == 1 );
					int crateAmount = ReadInt( aFile );
					iRandomCratesNormal.push_back( TRandomCrateInfo(2,0,crateAmount) );
					break;
				}
			case KRandomWeaponCrateAmountDM:
				{
					int amount = ReadInt( aFile );

					for (int a=0;a<amount;a++)
					{
						int crateAmount = ReadInt( aFile );
						iRandomCratesDM.push_back( TRandomCrateInfo(0,a,crateAmount) );
					};
					break;
				}
			case KRandomBulletCrateAmountDM:
				{
					int amount = ReadInt( aFile );

					for (int a=0;a<amount;a++)
					{
						int crateAmount = ReadInt( aFile );
						iRandomCratesDM.push_back( TRandomCrateInfo(1,a,crateAmount) );
					};
					break;
				}
			case KRandomOtherCrateAmountDM:
				{
					int amount = ReadInt( aFile );
					ASSERT( amount == 1 );
					int crateAmount = ReadInt( aFile );
					iRandomCratesDM.push_back( TRandomCrateInfo(2,0,crateAmount) );
					break;
				}
			case KPlacedCratesNormal:
				{
					int amount = ReadInt( aFile );
					
					for (int a=0;a<amount;a++)
					{
						TCrateInfo tmpCrate;
						// FIXME: endianness problem
						if (fread(&tmpCrate, sizeof(TCrateInfo), 1, aFile)!=1) 
							throw CFailureException("CLevel::LoadNewFormat: File loading error 1");

						iPlacedCratesNormal.push_back( tmpCrate );
					}
					break;
				}
			case KPlacedCratesDM:
				{
					int amount = ReadInt( aFile );

					for (int a=0;a<amount;a++)
					{
						TCrateInfo tmpCrate;
						// FIXME: endianness problem
						if (fread(&tmpCrate, sizeof(TCrateInfo), 1, aFile)!=1) 
							throw CFailureException("CLevel::LoadNewFormat: File loading error 2");
						
						iPlacedCratesDM.push_back( tmpCrate );
					}
					break;
				}
			case KOutblockData:
				{
					iOutBlock.iNumber = ReadInt( aFile );
					iOutBlock.iType = (TBlockType)(ReadInt( aFile ));
					break;
				}
			case KPlacedEnemies:
				{
					int amount = ReadInt( aFile );

					for (int a=0;a<amount;a++)
					{
						TEnemyCoords tmpEnemy;
						// FIXME: endianness problem
						if (fread(&tmpEnemy, sizeof(TEnemyCoords), 1, aFile)!=1) 
							throw CFailureException("CLevel::LoadNewFormat: File loading error 3");

						iEnemy.push_back( tmpEnemy );
					}
					break;
				}
			case KOutblockHeight:
				{
					iOutBlock.iHeight = ReadInt( aFile );
					break;
				}

			case KRandomCratesNormal:
				{
					int amount = ReadInt( aFile );

					for (int a=0;a<amount;a++)
					{
						int t1,t2,amo;
						t1  = ReadInt( aFile );
						t2  = ReadInt( aFile );
						amo = ReadInt( aFile );
						iRandomCratesNormal.push_back( TRandomCrateInfo( t1, t2, amo ) );
					}
					break;
				}
			case KRandomCratesDM:
				{
					int amount = ReadInt( aFile );

					for (int a=0;a<amount;a++)
					{
						int t1,t2,amo;
						t1  = ReadInt( aFile );
						t2  = ReadInt( aFile );
						amo = ReadInt( aFile );
						iRandomCratesDM.push_back( TRandomCrateInfo( t1, t2, amo ) );
					}
					break;
				}
			case KUnknown:
				fseek( aFile, blockLength, SEEK_CUR ); // skip unknown blocktype
				break;
			default:
				throw CFailureException("CLevel::LoadNewFormat: Chunk detection failure!");
		};
	}
}

void CLevel::LoadOldFormat(FILE* aFile)
{
	int a, bullets, weapons;

	try {
		iVersion = ReadInt(aFile);
	} catch (CFailureException e) {
		throw CFailureException("CLevel::LoadOldFormat: version read failure");
	}
	if (iVersion>KLastOldFormatVersion) 
		throw CCriticalException("CLevel::LoadOldFormat: loading new file with old loader!");

	try {
		iWidth = ReadInt(aFile);
		iHeight = ReadInt(aFile);
	} catch (CFailureException e) {
		throw CFailureException("CLevel::LoadOldFormat: File loading error 1");
	}

	iLevelData = new TBlock[iHeight*iWidth];

	for (a=0;a<iWidth * iHeight;a++)
	{
		try {
			iLevelData[a].iType = (TBlockType) ReadInt(aFile);
			iLevelData[a].iNumber = ReadInt(aFile);
		} catch (CFailureException e) {
			throw CFailureException("CLevel::LoadOldFormat: File loading error 2");
		}
		if (iVersion<6)
		{
			try {
				iLevelData[a].iShadow = ReadInt(aFile);
			} catch (CFailureException e) {
				throw CFailureException("CLevel::LoadOldFormat: File loading error 3");
			}
 			if (iLevelData[a].iType==EBlockTypeWall)
			{
				ASSERT( iLevelData[a].iNumber < KBlockHeightTableSize );
				iLevelData[a].iHeight=KBlockHeights[iLevelData[a].iNumber];
			}
			else
				iLevelData[a].iHeight=0;
		}
        else 
		{
			try {
				iLevelData[a].iHeight = ReadInt(aFile);
			} catch (CFailureException e) {
				throw CFailureException("CLevel::LoadOldFormat: File loading error 4");
			}
			if (iLevelData[a].iType==EBlockTypeFloor) 
				iLevelData[a].iHeight=0; 
		}
	}
	int x,y,players,tmp;

	if (iVersion<8) 
		players = 2;
	else 
		players = KMaxPlayers;

	iPlStart.clear();
		
	for (a=0;a<players;a++)
	{
		x = ReadInt( aFile );
		y = ReadInt( aFile );
		if (x!=0 || y!=0)
			iPlStart.push_back( CCoord<int>(x,y) );
	}

	try {
		tmp = ReadInt(aFile);
	} catch (CFailureException e) {
		throw CFailureException("CLevel::LoadOldFormat: File loading error 5");
	}
	for (a=0;a<tmp;a++) 
	{
		AddSpotLight(aFile);
	}

	try {
		tmp = ReadInt(aFile);
	} catch (CFailureException e) {
		throw CFailureException("CLevel::LoadOldFormat: File loading error 6");
	}
	for (a=0;a<tmp;a++) 
	{
		AddSteam(aFile);
	}

	if (fread(iGeneralLevelInfo.iComment,20, 1, aFile)<=0) throw CFailureException("CLevel::LoadOldFormat: File loading error 7");
	try {
		iGeneralLevelInfo.iTimeLimit = ReadInt(aFile);
	} catch (CFailureException e) {
		throw CFailureException("CLevel::LoadOldFormat: File loading error 8");
	}
	memset(iGeneralLevelInfo.iEnemies,0,4*EEnemyAmount);

	if (KVersionEnemies[iVersion])
		// FIXME: endianness problem
		if (fread(iGeneralLevelInfo.iEnemies,4*KVersionEnemies[iVersion], 1, aFile)<=0) throw CFailureException("CLevel::LoadOldFormat: File loading error 9");

	memset( &iRandomCratesNormal, 0, sizeof( iRandomCratesNormal ) ); 
	memset( &iRandomCratesDM, 0, sizeof( iRandomCratesDM ) ); 
	weapons=KVersionWeapons[iVersion];
    bullets=KVersionBullets[iVersion];

	for (a=0;a<weapons;a++)
		iRandomCratesNormal.push_back( TRandomCrateInfo(0,a,ReadInt( aFile )) );
	for (a=0;a<bullets;a++)
		iRandomCratesNormal.push_back( TRandomCrateInfo(1,a,ReadInt( aFile )) );
	iRandomCratesNormal.push_back( TRandomCrateInfo(2,0,ReadInt( aFile )) );

	for (a=0;a<weapons;a++)
		iRandomCratesDM.push_back( TRandomCrateInfo(0,a,ReadInt( aFile )) );
	for (a=0;a<bullets;a++)
		iRandomCratesDM.push_back( TRandomCrateInfo(1,a,ReadInt( aFile )) );
	iRandomCratesDM.push_back( TRandomCrateInfo(2,0,ReadInt( aFile )) );

	if (iVersion>=5)
	{
		int tmp;

		try {
			tmp = ReadInt(aFile);
		} catch (CFailureException e) {
			throw CFailureException("CLevel::LoadOldFormat: File loading error 10");
		}

		iPlacedCratesNormal.reserve(tmp);
		for (int a=0;a<tmp;a++)
		{
			TCrateInfo tmpCrate;
			// FIXME: endianness problem
			if (fread(&tmpCrate, sizeof(TCrateInfo), 1, aFile)!=1) throw CFailureException("CLevel::LoadOldFormat: File loading error 11");

			iPlacedCratesNormal.push_back( tmpCrate );
		}

		try {
			tmp = ReadInt(aFile);
		} catch (CFailureException e) {
			throw CFailureException("CLevel::LoadOldFormat: File loading error 12");
		}

		iPlacedCratesDM.reserve(tmp);
		for (int a=0;a<tmp;a++)
		{
			TCrateInfo tmpCrate;
			// FIXME: endianness problem
			if (fread(&tmpCrate, sizeof(TCrateInfo), 1, aFile)!=1) throw CFailureException("CLevel::LoadOldFormat: File loading error 13");

			iPlacedCratesDM.push_back( tmpCrate );
		}
	}

	if (iVersion>=6)
	{
		int tmp;
		try {
			iOutBlock.iNumber = ReadInt(aFile);
			iOutBlock.iType = (TBlockType) ReadInt(aFile);
			tmp = ReadInt(aFile);
		} catch (CFailureException e) {
			throw CFailureException("CLevel::LoadOldFormat: File loading error 14");
		}

		iEnemy.reserve(tmp);
		for (a=0;a<tmp;a++)
		{
			TEnemyCoords enemy;
			// FIXME: endianness problem
			if (fread(&enemy,sizeof(TEnemyCoords), 1, aFile)!=1) throw CFailureException("CLevel::LoadOldFormat: File loading error 24");

			iEnemy.push_back( enemy );
		}
				
	} 
    else 
	{
		iOutBlock.iNumber = 16*10-1;
		iOutBlock.iType = EBlockTypeWall;
    }
	
    if (iOutBlock.iType==EBlockTypeFloor) 
		iOutBlock.iHeight=0; 
    else 
	{
		ASSERT( iOutBlock.iNumber < KBlockHeightTableSize );
		iOutBlock.iHeight=KBlockHeights[iOutBlock.iNumber];
	}
}

void CLevel::AddSpotLight(int aX,int aY,int aSize)
{
	CSpotLight* ptr=new CSpotLight((float)aX,(float)aY,aSize);
	
	if (iSpotLights==NULL)
	{
		iSpotLights=ptr;
	}
	else
	{
		iSpotLightsLast->SetNext(ptr);
	}
	iSpotLightsLast=ptr;
	iCntSpotLights++;
}



void CLevel::AddSpotLight(FILE* fptr)
{
	ASSERT(fptr);
	CSpotLight* ptr=new CSpotLight(fptr);
	
	if (iSpotLights==NULL)
	{
		iSpotLights=ptr;
	}
	else
	{
		iSpotLightsLast->SetNext(ptr);
	}
	iSpotLightsLast=ptr;
	iCntSpotLights++;
}

void CLevel::AddSteam(int aX,int aY,int aAngle,int aSpeed)
{
	CSteam* ptr=new CSteam((float)aX, (float)aY, aAngle, aSpeed);
	
	if (iSteams==NULL)
	{
		iSteams=ptr;
	}
	else
	{
		iSteamsLast->SetNext(ptr);
	}
	iSteamsLast=ptr;
	iCntSteams++;
}

void CLevel::AddSteam(FILE* fptr)
{
	ASSERT(fptr);
	CSteam* ptr=new CSteam(fptr, iVersion);
	
	if (iSteams==NULL)
	{
		iSteams=ptr;
	}
	else
	{
		iSteamsLast->SetNext(ptr);
	}
	iSteamsLast=ptr;
	iCntSteams++;
}

void CLevel::AddEnemy(int aX,int aY,int aType)
{
	TEnemyCoords enemy;
	enemy.iX = aX;
	enemy.iY = aY;
	enemy.iType = aType;

	iEnemy.push_back( enemy );
}

void CLevel::AddNormalCrate(int aX,int aY,int aType1,int aType2)
{
	TCrateInfo tmp;
	tmp.iX = aX;
	tmp.iY = aY;
	tmp.iType1 = aType1;
	tmp.iType2 = aType2;

	iPlacedCratesNormal.push_back( tmp );
}

void CLevel::AddDMCrate(int aX,int aY,int aType1,int aType2)
{
	TCrateInfo tmp;
	tmp.iX = aX;
	tmp.iY = aY;
	tmp.iType1 = aType1;
	tmp.iType2 = aType2;
	
	iPlacedCratesDM.push_back( tmp );
}

void CLevel::ClearLevelInfo()
{
	int a;

	strset(iGeneralLevelInfo.iComment,0);
	iGeneralLevelInfo.iTimeLimit=0;

	iRandomCratesDM.clear();
	iRandomCratesNormal.clear();

	for (a=0;a<EEnemyAmount;a++) iGeneralLevelInfo.iEnemies[a]=0;
}

void CLevel::DarknessCast()
{
	int a,b;

	// Leave borders out
	for(a=1;a<iHeight-1;a++)
		for(b=1;b<iWidth-1;b++)
			if (LevelData(b,a).iHeight == 0) 
					iLevelData[a*iWidth+b].iCastDarkness=0;
				else 
					iLevelData[a*iWidth+b].iCastDarkness=1;
}


// TODO: this could be optimized with better algorithm?
bool CLevel::VisibilityCheck(const CCoord<float>& aSrc,const CCoord<float>& aDst,CCoord<float>* aCollisionPoint)
{
	int a;
    int iterations=int(max(1.0f,max(abs(aDst.X()-aSrc.X()),abs(aDst.Y()-aSrc.Y()))));
	int x_sign = sign(aDst.X()-aSrc.X());
	int y_sign = sign(aDst.Y()-aSrc.Y());
	float xDelta=((float)aDst.X()-aSrc.X())/iterations;
	float yDelta=((float)aDst.Y()-aSrc.Y())/iterations;
	CCoord<float> oldPos = aSrc;
	CCoord<float> newPos;
    
	for (a=0;a<=iterations;a++)
	{
		newPos = aSrc.Add(xDelta*a,yDelta*a);
		DEBUG_ASSERT((a==0) || (newPos != oldPos));

		if (Collision(newPos.Div(KBlockSpriteSize)))
		{
			if (aCollisionPoint)
				(*aCollisionPoint) = oldPos;
            return false;
		}

		if (x_sign!=0 && y_sign !=0 && a>0)
		{
			if (Collision(newPos.Add(-x_sign,0).Div(KBlockSpriteSize)) &&
				Collision(newPos.Add(0,-y_sign).Div(KBlockSpriteSize)))
				{
					if (aCollisionPoint)
						(*aCollisionPoint) = oldPos;
					return false;
				}
		}

		oldPos = newPos;
	}

	if (aCollisionPoint)
		(*aCollisionPoint) = aDst;
	return true;
}

void CLevel::GenerateReachableMap()
{
	delete[] iReachableBlock;
	iReachableBlock = new bool[iWidth*iHeight];
	for (int a=0;a<iWidth*iHeight;a++)
		iReachableBlock[a]=false;

	RecurseReachable(iPlStart[0], true); // have to do both ways to make sure
}

void CLevel::RecurseReachable(const CCoord<int>& aPos, bool aHoriz)
{
	if (!InLevel(aPos)) return;
	if (Collision(aPos)) return;
	if (Reachable(aPos)) return;

	if (aHoriz)
	{
		int xmin=aPos.X(), xmax=aPos.X(),y=aPos.Y(),a;
		do 
		{
			xmin--;
		} while (!Collision(xmin,y)&&!Reachable(CCoord<int>(xmin,y))&&InLevel(xmin,y));
		do
		{
			xmax++;
		} while (!Collision(xmax,y)&&!Reachable(CCoord<int>(xmax,y))&&InLevel(xmax,y));


		for (a=xmin+1;a<xmax;a++)
			iReachableBlock[y*iWidth + a] = true;
		if (y-1>=0)
			for (a=xmin+1;a<xmax;a++)
				RecurseReachable(CCoord<int>(a,y-1),!aHoriz);

		if (y+1<iHeight)
			for (a=xmin+1;a<xmax;a++)
				RecurseReachable(CCoord<int>(a,y+1),!aHoriz);
	}
	else
	{
		int ymin=aPos.Y(), ymax=aPos.Y(),x=aPos.X(),a;
		do
		{
			ymin--;
		} while (!Collision(x,ymin)&&!Reachable(CCoord<int>(x,ymin))&&InLevel(x,ymin));
		do
		{
			ymax++;
		} while (!Collision(x,ymax)&&!Reachable(CCoord<int>(x,ymax))&&InLevel(x,ymax));

		for (a=ymin+1;a<ymax;a++)
			iReachableBlock[a*iWidth + x] = true;

		if (x-1>=0)
			for (a=ymin+1;a<ymax;a++)
				RecurseReachable(CCoord<int>(x-1,a),!aHoriz);

		if (x+1<iWidth)
			for (a=ymin+1;a<ymax;a++)
				RecurseReachable(CCoord<int>(x+1,a),!aHoriz);
	}
}

EXPORT bool CLevel::Reachable(const CCoord<int>& aPos) const
{
	ASSERT(iReachableBlock);
	if (!InLevel(aPos)) return false;
	return iReachableBlock[aPos.Y()*iWidth + aPos.X()];
}

EXPORT char* CLevel::ReadLevelName(const char* aName)
{
	CLevel* lev=new CLevel;
	char *tmp;
	try
	{
		lev->Load(aName);
		tmp = strdup( lev->iGeneralLevelInfo.iComment );
		ASSERT( tmp );
	}
	catch (CFailureException& exception)
	{
		LOG2( "File reading failed: %s, Exception: %s\n", aName, exception.what());
		tmp = strdup( "(Level loading error!)" );
		ASSERT( tmp );
	}

	delete lev;
	return tmp;
}

EXPORT const CCoord<int>& CLevel::StartPosition(int aIndex) const
{
	ASSERT( aIndex < iPlStart.size() );
	return iPlStart[ aIndex ];
}

EXPORT int CLevel::StartPositions() const
{
	return (int)iPlStart.size();
}

/**
* ModifyBorderBlocks: Jos joku reunablockeista on matalampi kuin outblock, niin
* se korotetaan outblockin tasolle. Tämä siksi että grafiikka ei bugita...
* Lisäksi mahdolliset lattia palikat muunnetaan seinäksi...
*/
void CLevel::ModifyBorderBlocks()
{
	int a;
	for (a=0;a<Height();a++)
	{
		if (LevelData(0,a).iType==EBlockTypeFloor) 
			LevelData(0,a).iType= EBlockTypeWall;
		if (LevelData(Width()-1,a).iType==EBlockTypeFloor)
			LevelData(Width()-1,a).iType= EBlockTypeWall;

		ASSERT( iOutBlock.iNumber < KBlockHeightTableSize );

		if (LevelData(0,a).iHeight<KBlockHeights[iOutBlock.iNumber]) 
			LevelData(0,a).iHeight =  KBlockHeights[iOutBlock.iNumber];
		if (LevelData(Width()-1, a).iHeight<KBlockHeights[iOutBlock.iNumber]) 
			LevelData(Width()-1, a).iHeight = KBlockHeights[iOutBlock.iNumber];
	}
	for (a=0;a<Width();a++)
	{
		if (LevelData(a,0).iType==EBlockTypeFloor) 
			LevelData(a,0).iType= EBlockTypeWall;
		if (LevelData(a,Height()-1).iType==EBlockTypeFloor) 
			LevelData(a,Height()-1).iType= EBlockTypeWall;
		
		ASSERT( iOutBlock.iNumber < KBlockHeightTableSize );

		if (LevelData(a,0).iHeight<KBlockHeights[iOutBlock.iNumber]) 
			LevelData(a,0).iHeight = KBlockHeights[iOutBlock.iNumber];
		if (LevelData(a, Height()-1).iHeight<KBlockHeights[iOutBlock.iNumber]) 
			LevelData(a, Height()-1).iHeight = KBlockHeights[iOutBlock.iNumber];
	}
}

void CLevel::ReadChunkHeader( TChunk& aBlockType, int& aBlockLength, FILE* aFptr )
{
	aBlockType = (TChunk) ReadInt( aFptr );
	aBlockLength = ReadInt( aFptr );
}

int CLevel::ReadInt( FILE* aFptr )
{
	int temp;
	if ( fread( &temp, sizeof(int), 1, aFptr ) != 1 )
	{
		if ( feof( aFptr ) )
			throw CEOFException("CLevel::ReadInt: file end reached!");
		else
			throw CFailureException("CLevel::ReadInt: fread failed!");
	}

	return SDL_SwapLE32(temp);
} 

bool CLevel::DustBlock(const CCoord<int>& aPos) const
{
	int num = LevelData(aPos).iNumber;
	for (int a=0;a<KDustBlockAmount;a++) 
		if (num==KDustBlocks[a]) 
		{
			return true;
		}

	return false;
}

CLevel::TRandomCrateInfo CLevel::GetRandomCrateInfo(int aType1,int aType2,std::vector< TRandomCrateInfo >* aRandomCrateInfo) const
{
	for (int a=0;a < aRandomCrateInfo->size();a++)
		if (aRandomCrateInfo->at(a).iType1 == aType1 && 
			aRandomCrateInfo->at(a).iType2 == aType2) return aRandomCrateInfo->at(a);
	TRandomCrateInfo NullCrate(aType1,aType2,0);
	return NullCrate;
}
// EOF
