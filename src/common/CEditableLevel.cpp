#include "CEditableLevel.h"
#include "error.h"
#include "CSpotlight.h"
#include "CSteam.h"

int CEditableLevel::Save(const char* aName)
{
	ASSERT(aName);
	ASSERT(strlen(aName)>0);
	int a;
	iVersion=KLevelVersion;
	FILE *file;

	TrimRandomCrates( iRandomCratesNormal );
	TrimRandomCrates( iRandomCratesDM );

	file=fopen(aName,"wb");
	if (file!=NULL)
	{ 
		strcpy(iLevelFileName,aName);

		WriteInt(iVersion, file);

		// --------------- KLevelData --------------
		WriteChunkHeader( KLevelData, 8 + iWidth*iHeight*12 , file ); // height, width + height*width*blockfilea
		WriteInt( iWidth, file );
		WriteInt( iHeight, file );

		for (a=0;a<iWidth * iHeight;a++)
		{
			WriteInt( iLevelData[a].iType, file);
			WriteInt( iLevelData[a].iNumber, file);
			WriteInt( iLevelData[a].iHeight, file);
		}
		
		// --------------- KPlayerStartPoints --------------
		WriteChunkHeader( KPlayerStartPoints, 4 + iPlStart.size()*8, file );

		WriteInt( iPlStart.size(), file );

		for (a=0;a<iPlStart.size();a++)
		{
			WriteInt( iPlStart[a].X(), file);
			WriteInt( iPlStart[a].Y(), file);
		}
		// --------------- KSpotLights --------------
		WriteChunkHeader( KSpotLights, 4 + iCntSpotLights*12, file );

		WriteInt( iCntSpotLights, file );

		CSpotLight* slptr=iSpotLights;

		a=0;
		while (slptr)
		{
			slptr->WriteToFile( file );
			slptr=slptr->Next();
			a++;
		}

		ASSERT(a==iCntSpotLights);
		// --------------- KSteams --------------
		WriteChunkHeader( KSteams, 4 + iCntSteams*16, file );

		WriteInt(iCntSteams,file);
		CSteam* sptr=iSteams;

		a=0;
		while (sptr)
		{
			sptr->WriteToFile(file);
			sptr=sptr->Next();
			a++;
		}

		ASSERT(a==iCntSteams);
		// --------------- KGeneralLevelInfo --------------
		WriteChunkHeader( KGeneralLevelInfo, sizeof(iGeneralLevelInfo), file );

		fwrite(&iGeneralLevelInfo,sizeof(iGeneralLevelInfo),1,file);

		// --------------- KRandomCratesNormal --------------
		WriteChunkHeader( KRandomCratesNormal, sizeof(int)*(1+3*iRandomCratesNormal.size()), file );
		WriteInt( iRandomCratesNormal.size(), file );
		for (int a=0;a<iRandomCratesNormal.size();a++)
		{
			WriteInt( iRandomCratesNormal[a].iType1, file );
			WriteInt( iRandomCratesNormal[a].iType2, file );
			WriteInt( iRandomCratesNormal[a].iAmount, file );
		}

		// --------------- KRandomCratesDM ------------------
		WriteChunkHeader( KRandomCratesDM, sizeof(int)*(1+3*iRandomCratesDM.size()), file );
		WriteInt( iRandomCratesDM.size(), file );
		for (int a=0;a<iRandomCratesDM.size();a++)
		{
			WriteInt( iRandomCratesDM[a].iType1, file );
			WriteInt( iRandomCratesDM[a].iType2, file );
			WriteInt( iRandomCratesDM[a].iAmount, file );
		}


		// --------------- KPlacedCratesNormal --------------
		WriteChunkHeader( KPlacedCratesNormal, 4 + iPlacedCratesNormal.size()*sizeof(TCrateInfo), file );

		WriteInt(iPlacedCratesNormal.size(),file);
		for (int a=0;a<iPlacedCratesNormal.size();a++)
		{
			fwrite(&iPlacedCratesNormal[a],sizeof(TCrateInfo), 1, file);
		}		
		// --------------- KPlacedCratesDM --------------
		WriteChunkHeader( KPlacedCratesDM, 4 + sizeof(TCrateInfo)*iPlacedCratesDM.size(), file );
		
		WriteInt(iPlacedCratesDM.size(),file);
		for (int a=0;a<iPlacedCratesDM.size();a++)
		{
			fwrite(&iPlacedCratesDM[a],sizeof(TCrateInfo), 1, file);
		}		
		// --------------- KOutblockData --------------
		WriteChunkHeader( KOutblockData, 8, file );

		WriteInt(iOutBlock.iNumber,file);
		WriteInt(iOutBlock.iType,file);
		// --------------- KPlacedEnemies --------------
		WriteChunkHeader( KPlacedEnemies, 4 + sizeof(TEnemyCoords)*iEnemy.size(), file );

		WriteInt(iEnemy.size(), file);
		for (int a=0;a<iEnemy.size();a++)
		{
			fwrite(&iEnemy[a],sizeof(TEnemyCoords), 1, file);
		}
		// --------------- KOutblockHeight --------------
		WriteChunkHeader( KOutblockHeight, 4, file );
		WriteInt(iOutBlock.iHeight,file);

		fclose(file);

		return 1;
	} 
	else 
		return 0;
}

EXPORT void CEditableLevel::AddStartPoint( CCoord<int>& aStartLocation )
{
	iPlStart.push_back( aStartLocation );
}

EXPORT void CEditableLevel::ModifyStartPoint(int aIndex, CCoord<int>& aStartLocation)
{
	ASSERT( aIndex < iPlStart.size() );
	iPlStart[aIndex] = aStartLocation;
}

void CEditableLevel::New(int aXsize,int aYsize,TBlockType aType,int aNum,int aHeight)
{
	ASSERT(aXsize>0);
	ASSERT(aYsize>0);
	int a;

	Reset();

	iWidth=aXsize;
	iHeight=aYsize;
	iVersion=KLevelVersion;
	
	ASSERT(!iLevelData);
	iLevelData=new TBlock[iHeight*iWidth];
	ASSERT(iLevelData);
	for (a=0;a<iHeight*iWidth;a++)
	{
		iLevelData[a].iType=aType;	  
		iLevelData[a].iNumber=aNum;	  
		iLevelData[a].iHeight=aHeight;	  
	}

	for (a=0;a<KMaxLocalPlayers;a++)
		iPlStart.push_back( CCoord<int>(1,1+a) );

	MakeBorderWalls();
	strcpy(iLevelFileName,"");
}

void CEditableLevel::DeleteSpotLight(CSpotLight *aPtr)
{
	ASSERT(aPtr);
	CSpotLight** ptr=&iSpotLights;
	CSpotLight* next=aPtr->Next();
	
	if (iCntSpotLights > 0) iCntSpotLights--;
	
	while (*ptr)
	{

		if (next==NULL&&(*ptr)->Next()==aPtr)
		{
			iSpotLightsLast=*ptr;
		}

		if (*ptr==aPtr)
		{
			*ptr=next;
			delete aPtr;
			return;
		}		

		ptr=&((*ptr)->Next());
	}
	ASSERT(0);
}
void CEditableLevel::DeleteSteam(CSteam *aPtr)
{
	ASSERT(aPtr);
	CSteam** ptr=&iSteams;
	CSteam* next=aPtr->Next();

	if (iCntSteams > 0) iCntSteams--;

	while (*ptr)
	{

		if (next==NULL&&(*ptr)->Next()==aPtr)
		{
			iSteamsLast=*ptr;
		}

		if (*ptr==aPtr)
		{
			*ptr=next;
			delete aPtr;
			return;
		}		

		ptr=&((*ptr)->Next());
	}
	ASSERT(0);
}

void CEditableLevel::DeleteEnemy(int aIndex)
{
	iEnemy.erase( iEnemy.begin()+aIndex );
}
void CEditableLevel::DeleteNormalCrate(int aIndex)
{
	iPlacedCratesNormal.erase( iPlacedCratesNormal.begin() + aIndex );
}

void CEditableLevel::DeleteDMCrate(int aIndex)
{
	iPlacedCratesDM.erase( iPlacedCratesDM.begin() + aIndex );
}

void CEditableLevel::MakeBorderWalls()
{
	int a;

    for (a=0;a<iWidth;a++)
	{
        iLevelData[a].iType=EBlockTypeWall;
        iLevelData[a].iNumber=1;

		ASSERT( 1 < KBlockHeightTableSize );
        iLevelData[a].iHeight=KBlockHeights[1];
        iLevelData[a+(iWidth*(iHeight-1))].iType=EBlockTypeWall;
        iLevelData[a+(iWidth*(iHeight-1))].iNumber=1;
        iLevelData[a+(iWidth*(iHeight-1))].iHeight=KBlockHeights[1];
    }

    for (a=0;a<iHeight;a++)
	{
        iLevelData[a*iWidth].iType=EBlockTypeWall;
        iLevelData[a*iWidth].iNumber=16;
		ASSERT( 16 < KBlockHeightTableSize );
        iLevelData[a*iWidth].iHeight=KBlockHeights[16];
        iLevelData[a*iWidth+iWidth-1].iType=EBlockTypeWall;
        iLevelData[a*iWidth+iWidth-1].iNumber=16;
		ASSERT( 16 < KBlockHeightTableSize );
        iLevelData[a*iWidth+iWidth-1].iHeight=KBlockHeights[16];
    }

    iLevelData[0].iType=EBlockTypeWall;
    iLevelData[0].iNumber=0;
    iLevelData[0].iHeight=KBlockHeights[0];
    iLevelData[iWidth-1].iType=EBlockTypeWall;
    iLevelData[iWidth-1].iNumber=2;
	ASSERT( 2 < KBlockHeightTableSize );
    iLevelData[iWidth-1].iHeight=KBlockHeights[2];
    iLevelData[(iHeight-1)*iWidth].iType=EBlockTypeWall;
    iLevelData[(iHeight-1)*iWidth].iNumber=32;
	ASSERT( 32 < KBlockHeightTableSize );
    iLevelData[(iHeight-1)*iWidth].iHeight=KBlockHeights[32];
    iLevelData[(iHeight*iWidth)-1].iType=EBlockTypeWall;
    iLevelData[(iHeight*iWidth)-1].iNumber=18;
	ASSERT( 18 < KBlockHeightTableSize );
    iLevelData[(iHeight*iWidth)-1].iHeight=KBlockHeights[18];
}

EXPORT CCoord<int>& CEditableLevel::StartPosition(int aIndex)
{
	ASSERT( aIndex < iPlStart.size() );
	return iPlStart[ aIndex ];
}

// some utilities
EXPORT void CEditableLevel::WriteInt( int aValue, FILE* aFP )
{
	int ret = fwrite( &aValue, 4, 1, aFP ); // length fixed to 4
	if ( ret != 1 )
		throw CFailureException("CEditableLevel::WriteInt: fwrite failed!");
}

EXPORT void CEditableLevel::WriteChunkHeader( TChunk aBlockID, int aLength, FILE* aFP )
{
	WriteInt( aBlockID, aFP );
	WriteInt( aLength, aFP );
}

EXPORT void CEditableLevel::TrimRandomCrates( std::vector< TRandomCrateInfo >& iCrates )
{
	for (int a=0;a<iCrates.size();a++)
	{
		if (iCrates[a].iAmount == 0)
			iCrates.erase( iCrates.begin() + a );
	}
}

void CEditableLevel::ModifyRandomCrateAmount(int aType1,int aType2, int aAdd, bool aDM )
{
	std::vector< TRandomCrateInfo >* crates=&iRandomCratesNormal;

	if ( aDM )
		crates = &iRandomCratesDM;

	for (int a=0;a < crates->size();a++)
		if (crates->at(a).iType1 == aType1 && 
			crates->at(a).iType2 == aType2)
		{
			crates->at(a).iAmount+=aAdd;
			if (crates->at(a).iAmount < 0)
				crates->at(a).iAmount = 0;
			return; 
		}
	if (aAdd>0)
	{
		crates->push_back( TRandomCrateInfo(aType1,aType2,aAdd) );
	}
}

