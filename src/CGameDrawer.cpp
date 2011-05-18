#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common/CMath.h"
#include "common/CLevel.h"

#include "CGameDrawer.h"
#include "CCharacter.h"
#include "CIngameData.h"

CGameDrawer::CGameDrawer(CColorTable const* aColorTable):iColorTable(aColorTable)
{
	ASSERT(aColorTable);
}

// Calculate clipping rect
inline CRect<int>CGameDrawer::ClipRect(int aX,int aY, int aBlockXSize,int aBlockYSize,CGraphicsBuffer* aDrawBuffer,const CRect<int>& aDrawRect)
{	
	CRect<int>clipRect(0,0,aBlockXSize,aBlockYSize);
	
	// Check X Limits
	if (aX+aBlockXSize>aDrawRect.iRight) 
		clipRect.iRight=aBlockXSize-(aX+aBlockXSize-aDrawRect.iRight);
	if (aX<aDrawRect.iLeft)
		clipRect.iLeft=aDrawRect.iLeft-aX;

	// Check Y Limits
	if (aY+aBlockYSize>aDrawRect.iBottom) 
		clipRect.iBottom=aBlockYSize-(aY+aBlockYSize-aDrawRect.iBottom);
	if (aY<aDrawRect.iTop)
		clipRect.iTop=aDrawRect.iTop-aY;

	return clipRect;
}

void CGameDrawer::DrawBlock(int aX,
							int aY,
							int aType,
							int aNumber,
							int aBlockHeight,
							CIngameData* aIngameData,
							CGraphicsBuffer* aDrawBuffer,
							const CRect<int>& aDrawRect) 
{
	int a, offset, xSize=aBlockHeight*16+320;
	const unsigned char *texture=NULL;
	unsigned char *gofs;
	int blockSpriteSize=KBlockSpriteSize+aBlockHeight;
	CRect<int>clipRect;

	// Check that aBlockHeight isn't>KWallSprites
	ASSERT(aBlockHeight<KWallSprites);

	clipRect=ClipRect(aX,aY,blockSpriteSize,blockSpriteSize,aDrawBuffer,aDrawRect);
	if (!clipRect.Valid()) return;
		
	// Calculate sprite offset
	offset=((aNumber&15) *blockSpriteSize) +(aNumber/16*xSize*blockSpriteSize)+clipRect.iTop*xSize+clipRect.iLeft;

	// now add texture
	switch (aType)
	{
	case EBlockTypeFloor:
		ASSERT(texture=aIngameData->GameSprite(EGameSpriteFloor)->Ptr());
		break;
	case EBlockTypeWall:
		ASSERT(texture=aIngameData->GameSprite(TGameSprite(EGameSpriteWalls+aBlockHeight))->Ptr());
		break;
	default:
		error("texture type unknown!");
		break;
	}
		
	texture+=offset;
	
	// Calculate target buffet offset
	gofs=aDrawBuffer->Ptr()+
		 (aY+clipRect.iTop)*aDrawBuffer->Width()+
		 aX+clipRect.iLeft;
	
	for (a=clipRect.iTop; a<clipRect.iBottom; a++) 
	{
		memcpy(gofs, texture, clipRect.Width());
		texture+=xSize;
		gofs+=aDrawBuffer->Width();
	}
}

void CGameDrawer::DrawDarkBlock(int aX,int aY,int aType,int aNumber, unsigned char *aTarget,CIngameData* aIngameData,CGraphicsBuffer* aDrawBuffer,const CRect<int>& aDrawRect) 
{
	int a, offset;
	unsigned char *gofs;
	const unsigned char *spr;
    CRect<int>clipRect;

	clipRect=ClipRect(aX,aY,KBlockSpriteSize,KBlockSpriteSize,aDrawBuffer,aDrawRect);
	if (!clipRect.Valid()) return;

	offset=((aNumber&15) *KBlockSpriteSize) +(aNumber/16*320*KBlockSpriteSize)+clipRect.iTop*320+clipRect.iLeft;
	gofs=aTarget +
		(clipRect.iTop+aY)*aDrawBuffer->Width()+
		 aX+clipRect.iLeft;

	switch (aType)
	{
		case EBlockTypeFloor:spr=aIngameData->GameSprite(EGameSpriteFloor2)->Ptr();break;
		case EBlockTypeWall:spr=aIngameData->GameSprite(EGameSpriteWall2)->Ptr();break;
		default: error("CGameDrawer::DrawDarkBlock: Invalid sprite.");
	}

	for (a=clipRect.iTop; a<clipRect.iBottom; a++) 
	{
		memcpy(gofs,spr+offset, clipRect.Width());
		offset+=320;
		gofs+=aDrawBuffer->Width();
	}
}

//Mitä tekee? answer: draws human's shadows
void CGameDrawer::DrawHumanShadow(int aX,int aY,int aNumber,const unsigned char* aShadowSprite,CGraphicsBuffer* aDrawBuffer,const CRect<int>& aDrawRect) 
{
	int a, b;
	const unsigned char *s;
	unsigned char *vb;
	unsigned char colo;
	CRect<int>clipRect;

	clipRect=ClipRect(aX,aY,KBlockSpriteSize,KBlockSpriteSize,aDrawBuffer,aDrawRect);
	if (!clipRect.Valid()) return;
	
	//s=iGameData->Sprite(ESpriteShadow)->Ptr()+aNumber*KBlockSpriteSize+clipRect.iLeft+320*clipRect.iTop;
	s=aShadowSprite+aNumber*KBlockSpriteSize+clipRect.iLeft+320*clipRect.iTop;

	vb=aDrawBuffer->Ptr()+((aY+clipRect.iTop)*aDrawBuffer->Width()+aX)+clipRect.iLeft;
	
	for (a=0;a<clipRect.Height(); a++, vb+=aDrawBuffer->Width(), s+=320) 
		for (b=0; b<clipRect.Width(); b++) 
		{
			colo=s[b];
			if (colo!=0) 
				vb[b]=iColorTable->ShadowTable[vb[b]][colo];
		}
}

void CGameDrawer::DrawWallShadow(int aX,int aY,int aNumber,int aSize, CGraphicsBuffer* aDrawBuffer,const CRect<int>& aDrawRect) 
{
	int a, b;
	int lum=6;
    unsigned char *vb;
	CRect<int>clipRect;

	clipRect =ClipRect(aX,aY,aSize,aSize,aDrawBuffer,aDrawRect);
	if (!clipRect.Valid()) return;
	
	vb=aDrawBuffer->Ptr()+((aY+clipRect.iTop)*aDrawBuffer->Width()+aX);
	
	switch (aNumber)
	{
	case 1: // Laatikko
		for (a=clipRect.iTop; a<clipRect.iBottom; a++, vb+=aDrawBuffer->Width()) 
			for (b=clipRect.iLeft; b<clipRect.iRight; b++) 
				vb[b]=iColorTable->ShadowTable[vb[b]][lum];
			break;
	case 2: // Oikea alakolmio tummennettu
		for (a=clipRect.iTop; a<clipRect.iBottom; a++, vb+=aDrawBuffer->Width()) 
			for (b=max(clipRect.iLeft,aSize-a); b<clipRect.iRight; b++) 
				vb[b]=iColorTable->ShadowTable[vb[b]][lum];
				/*			for (b=aSize-a; b<clipRect.iRight; b++) 
				if (b>=clipRect.iLeft) vb[b]=iColorTable->ShadowTable[vb[b]][lum];
			*/
			break;	
    case 3: // Vasen yläkolmio tummennettu
		for (a=clipRect.iTop; a<clipRect.iBottom; a++, vb+=aDrawBuffer->Width()) 
			for (b=clipRect.iLeft; b<aSize-a && clipRect.iLeft<aSize-a && b<clipRect.iRight; b++) 
				vb[b]=iColorTable->ShadowTable[vb[b]][lum];
			break;
			
    case 4: // Vasen alakulma tummennettu
		for (a=clipRect.iTop; a<clipRect.iBottom; a++, vb+=aDrawBuffer->Width()) 
			for (b=clipRect.iLeft+a; b<clipRect.iRight && clipRect.iLeft+a<clipRect.iRight; b++) 
				vb[b]=iColorTable->ShadowTable[vb[b]][lum];
			break;
			
    case 5: // 
		for (a=clipRect.iTop; a<clipRect.iBottom; a++, vb+=aDrawBuffer->Width()) 
			for (b=clipRect.iLeft; b<clipRect.iRight+a; b++) 
				vb[b]=iColorTable->ShadowTable[vb[b]][lum];
			break;
	};
}

//28 x 28
void CGameDrawer::DrawRambo(CCoord<int> aPos,int aAngle,int aNumber,int aColor,const CGraphicsBuffer* aSprite,CGraphicsBuffer* aDrawBuffer,const CRect<int>& aDrawRect) 
{
	//const int KBlockSpriteSize=KCharacterSpriteSize;
	
	int a, b, c, d, dbufStartOffset;
	int colo;
	CRect<int>clipRect;
	const unsigned char* sprOfs;
	int sprno;
    
	clipRect =ClipRect(aPos.X(),aPos.Y(),KCharacterSpriteSize,KCharacterSpriteSize,aDrawBuffer,aDrawRect);
	if (!clipRect.Valid()) return;
	
	dbufStartOffset=(aPos.Y()+clipRect.iTop)*aDrawBuffer->Width()+aPos.X();

	sprno = (KPlayerSpriteAmount-aAngle)+KPlayerSpriteAmount/4; // +90 degrees
	while (sprno>=KPlayerSpriteAmount) sprno-=KPlayerSpriteAmount;
	while (sprno<0) sprno+=KPlayerSpriteAmount;

	sprOfs= aSprite->Ptr()+
			((1+aNumber*(KCharacterSpriteSize+1)+clipRect.iTop) * aSprite->Width()) +(sprno*29) +1;

	for (a=0, c=0, d=0; a<clipRect.Height(); a++, c+=aDrawBuffer->Width(), d+=29*KPlayerSpriteAmount) 
		for (b=clipRect.iLeft; b<clipRect.iRight; b++) 
		{
			colo = sprOfs[d+b];
			if (colo)
			{
				// Check if this is replaceable color
				if (colo>=80 && colo<=80+15) // if so
					(*aDrawBuffer)[dbufStartOffset+c+b]=(unsigned char) (aColor+(colo-80));
				else // otherwise
					(*aDrawBuffer)[dbufStartOffset+c+b]=(unsigned char) colo;
			}
		}
}

void CGameDrawer::DrawSprite(int aX,int aY,int aXS,int aYS, const CGraphicsBuffer* aSprite,int aOffset,CGraphicsBuffer* aDrawBuffer,const CRect<int>& aDrawRect) 
{
	int a, b;
	const unsigned char *src;
	unsigned char *dest;
	unsigned char colo;
	CRect<int>clipRect;

	clipRect=ClipRect(aX,aY,aXS,aYS,aDrawBuffer,aDrawRect);
	if (!clipRect.Valid()) return;

	src=aSprite->Ptr()+aOffset+clipRect.iLeft+clipRect.iTop*aSprite->Width();
	dest=aDrawBuffer->Ptr()+(aY+clipRect.iTop)*aDrawBuffer->Width()+aX+clipRect.iLeft;
	
	//TODO: Check perform
	for (a=0; a<clipRect.Height(); a++, dest+=aDrawBuffer->Width(), src+=aSprite->Width()-(clipRect.Width()))
		for (b=0; b<clipRect.Width(); b++)
		{
			colo=*(src++);
			if (colo!=0) 
				*(dest+b)=colo;
		}
}

void CGameDrawer::DrawTransSprite(int aX,int aY,int aXS,int aYS, const CGraphicsBuffer* aSprite,int aOffset,CGraphicsBuffer* aDrawBuffer,const CRect<int>& aDrawRect) 
{
	int a, b;
	const unsigned char *src;
	unsigned char *dest;
	unsigned char colo;
	int w;
	CRect<int>clipRect;

	clipRect=ClipRect(aX,aY,aXS,aYS,aDrawBuffer,aDrawRect);
	if (!clipRect.Valid()) return;

	w = clipRect.Width();

	src=aSprite->Ptr()+aOffset+clipRect.iLeft+clipRect.iTop*aSprite->Width();
	dest=aDrawBuffer->Ptr()+(aY+clipRect.iTop)*aDrawBuffer->Width()+aX+clipRect.iLeft;

	for (a=0; a<clipRect.Height(); a++, dest+=aDrawBuffer->Width()-w, src+=aSprite->Width()-(clipRect.Width())) 
		for (b=0; b<w; b++,dest++) 
		{
			colo=*(src++);
			if (colo!=0) 
				*(dest)=iColorTable->TransparencyTable[colo][ *dest ];
		}
}

void CGameDrawer::DrawRocket(int aX,int aY,int aAngle,int aLength,unsigned char aRocketColor,CGraphicsBuffer* aDrawBuffer,const CRect<int>& aDrawRect)
{
	int a;
//	int limit=aDrawBuffer->Width();
	CCoord<float> ofs;
	    	
    for (a=0;a<aLength;a++)
	{
		aDrawBuffer->PutPixelClip((int)(aX+ofs.X()),(int)(aY+1+ofs.Y()),aRocketColor,aDrawRect);
		aDrawBuffer->PutPixelClip((int)(aX+ofs.X()),(int)(aY  +ofs.Y()),aRocketColor,aDrawRect);
		aDrawBuffer->PutPixelClip((int)(aX+ofs.X()+1),(int)(aY+1+ofs.Y()),aRocketColor,aDrawRect);
		aDrawBuffer->PutPixelClip((int)(aX+ofs.X()+1),(int)(aY  +ofs.Y()),aRocketColor,aDrawRect);

		ofs = ofs.AddMovement(aAngle,1);
    }
}

void CGameDrawer::DrawLight(enum TLightTable aLightTable,const CCoord<int>& aPos,int aLightSize,int aAdd, CGraphicsBuffer* aDrawBuffer,const CRect<int>& aDrawRect)
{
	ASSERT(aLightSize>=0);
	ASSERT(aLightSize<KLightAmount);
	CLightTable const* lightTable=0;
	CLightMap const& lightMap=iColorTable->LightMap(aLightSize);
	const int xs=lightMap.Size(), ys=lightMap.Size();

	int ycount=0, xcount=0, sum=0, offset=0;
	unsigned char *dbuf=0;
	CRect<int>clipRect;

	CCoord<int> pos = aPos.Sub(xs/2,ys/2);

	clipRect = ClipRect(pos.X(),pos.Y(),xs,ys,aDrawBuffer,aDrawRect);
	if (!clipRect.Valid()) return;

	switch (aLightTable)
	{
	case ELightTableNormal:
		lightTable=&iColorTable->NormalLight();
		break;
	case ELightTableExplosion:
		lightTable=&iColorTable->ExplosionLight();
		break;
	case ELightTableYellow:
		lightTable=&iColorTable->YellowLight();
		break;
	case ELightTableRed:
		lightTable=&iColorTable->RedLight();
		break;
	default: 
		error("CGameDrawer::DrawLight: No light table somehow!");
	}
		
	offset=xs*clipRect.iTop+clipRect.iLeft;
	dbuf=(pos.Y()+clipRect.iTop)*aDrawBuffer->Width()+pos.X()+aDrawBuffer->Ptr()+clipRect.iLeft;
	
	for (ycount=clipRect.iTop;ycount<clipRect.iBottom; ycount++, dbuf+=aDrawBuffer->Width()-clipRect.Width(), offset+=xs-clipRect.Width()) 
		for (xcount=clipRect.iLeft; xcount<clipRect.iRight; xcount++,offset++,dbuf++) 
		{
			sum = lightMap.iData[offset];
			if ( sum ) 
			{			
				sum += aAdd;
				if (sum<0) 
					sum=0; 
				else 
					if (sum>15) 
						sum=15;
					
				*dbuf=lightTable->iData[*dbuf][sum];
			}
		}
}
void CGameDrawer::DrawMapPixel(int aOffset,int aSize,int aColor, CGraphicsBuffer* aDrawBuffer)
{
	int a,b;
	
	for (a=0;a<aSize;a++)    
		for (b=0;b<aSize;b++)
                        if (aColor>=0) 
                                (*aDrawBuffer)[aOffset+a*aDrawBuffer->Width()+b] =
                                        (unsigned char)aColor;
                        else 
                                (*aDrawBuffer)[aOffset+a*aDrawBuffer->Width()+b] =
                                        iColorTable->ShadowTable[(*aDrawBuffer)[aOffset+a*aDrawBuffer->Width()+b]][15];
}



int CGameDrawer::CalcX(CCoord<int>& aP1,CCoord<int>& aP2,int y)
{
	return (aP1.X()+(y-aP1.Y())*(aP2.X()-aP1.X())/(aP2.Y()-aP1.Y()));
}

int CGameDrawer::CalcY(CCoord<int>& aP1,CCoord<int>& aP2,int x)
{
	return (aP1.Y()+(x-aP1.X())*(aP2.Y()-aP1.Y())/(aP2.X()-aP1.X()));
}

#define swap(a,b,c){ c=a;a=b;b=c;}
void CGameDrawer::DrawPolygon(CCoord<int>& aP1,CCoord<int>& aP2,CCoord<int>& aP3,int col, CGraphicsBuffer* aDarkBuffer,CGraphicsBuffer* aDrawBuffer,const CRect<int>& aDrawRect)
{
	CCoord<int> *p1=&aP1,
				*p2=&aP2,
				*p3=&aP3,
				*tmp=NULL;
	CCoord<int> p4,p5;

	//ASSERT(col>=0||aDarkBuffer);
	
	// Put these polygons in X order
	if (p1->X()>p2->X()) swap(p1,p2,tmp);
	if (p1->X()>p3->X()) swap(p1,p3,tmp);
	if (p2->X()>p3->X()) swap(p2,p3,tmp);

	// Check if not flat triangle
	if (p1->X()!=p3->X())
	{
		// Check if 2 polygons are above drawrect limit
		if (p2->X()<aDrawRect.iLeft)
		{
			if (p3->X()<=aDrawRect.iLeft) return;
			// Set X to the clipping of iTop
			p2->SetY(CalcY(*p3,*p2,aDrawRect.iLeft));
			p2->SetX(aDrawRect.iLeft);
			p1->SetY(CalcY(*p3,*p1,aDrawRect.iLeft));
			p1->SetX(aDrawRect.iLeft);
		}
		else 
		// Check if 1 polygon is left of drawrect limit
		if (p1->X()<aDrawRect.iLeft)
		{
			if (p3->X()<=aDrawRect.iLeft) return;
			// triangle must divided into two
			p4.SetY(CalcY(*p3,*p1,aDrawRect.iLeft));
			p4.SetX(aDrawRect.iLeft);
			p5.SetY(CalcY(*p1,*p2,aDrawRect.iLeft));
			p5.SetX(aDrawRect.iLeft);

			if (p1->X()==p4.X()&&p1->Y()==p4.Y()) return;
			if (p1->X()==p5.X()&&p1->Y()==p5.Y()) return;

			DrawPolygon(*p3,p4,*p2,col,aDarkBuffer,aDrawBuffer,aDrawRect);
			DrawPolygon(p4,p5,*p2,col,aDarkBuffer,aDrawBuffer,aDrawRect);
			return;
		}
		
		// Check if 2 polygons are below drawrect limit
		if (p2->X()>=aDrawRect.iRight)
		{
			if (p1->X()>=aDrawRect.iRight-1) return;
			// Set X to the clipping of iTop
			p2->SetY(CalcY(*p1,*p2,aDrawRect.iRight-1));
			p2->SetX(aDrawRect.iRight-1);
			p3->SetY(CalcY(*p3,*p1,aDrawRect.iRight-1));
			p3->SetX(aDrawRect.iRight-1);
		}
		else 
		// Check if 1 polygon is above drawrect limit
		if (p3->X()>=aDrawRect.iRight)
		{
			if (p1->X()>=aDrawRect.iRight-1) return;
			// triangle must divided into two
			p4.SetY(CalcY(*p3,*p1,aDrawRect.iRight-1));
			p4.SetX(aDrawRect.iRight-1);
			p5.SetY(CalcY(*p3,*p2,aDrawRect.iRight-1));
			p5.SetX(aDrawRect.iRight-1);

			if (p3->X()==p4.X()&&p3->Y()==p4.Y()) return;
			if (p3->X()==p5.X()&&p3->Y()==p5.Y()) return;

			DrawPolygon(*p1,p4,*p2,col,aDarkBuffer,aDrawBuffer,aDrawRect);
			DrawPolygon(p4,p5,*p2,col,aDarkBuffer,aDrawBuffer,aDrawRect);
			return;
		}
	}
	else return;

	// Put these polygons in Y order
	if (p1->Y()>p2->Y()) swap(p1,p2,tmp);
	if (p1->Y()>p3->Y()) swap(p1,p3,tmp);
	if (p2->Y()>p3->Y()) swap(p2,p3,tmp);

	// Check if not flat triangle
	if (p1->Y()!=p3->Y())
	{
		// Check if 2 polygons are above drawrect limit
		if (p2->Y()<aDrawRect.iTop)
		{
			if (p3->Y()<=aDrawRect.iTop) return;
			// Set X to the clipping of iTop
			p1->SetX(CalcX(*p1,*p3,aDrawRect.iTop));
			p1->SetY(aDrawRect.iTop);
			p2->SetX(CalcX(*p2,*p3,aDrawRect.iTop));
			p2->SetY(aDrawRect.iTop);
		}
		else 
		// Check if 1 polygon is above drawrect limit
		if (p1->Y()<aDrawRect.iTop)
		{
			if (p3->Y()<=aDrawRect.iTop) return;
			// triangle must divided into two
			p4.SetX(CalcX(*p1,*p3,aDrawRect.iTop));
			p4.SetY(aDrawRect.iTop);
			p5.SetX(CalcX(*p1,*p2,aDrawRect.iTop));
			p5.SetY(aDrawRect.iTop);

			if (p1->X()==p4.X()&&p1->Y()==p4.Y()) return;
			if (p1->X()==p5.X()&&p1->Y()==p5.Y()) return;

			DrawPolygon(*p3,p4,*p2,col,aDarkBuffer,aDrawBuffer,aDrawRect);
			DrawPolygon(p4,p5,*p2,col,aDarkBuffer,aDrawBuffer,aDrawRect);
			return;
		}

		// Check if 2 polygons are below drawrect limit
		if (p2->Y()>=aDrawRect.iBottom)
		{
			if (p1->Y()>=aDrawRect.iBottom-1) return;
			// Set Y to the clipping of iBottom
			p2->SetX(CalcX(*p2,*p1,aDrawRect.iBottom-1));
			p2->SetY(aDrawRect.iBottom-1);
			p3->SetX(CalcX(*p3,*p1,aDrawRect.iBottom-1));
			p3->SetY(aDrawRect.iBottom-1);
		}
		else 
		// Check if 1 polygon is above drawrect limit
		if (p3->Y()>=aDrawRect.iBottom)
		{
			if (p1->Y()>=aDrawRect.iBottom-1) return;
			// triangle must divided into two
			p4.SetX(CalcX(*p3,*p1,aDrawRect.iBottom-1));
			p4.SetY(aDrawRect.iBottom-1);
			p5.SetX(CalcX(*p3,*p2,aDrawRect.iBottom-1));
			p5.SetY(aDrawRect.iBottom-1);

			if (p3->X()==p4.X()&&p3->Y()==p4.Y()) return;
			if (p3->X()==p5.X()&&p3->Y()==p5.Y()) return;

			DrawPolygon(*p1,p4,*p2,col,aDarkBuffer,aDrawBuffer,aDrawRect);
			DrawPolygon(p4,p5,*p2,col,aDarkBuffer,aDrawBuffer,aDrawRect);
			return;
		}
	}
	else return;
	
	int h1,h2,k1,k2;
    aDrawRect.FixSmall(*p1);
	aDrawRect.FixSmall(*p2);
	aDrawRect.FixSmall(*p3);

    ASSERT(aDrawRect.Inside(*p1));
	ASSERT(aDrawRect.Inside(*p2));
	ASSERT(aDrawRect.Inside(*p3));

	h1=p2->Y()-p1->Y();
	h2=p3->Y()-p1->Y();

	k2=((p3->X()-p1->X())<<10)/h2;

	if (h1)
	{
		k1=((p2->X()-p1->X())<<10)/h1;
		DrawPolyTri(k1,k2,p1->X(),p1->Y(),h1,col, aDarkBuffer, aDrawBuffer);
	}
	h2-=h1;
	if (h2)
	{
		k1=((p2->X()-p3->X())<<10)/h2;
		DrawPolyTri(k1,-k2,p3->X(),p3->Y(),-h2,col, aDarkBuffer, aDrawBuffer);
	}
}

// Relative draw
void CGameDrawer::DrawPolygon(int p1x,int p1y,int p2x,int p2y,int p3x,int p3y,int col, CGraphicsBuffer* aDarkBuffer,CGraphicsBuffer* aDrawBuffer,const CRect<int>& aDrawRect)
{
	CCoord<int> p1(p1x+aDrawRect.iLeft,p1y+aDrawRect.iTop),
				p2(p2x+aDrawRect.iLeft,p2y+aDrawRect.iTop),
				p3(p3x+aDrawRect.iLeft,p3y+aDrawRect.iTop);
//    ASSERT(col>=0);
	if (col==-1&&aDarkBuffer==NULL) 
		return;

	DrawPolygon(p1,p2,p3,col,aDarkBuffer,aDrawBuffer,aDrawRect);
}

#undef swap
#define swap(a,b) { a+=b;b=a-b;a-=b; }
inline void CGameDrawer::DrawPolyTri(int k1,int k2,int x,int y,int aHeight, int col, CGraphicsBuffer* aDarkBuffer, CGraphicsBuffer* aDrawBuffer)
{
	int adder=aDrawBuffer->Width(),row,dx,dx2;
	unsigned char *yg;
	int ofs=adder*y;

	if (aHeight<0)
	{
		adder=-adder;
		aHeight=-aHeight;
	}

	yg=aDrawBuffer->Ptr() + ofs;//y ofs in screen
	if (k1>k2) swap(k1,k2);
	//dx =left side, dx2 =right side
	dx2=dx=x<<10;
	
	if (col<0)
	{
		unsigned char* yg2=aDarkBuffer->Ptr() + ofs;//y ofs in screen
		for (row=0;row<=aHeight;row++)
		{
			memcpy(yg+(dx>>10),yg2+(dx>>10),(dx2>>10)-(dx>>10)+1);
			yg+=adder;
			yg2+=adder;
			dx+=k1;
			dx2+=k2;
		}
	}
	else
	{
		for (row=0;row<=aHeight;row++)
		{
			memset(yg+(dx>>10),col,(dx2>>10)-(dx>>10)+1);
			yg+=adder;
			dx+=k1;
			dx2+=k2;
		}
	}
}	  
