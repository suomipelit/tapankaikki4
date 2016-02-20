#include <math.h>
#include "EditorDrawer.h"
#include "EditorGraphics.h"
#include "editorapp.h"
#include "EditorSelection.h"
#include "EditorBrush.h"
#include "EditorClipBoardBlock.h"
#include "../common/CGraphicsBuffer.h"
#include "../common/CGraphicsDevice.h"
#include "../common/CEditableLevel.h"
#include "../common/error.h"
#include "../common/CFonts.h"
#include "../common/CSpotlight.h"
#include "../common/CSteam.h"
#include "../common/CMath.h"
#include "../common/CGameTypeData.h"
#include "../common/files.h"
#include "../common/IBulletType.h"
#include "../common/IEnemyType.h"
#include "../common/IWeaponType.h"
#include "../common/CLevel.h"


#define BlockBufferWidth 320
#define BlockBufferHeight 600
#define BlocksInRow (BlockBufferWidth/KBlockSpriteSize)
#define BlocksInCol (BlockBufferHeight/KBlockSpriteSize)
#define OUTBLOCK -1
#define MinZoom 1
#define MaxZoom 4

// Info texts

#define KInfoTexts 26

const char *InfoText[KInfoTexts]={
		  "F1  - this help",
		  "F2  - save level as",
		  "F3  - load level",
		  "F4  - make new level",
		  "F5  - toggle level map",
		  "F6  - edit general level variables",
		  "F7  - edit random crate data for normal game",
		  "F8  - edit random crate data for death match",
		  "F9  - load config (.ini) file",
		  "CTRL & S - save",
		  "CTRL & C - copy to clipboard",
		  "CTRL & V - paste clipboard",
		  "Q - place spot light",
		  "W - place steam",
		  "A - place normal game crate",
		  "Z - place deathmatch crate",
		  "E - place enemy",
		  "DEL - delete object",
		  "P - place player startpoint",
		  "CTRL - make new selection",
		  "ALT - remove from selection",
		  "SHIFT - add to selection",
		  "enter - edit heights/edit blocks",	
		  "space - block selection mode/level editing mode",	
		  "pageup/pagedown - change block type (walls/floor)",
		  "-/+ or mouse wheel - zoom"
          };

void CEditorDrawer::UpdateScrBlockDimensions()
{
	iScrBlockWidth = (int)ceil((float)iDrawBuffer->Width()/iScrBlockSize);
	iScrBlockHeight = (int)ceil((float)iDrawBuffer->Height()/iScrBlockSize);
}

CEditorDrawer::CEditorDrawer(CGraphicsDevice* aGD,CEditorGraphics* aGraphics,CEditableLevel* aLevel,CEditorSelection* aLevelSelection,CEditorBrush* aBrush)
{
	iBrush = aBrush;
	iLevelSelection = aLevelSelection;
	iGD = aGD;
	iGraphics = aGraphics;
	iLevel = aLevel;
	iDrawBuffer = new CGraphicsBuffer(iGD->Width(),iGD->Height());
	iScrBlockSize = KBlockSpriteSize * iZoom;
	UpdateScrBlockDimensions();
	iScrXOffs=0;
	iScrYOffs=0;
	iBlockScrXOffs=0;
	iBlockScrYOffs=0;
	iTypeData = new CGameTypeData;
	iMap=true;
}

CEditorDrawer::~CEditorDrawer(void)
{
	delete iTypeData;
	delete iDrawBuffer;
}

void CEditorDrawer::Resize(int aWidth,int aHeight)
{
	iDrawBuffer->Resize(aWidth,aHeight);
	UpdateScrBlockDimensions();
	UpdateCaption();
}

void CEditorDrawer::DrawRect(int aX1,int aY1,int aX2,int aY2,unsigned char aColor)
{
	int a;

	if (aX1>=0 && aX1<iDrawBuffer->Width())
		for (a=aY1;a<aY2;a++)
			if (a>=0 && a<iDrawBuffer->Height())	
				iDrawBuffer->PutPixel(aX1,a,aColor);
	if (aX2>=0 && aX2<iDrawBuffer->Width())
		for (a=aY1;a<aY2;a++)
			if (a>=0 && a<iDrawBuffer->Height())	
				iDrawBuffer->PutPixel(aX2,a,aColor);
	if (aY1>=0 && aY1<iDrawBuffer->Height())
		for (a=aX1;a<aX2;a++)
			if (a>=0 && a<iDrawBuffer->Width())	
				iDrawBuffer->PutPixel(a,aY1,aColor);
	if (aY2>=0 && aY2<iDrawBuffer->Height())
		for (a=aX1;a<=aX2;a++)
			if (a>=0 && a<iDrawBuffer->Width())	
				iDrawBuffer->PutPixel(a,aY2,aColor);
}

void CEditorDrawer::DrawLiveRect(int aX1,int aY1,int aX2,int aY2,unsigned char aColor, int aCounter, int aLineLength)
{
	int a;
	int div = aLineLength;
	int counter = aCounter;

	if (aX1>=0 && aX1<iDrawBuffer->Width())
		for (a=aY1;a<aY2;a++)
			if (a>=0 && a<iDrawBuffer->Height())	
				if ((a+counter)/div%2) iDrawBuffer->PutPixel(aX1,a,aColor);
	if (aX2>=0 && aX2<iDrawBuffer->Width())
		for (a=aY1;a<aY2;a++)
			if (a>=0 && a<iDrawBuffer->Height())	
				if ((a-counter)/div%2) iDrawBuffer->PutPixel(aX2,a,aColor);
	if (aY1>=0 && aY1<iDrawBuffer->Height())
		for (a=aX1;a<aX2;a++)
			if (a>=0 && a<iDrawBuffer->Width())	
				if ((a-counter)/div%2) iDrawBuffer->PutPixel(a,aY1,aColor);
	if (aY2>=0 && aY2<iDrawBuffer->Height())
		for (a=aX1;a<=aX2;a++)
			if (a>=0 && a<iDrawBuffer->Width())	
				if ((a+counter)/div%2) iDrawBuffer->PutPixel(a,aY2,aColor);
}

void CEditorDrawer::ToggleMap()
{
	if (iMap==false) iMap=true;
	else iMap=false;
}

void CEditorDrawer::ZoomPixel(int aX,int aY,unsigned char aColor,int aZoom)
{
	for (int a=0;a < aZoom;a++)
		for (int b=0;b < aZoom;b++)
			if (aX+b >= 0 && aX+b<iDrawBuffer->Width() &&
				aY+a >= 0 && aY+a<iDrawBuffer->Height())
				iDrawBuffer->PutPixel(aX+b,aY+a,aColor);
}

void CEditorDrawer::DrawMap()
{
	int X,Y;
	int Zoom=2;

	X = iDrawBuffer->Width()-iLevel->Width()*Zoom;
	Y = 0;

	for (int a=0;a < iLevel->Height();a++) 
		for (int b=0;b < iLevel->Width();b++)
			if (iLevel->LevelData(b,a).iType==EBlockTypeWall)
				ZoomPixel(b*Zoom+X,a*Zoom+Y,34,Zoom);
			else
				ZoomPixel(b*Zoom+X,a*Zoom+Y,0,Zoom);
	DrawRect(X+iScrXOffs*Zoom-1,Y+iScrYOffs*Zoom-1,X+iScrXOffs*Zoom+iScrBlockWidth*Zoom,Y+iScrYOffs*Zoom+iScrBlockHeight*Zoom,255);
}

void CEditorDrawer::DrawInfo()
{
	int boxWidth;
	int boxHeight;
	int yOffset;
	
	char text[255];
	
	strcpy(text,"Keys info (press esc to return)");

	boxWidth=iGraphics->Fonts()->Length(text)*2+10;
	boxHeight=(KInfoTexts+5)*FontHeight()+20;

	int x = (iDrawBuffer->Width()-boxWidth)/2;
	int y = (iDrawBuffer->Height()-boxHeight)/2;

	for (int a=y;a<y+boxHeight;a++)
		for (int b=x;b<x+boxWidth;b++)
			if (a >= 0 && a < iDrawBuffer->Height() && b >= 0 && b < iDrawBuffer->Width())
				iDrawBuffer->PutPixel(b,a,KDialogColor);

	DrawRect(x,y,x+boxWidth,y+boxHeight,KDialogBorderColor); // For nicer looks !

	Write(x+10,y+10,text);

	yOffset = y+10+2*FontHeight();

	for (int a=0;a < KInfoTexts;a++,yOffset+=FontHeight())
	{
		Write(x+20,yOffset,InfoText[a]);
	}
}

void CEditorDrawer::DrawGeneralData(int aCursorXPos,int aCursorYPos,bool aCommentMode)
{
	int boxWidth;
	int boxHeight;
	int yOffset;
	
	char text[255];
	
	strcpy(text,"Edit general level data (press esc to return)");

	boxWidth=iGraphics->Fonts()->Length(text)+10;
	boxHeight=(EEnemyAmount+5)*FontHeight()+20;

	int x = (iDrawBuffer->Width()-boxWidth)/2;
	int y = (iDrawBuffer->Height()-boxHeight)/2;

	for (int a=y;a<y+boxHeight;a++)
		for (int b=x;b<x+boxWidth;b++)
			if (a >= 0 && a < iDrawBuffer->Height() && b >= 0 && b < iDrawBuffer->Width())
				iDrawBuffer->PutPixel(b,a,KDialogColor);

	DrawRect(x,y,x+boxWidth,y+boxHeight,KDialogBorderColor); // For nicer looks !

	Write(x+10,y+10,text);

	yOffset = y+10+2*FontHeight();

	Write(x+10,yOffset+aCursorYPos*FontHeight(),"*");

	sprintf(text,"Level title: %s",iLevel->iGeneralLevelInfo.iComment);
	if (aCommentMode) strcat(text,"_");
	Write(x+20,yOffset,text);
	yOffset+=FontHeight();
	sprintf(text,"Timelimit (seconds): %d",iLevel->iGeneralLevelInfo.iTimeLimit);
	Write(x+20,yOffset,text);
	yOffset+=FontHeight();
	for (int a=0;a < EEnemyAmount;a++,yOffset+=FontHeight())
	{
		sprintf(text,"%s: %d",iTypeData->iEnemyType[a]->Name(),iLevel->iGeneralLevelInfo.iEnemies[a]);
		Write(x+20,yOffset,text);
	}
}

void CEditorDrawer::DrawRCData(int aCursorXPos,int aCursorYPos,int aMode)
{
	int boxWidth;
	int boxHeight;
	int yOffset;
	std::vector< CLevel::TRandomCrateInfo >* cratesPointer;

	char text[255];
	
	if (aMode==EEditNormalGameData)
		strcpy(text,"Edit normal game random crate data (press esc to return)");
	else
		strcpy(text,"Edit deathmatch game random crate data (press esc to return)");

	boxWidth=iGraphics->Fonts()->Length(text)+10;
	boxHeight=(KWeaponCrates+KBulletCrates+KMoneyCrates+5)*FontHeight()+20;

	int x = (iDrawBuffer->Width()-boxWidth)/2;
	int y = (iDrawBuffer->Height()-boxHeight)/2;

	for (int a=y;a<y+boxHeight;a++)
		for (int b=x;b<x+boxWidth;b++)
			if (a >= 0 && a < iDrawBuffer->Height() && b >= 0 && b < iDrawBuffer->Width())
				iDrawBuffer->PutPixel(b,a,KDialogColor);

	DrawRect(x,y,x+boxWidth,y+boxHeight,KDialogBorderColor); // For nicer looks !

	Write(x+10,y+10,text);

	yOffset = y+10+2*FontHeight();

	Write(x+10,yOffset+aCursorYPos*FontHeight(),"*");

	if (aMode==EEditNormalGameData)
		cratesPointer = &iLevel->iRandomCratesNormal;
	else
		cratesPointer = &iLevel->iRandomCratesDM;

	CLevel::TRandomCrateInfo RandomCrate(0,0,0);

	for (int a=0;a < KWeaponCrates;a++,yOffset+=FontHeight())
	{
		RandomCrate = iLevel->GetRandomCrateInfo(CLevel::TCrate(CLevel::EWeaponCrate),a,cratesPointer);
		sprintf(text,"%s: %d",iTypeData->iWeaponType[a+1]->Name(),RandomCrate.iAmount);
		Write(x+20,yOffset,text);

	}
	for (int a=0;a < KBulletCrates;a++,yOffset+=FontHeight())
	{
		RandomCrate = iLevel->GetRandomCrateInfo(CLevel::TCrate(CLevel::EBulletCrate),a,cratesPointer);
		sprintf(text,"%s: %d",iTypeData->iBulletType[a+1]->Name(),RandomCrate.iAmount);
		Write(x+20,yOffset,text);

	}
	RandomCrate = iLevel->GetRandomCrateInfo(CLevel::TCrate(CLevel::EHealthCrate),0,cratesPointer);
	sprintf(text,"Energy: %d",RandomCrate.iAmount);
	Write(x+20,yOffset,text);

	for (int a=0;a < KMoneyCrates; a++)
	{
		RandomCrate = iLevel->GetRandomCrateInfo(CLevel::TCrate(CLevel::EMoneyCrate),KMoneyAmounts[a],cratesPointer);
		sprintf(text,"Money(%d): %d",KMoneyAmounts[a],RandomCrate.iAmount);
		Write(x+20,yOffset+=FontHeight(),text);
	}

	RandomCrate = iLevel->GetRandomCrateInfo(CLevel::TCrate(CLevel::ETargetCrate),0,cratesPointer);
	sprintf(text,"Target: %d",RandomCrate.iAmount);
	Write(x+20,yOffset+=FontHeight(),text);
}

void CEditorDrawer::DrawNewLevelPrompt(char* aWidth,char* aHeight)
{
	int BoxWidth;
	const int BoxHeight = 60;

	char Text[255]="New level - input dimensions (press esc to cancel)";

	BoxWidth=iGraphics->Fonts()->Length(Text)+10;

	int X = (iDrawBuffer->Width()-BoxWidth)/2;
	int Y = (iDrawBuffer->Height()-BoxHeight)/2;

	for (int a=Y;a<Y+BoxHeight;a++)
		for (int b=X;b<X+BoxWidth;b++)
			if (a >= 0 && a < iDrawBuffer->Height() && b >= 0 && b < iDrawBuffer->Width())
				iDrawBuffer->PutPixel(b,a,KDialogColor);

	DrawRect(X,Y,X+BoxWidth,Y+BoxHeight,KDialogBorderColor); // For nicer looks !

	Write(X+10,Y+10,Text);
	strcpy(Text,"Width: ");
	strcat(Text,aWidth);
	Write(X+10,Y+10+FontHeight()*2,Text);
	strcpy(Text,"Height: ");
	strcat(Text,aHeight);
	Write(X+10,Y+10+FontHeight()*3,Text);
}

void CEditorDrawer::DrawBlock(int aX,int aY,int aType, int aNumber,int aSize,int aZoom)
{
	int a, b, xSize= aSize*BlocksInRow+BlockBufferWidth;
	unsigned char *iTexture=NULL;
	int ZBlockSize = KBlockSpriteSize * aZoom;
	int sx=0, ex=ZBlockSize+aSize*aZoom;
	int sy=0, ey=ZBlockSize+aSize*aZoom;
	int ScrOffs=0;
	int TextureOffs=0,YOffs=0;

	// Exception handling start
	if (aNumber > BlocksInRow*(BlockBufferHeight/KBlockSpriteSize)-1)
		aNumber = BlocksInRow*(BlockBufferHeight/KBlockSpriteSize)-1;
	if (aNumber < 0) aNumber = 0;
	// Exception handling end

	if (aX<0) 
		sx=abs(aX);
	else
		if (aX+ZBlockSize+aSize*aZoom > iDrawBuffer->Width())
			ex-=aX+ZBlockSize+aSize*aZoom-iDrawBuffer->Width();
	if (aY<0) 
		sy=abs(aY);
	else
		if (aY+ZBlockSize+aSize*aZoom > iDrawBuffer->Height())
			ey-=aY+ZBlockSize+aSize*aZoom-iDrawBuffer->Height();
	if (sx>=ex || sy>=ey) return;

	TextureOffs=((aNumber&(BlocksInRow-1)) *(KBlockSpriteSize+aSize)) +(aNumber/BlocksInRow*xSize*(KBlockSpriteSize + aSize));
	assert(aSize<KWallSprites);
	
	switch (aType)
	{
	case EBlockTypeFloor:
		iTexture=iGraphics->FloorBlocks()->Ptr();
		break;
	case EBlockTypeWall:
		iTexture=iGraphics->WallBlocks(aSize)->Ptr();
		break;
	default:
		error("texture type unknown!");
		break;
	}

	ScrOffs=(sy+aY)*iDrawBuffer->Width()+sx+aX;
	YOffs=TextureOffs;
	for (a=sy;a<ey;a++,ScrOffs+=iDrawBuffer->Width()-(ex-sx),YOffs=TextureOffs+a/aZoom*xSize)
		for (b=sx;b<ex;b++,ScrOffs++)
			iDrawBuffer->Ptr()[ScrOffs]= iTexture[b/aZoom+YOffs];
}

void CEditorDrawer::DrawLevelType(int aXOffs,int aYOffs,int aType)
{
	int a,b;
	
	for (a=0;a<iScrBlockHeight;a++)
		for (b=0;b<iScrBlockWidth;b++)
			if (iLevel->InLevel(aXOffs+b,aYOffs+a))
			{
				if (iLevel->LevelData(aXOffs+b,aYOffs+a).iType==aType)
					DrawBlock(b*iScrBlockSize,a*iScrBlockSize,aType,iLevel->LevelData(aXOffs+b,aYOffs+a).iNumber,0,iZoom);
			}
			else if (aType==OUTBLOCK)
				DrawBlock(b*iScrBlockSize,a*iScrBlockSize,iLevel->OutBlock().iType,iLevel->OutBlock().iNumber,0,iZoom);
}

int CEditorDrawer::BlocksXOffs()
{
	int XOffs=(iDrawBuffer->Width()-BlocksInRow*KBlockSpriteSize*iZoom)/2;
	if (XOffs<0) XOffs=0;
	return XOffs;
}

void CEditorDrawer::DrawClipBoard(int aX,int aY,std::vector<CEditorClipBoardBlock>* aBlocks)
{
	int x,y;
	for (int a=0;a < aBlocks->size();a++)
	{
		x = aBlocks->at(a).X()+aX;
		y = aBlocks->at(a).Y()+aY;
		if (x < iScrBlockWidth && y < iScrBlockHeight)
			DrawBlock(x*iScrBlockSize,y*iScrBlockSize,aBlocks->at(a).Type(),aBlocks->at(a).Number(),0,iZoom);
	}
}

void CEditorDrawer::DrawBlocks(int aType)
{
	int a,b;
	int XOffs=BlocksXOffs();

	memset(iDrawBuffer->Ptr(),42,iDrawBuffer->Width()*iDrawBuffer->Height());

	for (a=0;a<iScrBlockHeight && a<BlocksInCol;a++)
		for (b=0;b<iScrBlockWidth && b<BlocksInRow;b++)
			if (InBlocksBuffer(b+iBlockScrXOffs,a+iBlockScrYOffs))
				DrawBlock(b*iScrBlockSize+XOffs,a*iScrBlockSize,aType,b+iBlockScrXOffs+(a+iBlockScrYOffs)*BlocksInRow,0,iZoom);
}

void CEditorDrawer::DrawSelection(int a3D,int aXOffs, int aYOffs)
{
	int x,y,xofs,yofs;
	static int counter = 0 ;
	const int div = 5; //selection nauhan viivan pituus
	int block_x_ofs[KWallSprites];
	int block_y_ofs[KWallSprites];
	int HalfScrBlockWidth = iScrBlockWidth / 2;
	int HalfScrBlockHeight = iScrBlockHeight / 2;

    for (int b=0; b<KWallSprites; b++)
	{
		block_x_ofs[b] = HalfScrBlockWidth * b * iZoom * (-1) * a3D;
		block_y_ofs[b] = HalfScrBlockHeight * b * iZoom * (-1) * a3D;
	}

	counter++;
	counter%=div*2;

	for (y=aYOffs;y < iScrBlockHeight+aYOffs;y++)
	for (x=aXOffs;x < iScrBlockWidth+aXOffs;x++)
	if (iLevelSelection->Includes(x,y))
	{
		xofs=block_x_ofs[iLevel->LevelData(x,y).iHeight]+(x-aXOffs)*(iLevel->LevelData(x,y).iHeight*a3D*iZoom+iScrBlockSize);
		yofs=block_y_ofs[iLevel->LevelData(x,y).iHeight]+(y-aYOffs)*(iLevel->LevelData(x,y).iHeight*a3D*iZoom+iScrBlockSize);
	
//		if (x>=aXOffs && x<aXOffs+iScrBlockWidth && y>=aYOffs && y<aYOffs+iScrBlockHeight)
		DrawLiveRect(xofs,yofs,xofs+(KBlockSpriteSize+iLevel->LevelData(x,y).iHeight*a3D)*iZoom-1, yofs+(KBlockSpriteSize+iLevel->LevelData(x,y).iHeight*a3D)*iZoom-1, 255, counter, div);
		iAnimation = true;
	}
}

void CEditorDrawer::DrawBrush(int aX,int aY)
{
	DrawRect(iDrawBuffer->Width()-KBlockSpriteSize*2-2,iDrawBuffer->Height()-KBlockSpriteSize-2,iDrawBuffer->Width()-1,iDrawBuffer->Height()-1,0);
	DrawBlock(iDrawBuffer->Width()-KBlockSpriteSize*2-1,iDrawBuffer->Height()-KBlockSpriteSize-1,iBrush->LeftBlock()->iType,iBrush->LeftBlock()->iNumber,0,1);
	DrawBlock(iDrawBuffer->Width()-KBlockSpriteSize-1,iDrawBuffer->Height()-KBlockSpriteSize-1,iBrush->RightBlock()->iType,iBrush->RightBlock()->iNumber,0,1);
}


void CEditorDrawer::DrawLevel(int a3D,int aSelX,int aSelY)
{
	DrawLevelType(iScrXOffs,iScrYOffs,EBlockTypeFloor);
	if (! a3D)
	{
		DrawLevelType(iScrXOffs,iScrYOffs,EBlockTypeWall);
		DrawLevelType(iScrXOffs,iScrYOffs,OUTBLOCK);
		DrawObjects();
	} else
		Draw3DWalls(aSelX,aSelY);
	DrawSelection(a3D,iScrXOffs,iScrYOffs);
	if (iMap) DrawMap();
	DrawFileInfo();
}

void CEditorDrawer::DrawCircle(int aX,int aY,int aR,unsigned char aCol)
{
	int a,rx,ry;
	float ang;

	for (a=0;a<2*pi*aR;a++)
	{
		ang=(float)a/aR;
		rx=aX+(int)(aR*cos(ang));
		ry=aY+(int)(aR*sin(ang));
	
		if (rx>=0 && ry>=0 && rx<iDrawBuffer->Width() && ry<iDrawBuffer->Height()) 
		 iDrawBuffer->PutPixel(rx,ry,aCol);
	}
}

void CEditorDrawer::DrawSpot(int aX,int aY,int aSize)
{
	if (aX >= 0 && aX < iDrawBuffer->Width() &&
		aY >= 0 && aY < iDrawBuffer->Height())
		DrawCircle(aX,aY,(15+5*aSize)/2*iZoom,255);
}

void CEditorDrawer::DrawEnemy(int aX,int aY,int aType)
{
	if (aX >= 0 && aX < iDrawBuffer->Width() &&
		aY >= 0 && aY < iDrawBuffer->Height())
		DrawCircle(aX+KEnemyRealRad*iZoom,aY+KEnemyRealRad*iZoom,KEnemyRad*iZoom,(unsigned char)KEnemyColor);
	Write(aX,aY+KEnemyRealRad*iZoom-FontHeight(),iTypeData->iEnemyType[aType]->Name());

}

void CEditorDrawer::DrawSteam(int aX,int aY,int aAngle,int aSpeed)
{
	int a;
	for (a=0;a<6;a++)
		DrawCircle(aX+(int)(CMath::Static()->Cose(aAngle)*(a*(aSpeed*4)))*iZoom,aY+(int)(CMath::Static()->Sine(aAngle)*(a*(aSpeed*4)))*iZoom,(a+3)*iZoom,32);
}

void CEditorDrawer::DrawCrate(int aX,int aY,int aType1,int aType2,unsigned char aColor)
{
	int a,b,size=KWeaponCrateSize*iZoom,start=0;
	char texti[255];
	
	if (aX < 0 || aX >= iDrawBuffer->Width() || aY < 0 || aY >= iDrawBuffer->Height())
		return;

	if (aType1==CLevel::EWeaponCrate)
		strcpy(texti,iTypeData->iWeaponType[aType2+1]->Name());
	else
	if (aType1==CLevel::EBulletCrate)
	{
		sprintf(texti,"%s(%d)",iTypeData->iBulletType[aType2+1]->Name(),iTypeData->iBulletType[aType2+1]->AmountInCrate()/iTypeData->iBulletType[aType2+1]->Multiplier());
		size = KBulletCrateSize*iZoom;
		start = ((KWeaponCrateSize-KBulletCrateSize)/2)*iZoom;
	}
	else
	if (aType1==CLevel::EHealthCrate)
		strcpy(texti,"Energy");	
	else
	if (aType1==CLevel::EMoneyCrate)
		sprintf(texti,"Money(%d)",aType2);
	else
	if (aType1==CLevel::ETargetCrate)
		strcpy(texti,"Target");	

	for (a=aY+start;a<aY+size;a++)
		for (b=aX+start;b<aX+size;b++)
			if (a >= 0 && 
				a < iDrawBuffer->Height() && 
				b >= 0 && 
				b < iDrawBuffer->Width())
				iDrawBuffer->PutPixel(b,a,aColor);
	
	Write(aX,aY+size-FontHeight(),texti);
}

void CEditorDrawer::DrawObjects()
{
	CSpotLight* SpotPtr = iLevel->iSpotLights;
	CSteam* SteamPtr = iLevel->iSteams;
	int a;

	while (SpotPtr)
	{
		DrawSpot(int(SpotPtr->Location().X()*iZoom-iScrXOffs*iScrBlockSize),
				 int(SpotPtr->Location().Y()*iZoom-iScrYOffs*iScrBlockSize),
				 SpotPtr->Size());	
		SpotPtr = SpotPtr->Next();
	}

	while (SteamPtr)
	{
		DrawSteam(int(SteamPtr->X()*iZoom-iScrXOffs*iScrBlockSize),
			      int(SteamPtr->Y()*iZoom-iScrYOffs*iScrBlockSize),SteamPtr->Angle(),
				  SteamPtr->Speed());	
		SteamPtr = SteamPtr->Next();
	}

	for (a=0;a<iLevel->StartPositions();a++)
	{
		char text[255];
		sprintf(text,"PL %d",a+1);
		if (iLevel->StartPosition(a).X() >= iScrXOffs && iLevel->StartPosition(a).X() < iScrXOffs+iScrBlockWidth &&
			iLevel->StartPosition(a).Y() >= iScrYOffs && iLevel->StartPosition(a).Y() < iScrYOffs+iScrBlockHeight)
			Write((iLevel->StartPosition(a).X()-iScrXOffs)* iScrBlockSize + iScrBlockSize /2 - 8, (iLevel->StartPosition(a).Y()-iScrYOffs)* iScrBlockSize + iScrBlockSize /2 - 5 ,text);
	}

	for (a=0;a<iLevel->iPlacedCratesNormal.size();a++)
	{
		DrawCrate(iLevel->iPlacedCratesNormal[a].iX*iZoom-iScrXOffs*iScrBlockSize,
				  iLevel->iPlacedCratesNormal[a].iY*iZoom-iScrYOffs*iScrBlockSize,iLevel->iPlacedCratesNormal[a].iType1,
				  iLevel->iPlacedCratesNormal[a].iType2,16);
	}

	for (a=0;a<iLevel->iPlacedCratesDM.size();a++)
	{
		DrawCrate(iLevel->iPlacedCratesDM[a].iX*iZoom-iScrXOffs*iScrBlockSize,iLevel->iPlacedCratesDM[a].iY*iZoom-iScrYOffs*iScrBlockSize,iLevel->iPlacedCratesDM[a].iType1,iLevel->iPlacedCratesDM[a].iType2,16*9);
	}

	for (a=0;a<iLevel->iEnemy.size();a++)
	{
		DrawEnemy(iLevel->iEnemy[a].iX*iZoom-iScrXOffs*iScrBlockSize,
			iLevel->iEnemy[a].iY*iZoom-iScrYOffs*iScrBlockSize,
			iLevel->iEnemy[a].iType);
	}

}

void CEditorDrawer::ResetAnimationFlag()
{
	iAnimation = false;
}

bool CEditorDrawer::UpdateScreen()
{
	iGD->ShowBuf(iDrawBuffer);
	return iAnimation;
}

void CEditorDrawer::UpdateCaption()
{
	char text[255];
	sprintf(text,"Tapan Kaikki Bloodshed - Level Editor %s - Zoom: %d%%",KVersion,(int)iZoom*100);
	SDL_WM_SetCaption(text,"tk.ico");
}

void CEditorDrawer::SetZoom(int aZoom)
{
	int oScrBlockWidth=iScrBlockWidth;
	int oScrBlockHeight=iScrBlockHeight;
	iZoom = aZoom;

	if (iZoom<MinZoom) iZoom = MinZoom;
	if (iZoom>MaxZoom) iZoom = MaxZoom;

	iScrBlockSize = KBlockSpriteSize * iZoom;
	UpdateScrBlockDimensions();
	UpdateCaption();
	SetScrXOffs(iScrXOffs+(oScrBlockWidth-iScrBlockWidth)/2);
	SetScrYOffs(iScrYOffs+(oScrBlockHeight-iScrBlockHeight)/2);
	SetBlockScrXOffs(iBlockScrXOffs+(oScrBlockWidth-iScrBlockWidth)/2);
	SetBlockScrYOffs(iBlockScrYOffs+(oScrBlockHeight-iScrBlockHeight)/2);
}

void CEditorDrawer::ZoomIn()
{
	SetZoom(iZoom+1);
}

void CEditorDrawer::ZoomOut()
{
	SetZoom(iZoom-1);
}

void CEditorDrawer::SetScrXOffs(int aScrXOffs)
{
	iScrXOffs = aScrXOffs;
	if (iScrXOffs < iScrBlockWidth/(-2)) iScrXOffs=iScrBlockWidth/(-2); 
	if (iScrXOffs > iLevel->Width()-iScrBlockWidth/2) iScrXOffs=iLevel->Width()-iScrBlockWidth/2; 
}
void CEditorDrawer::SetScrYOffs(int aScrYOffs)
{
	iScrYOffs = aScrYOffs;
	if (iScrYOffs < iScrBlockHeight/(-2)) iScrYOffs=iScrBlockHeight/(-2); 
	if (iScrYOffs > iLevel->Height()-iScrBlockHeight/2) iScrYOffs=iLevel->Height()-iScrBlockHeight/2; 
}

void CEditorDrawer::SetBlockScrXOffs(int aScrXOffs)
{
	iBlockScrXOffs = aScrXOffs;
	if (iBlockScrXOffs > BlocksInRow-iScrBlockWidth) iBlockScrXOffs = BlocksInRow-iScrBlockWidth; 
	if (iBlockScrXOffs < 0) iBlockScrXOffs=0; 
}
void CEditorDrawer::SetBlockScrYOffs(int aScrYOffs)
{
	iBlockScrYOffs = aScrYOffs;
	if (iBlockScrYOffs > BlocksInCol-iScrBlockHeight) iBlockScrYOffs = BlocksInCol-iScrBlockHeight; 
	if (iBlockScrYOffs < 0) iBlockScrYOffs=0; 
}

void CEditorDrawer::Write(int aX,int aY,const char* aText, CFonts::TVertAlign aVAlign, CFonts::THorizAlign aHAlign)
{
	iGraphics->Fonts()->Write(aX,aY,aText, aVAlign, aHAlign,iDrawBuffer);
}

void CEditorDrawer::Write(int aX,int aY,int aNumber, CFonts::TVertAlign aVAlign, CFonts::THorizAlign aHAlign)
{
	char Text[255];
	sprintf(Text, "%d", aNumber);
	iGraphics->Fonts()->Write(aX,aY,Text, aVAlign, aHAlign,iDrawBuffer);
}

int CEditorDrawer::FontHeight()
{
	return iGraphics->Fonts()->Height()+3;
}

int CEditorDrawer::InBlocksBuffer(int aX,int aY)
{
	if (aX >= 0 && aX < BlocksInRow && aY >=0 && aY < BlocksInCol)
		return 1;
	return 0;
}

int CEditorDrawer::GetBlocksInRow()
{
	return BlocksInRow;
}

void CEditorDrawer::DrawFileInfo()
{
	char Text[FILENAME_MAX*2],LevelFileName[FILENAME_MAX];

	if (*iLevel->LevelFileName() != 0)
		strcpy(LevelFileName,iLevel->LevelFileName());
	else
		strcpy(LevelFileName,"Untitled");

	sprintf(Text,"%s (%dx%d)(v%d.0)",LevelFileName,iLevel->Width(),iLevel->Height(),iLevel->iVersion);

	Write(5,iDrawBuffer->Height()-FontHeight(),Text);
}

void CEditorDrawer::DrawMouseCoords(int aX,int aY)
{
	char text[255];
	sprintf(text,"X:%d, Y:%d",aX,aY);
	Write(iDrawBuffer->Width()-iGraphics->Fonts()->Length(text)-5,iDrawBuffer->Height()-FontHeight()-25,text);
}

// **********************************************************************
// * 3D Wall Shit ->
// **********************************************************************

void CEditorDrawer::DrawFace(int aX1,int aY1,int aX2,int aY2,int aX3,int aY3,int aX4,int aY4,unsigned char aColor)
{
	iDrawBuffer->DrawPolygon (aX1, aY1, aX3, aY3, aX4, aY4, aColor, 1);
	iDrawBuffer->DrawPolygon (aX1, aY1, aX2, aY2, aX4, aY4, aColor, 1);
}

int CEditorDrawer::NeedToDraw(int aX,int aY,int aXAdd,int aYAdd)
{
	if (iLevel->InLevel(aX+aXAdd,aY+aYAdd)){
	if ( iLevel->LevelData(aX+aXAdd,aY+aYAdd).iType == EBlockTypeWall && 
	 	 iLevel->LevelData(aX+aXAdd,aY+aYAdd).iHeight >= iLevel->LevelData(aX,aY).iHeight )
		 return 0;
	} else
	if ( iLevel->OutBlock().iType == EBlockTypeWall && 
	 	 iLevel->OutBlock().iHeight >= iLevel->LevelData(aX,aY).iHeight )
		 return 0;

	return 1;
}

void CEditorDrawer::Draw3DBlockRow(int aX1,int aX2,int aY,EDrawSect aSection,int aSelX,int aSelY)
{
	int b,mod,xofs1,yofs1,xofs2,yofs2,iSize;
    int block_x_ofs[KWallSprites];
	int block_y_ofs[KWallSprites];
	int HalfScrBlockWidth = iScrBlockWidth / 2;
	int HalfScrBlockHeight = iScrBlockHeight / 2;
	int LevelX,LevelY;
	unsigned char col1,col2;

    for (b=0; b<KWallSprites; b++)
	{
		block_x_ofs[b] = HalfScrBlockWidth * b * iZoom * (-1);
		block_y_ofs[b] = HalfScrBlockHeight * b * iZoom * (-1);
	}

	if (aX1<aX2) 
		mod = 1;
	else
		mod = -1;

	yofs1=aY * iScrBlockSize;
    LevelY = iScrYOffs+aY;

	for (b=aX1; b != aX2; b+=mod)
	{
		xofs1= b * iScrBlockSize;

		LevelX = iScrXOffs+b;	

		if (iLevel->InLevel(LevelX,LevelY))	
		{
			if (iLevel->LevelData(LevelX,LevelY).iType==EBlockTypeWall)
			{
				xofs2=block_x_ofs[iLevel->LevelData(LevelX,LevelY).iHeight]+b*(iLevel->LevelData(LevelX,LevelY).iHeight*iZoom+iScrBlockSize);
				yofs2=block_y_ofs[iLevel->LevelData(LevelX,LevelY).iHeight]+aY*(iLevel->LevelData(LevelX,LevelY).iHeight*iZoom+iScrBlockSize);
				iSize=iScrBlockSize+iLevel->LevelData(LevelX,LevelY).iHeight*iZoom;
				col1=iGraphics->WallColors()->Ptr()[iLevel->LevelData(LevelX,LevelY).iNumber+16*(iLevel->LevelData(LevelX,LevelY).iNumber/16)];
				col2=iGraphics->WallColors()->Ptr()[iLevel->LevelData(LevelX,LevelY).iNumber+16*(iLevel->LevelData(LevelX,LevelY).iNumber/16)+16];
				if (iLevel->LevelData(LevelX,LevelY).iHeight > 0)
				{
					switch(aSection) {							
					case UpLeft:
						if (NeedToDraw(LevelX,LevelY,0,1))
							DrawFace(xofs2-1, yofs2+iSize, xofs2+iSize, yofs2+iSize, xofs1-1, yofs1+iScrBlockSize, xofs1+iScrBlockSize,yofs1+iScrBlockSize,col2); 
						if (NeedToDraw(LevelX,LevelY,1,0))
							DrawFace(xofs2+iSize, yofs2+iSize, xofs1+iScrBlockSize,yofs1+iScrBlockSize, xofs2+iSize, yofs2-1, xofs1+iScrBlockSize, yofs1-1,col1); 
					break;
					case UpRight:
						if (NeedToDraw(LevelX,LevelY,0,1))
							DrawFace(xofs2-1, yofs2+iSize, xofs2+iSize, yofs2+iSize, xofs1, yofs1+iScrBlockSize, xofs1+iScrBlockSize,yofs1+iScrBlockSize,col2); 
						if (NeedToDraw(LevelX,LevelY,-1,0))
							DrawFace(xofs2, yofs2, xofs1,yofs1, xofs2-1, yofs2 + iSize, xofs1-1, yofs1 + iScrBlockSize,col2); 
					break;
					case DownLeft:
						if (NeedToDraw(LevelX,LevelY,0,-1))
							DrawFace(xofs2+iSize-1, yofs2, xofs1+iScrBlockSize,yofs1, xofs2, yofs2, xofs1-1, yofs1,col1); 
						if (NeedToDraw(LevelX,LevelY,1,0))
							DrawFace(xofs2+iSize, yofs2+iSize, xofs1+iScrBlockSize,yofs1+iScrBlockSize, xofs2+iSize, yofs2-1, xofs1+iScrBlockSize, yofs1-1,col1); 
					break;
					case DownRight:
						if (NeedToDraw(LevelX,LevelY,0,-1))
							DrawFace(xofs2+iSize-1, yofs2, xofs1+iScrBlockSize,yofs1, xofs2, yofs2, xofs1-1, yofs1,col1); 
						if (NeedToDraw(LevelX,LevelY,-1,0))
							DrawFace(xofs2, yofs2, xofs1,yofs1, xofs2-1, yofs2 + iSize, xofs1-1, yofs1 + iScrBlockSize,col2); 
					break;
					}
				}
				DrawBlock(xofs2, yofs2, iLevel->LevelData(LevelX,LevelY).iType, iLevel->LevelData(LevelX,LevelY).iNumber, iLevel->LevelData(LevelX,LevelY).iHeight,iZoom);
				if (aSelX == LevelX && aSelY == LevelY) 
					DrawRect(xofs2,yofs2,xofs2+(KBlockSpriteSize+iLevel->LevelData(LevelX,LevelY).iHeight)*iZoom-1,yofs2+(KBlockSpriteSize+iLevel->LevelData(LevelX,LevelY).iHeight)*iZoom-1,255);
			}
		} 
		else 
		{
			xofs2=block_x_ofs[iLevel->OutBlock().iHeight]+b*(iLevel->OutBlock().iHeight*iZoom+iScrBlockSize);
			yofs2=block_y_ofs[iLevel->OutBlock().iHeight]+aY*(iLevel->OutBlock().iHeight*iZoom+iScrBlockSize);
			DrawBlock(xofs2, yofs2, iLevel->OutBlock().iType, iLevel->OutBlock().iNumber, iLevel->OutBlock().iHeight,iZoom);
		}   
	}
}


void CEditorDrawer::Draw3DWalls(int aSelX,int aSelY) 
{
	int x_start, x_end, y_start, y_end;
    int a;
	int HalfScrBlockWidth = iScrBlockWidth / 2;
	int HalfScrBlockHeight = iScrBlockHeight / 2;

    y_start=0;
    y_end= HalfScrBlockHeight;
	
	for (a=y_start; a<y_end ; a ++)
	{ 
		x_start=0;
		x_end= HalfScrBlockWidth;
		Draw3DBlockRow(x_start ,x_end, a, UpLeft, aSelX, aSelY);
		x_start= iScrBlockWidth - 1;
		x_end= HalfScrBlockWidth - 1;
 		Draw3DBlockRow(x_start ,x_end , a, UpRight, aSelX, aSelY);
	}
		y_start= iScrBlockHeight - 1;

	for (a=y_start; a >= y_end ; a--) 
	{	
		x_start = 0;
		x_end = HalfScrBlockWidth;
		Draw3DBlockRow(x_start ,x_end, a, DownLeft, aSelX, aSelY);
		x_start = iScrBlockWidth -1;
		x_end= HalfScrBlockWidth - 1;
		Draw3DBlockRow(x_start ,x_end , a, DownRight, aSelX, aSelY);

	}        
}		
