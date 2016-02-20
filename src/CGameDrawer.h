#ifndef __CGAMEDRAWER_H
#define __CGAMEDRAWER_H

#include "common/Types.h"
#include "common/CRect.h"
#include "common/CColorTable.h"

class CGraphicsBuffer;
class CLevel;
class CBodyPart;
class CBullet;
class CCrate;
class CEffect;
class CSpotLight;
class CIngameData;

class CGameDrawer
{
public:
	CGameDrawer(CColorTable const* aColorTable);

	void DrawLight(enum TLightTable aLightTable,const CCoord<int>& aPos,int aNumber,int aAdd,CGraphicsBuffer* aDrawBuffer,const CRect<int>& aDrawRect);
	inline CRect<int> ClipRect(int aX,int aY, int aBlockXSize,int aBlockYSize,CGraphicsBuffer* aDrawBuffer,const CRect<int>& aDrawRect);
	void DrawBlock(int aX, int aY, int aType,int aNumber,int aBlockHeight,CIngameData* aIngameData,CGraphicsBuffer* aDrawBuffer,const CRect<int>& aDrawRect);
	void DrawDarkBlock(int aX,int aY,int aType,int aNumber, unsigned char *aTarget,CIngameData* aIngameData,CGraphicsBuffer* aDrawBuffer,const CRect<int>& aDrawRect);
	void DrawMapPixel(int aOffset,int aSize,int aColor, CGraphicsBuffer* aDrawBuffer);
	void DrawRambo(CCoord<int> aPos,int aAngle,int aNumber,int aColor,const CGraphicsBuffer* aSprite,CGraphicsBuffer* aDrawBuffer,const CRect<int>& aDrawRect);
	void DrawRocket(int aX,int aY,int aAngle,int aLength,unsigned char aRocketColor,CGraphicsBuffer* aDrawBuffer,const CRect<int>& aDrawRect);
	void DrawHumanShadow(int aX,int aY,int aNumber,const unsigned char* aShadowSprite,CGraphicsBuffer* aDrawBuffer,const CRect<int>& aDrawRect);
	void DrawSprite(int aX,int aY,int aXS,int aYS, const CGraphicsBuffer* aSprite,int aOffset,CGraphicsBuffer* aDrawBuffer,const CRect<int>& aDrawRect);
	void DrawTransSprite(int aX,int aY,int aXS,int aYS, const CGraphicsBuffer* aSprite,int aOffset,CGraphicsBuffer* aDrawBuffer,const CRect<int>& aDrawRect);
	void DrawWallShadow(int aX,int aY,int aNumber,int aSize, CGraphicsBuffer* aDrawBuffer,const CRect<int>& aDrawRect);

	void DrawPolygon(CCoord<int>& aP1,CCoord<int>& aP2,CCoord<int>& aP3,int col, CGraphicsBuffer* aDarkBuffer,CGraphicsBuffer* aDrawBuffer,const CRect<int>& aDrawRect);
	void DrawPolygon(int p1x,int p1y,int p2x,int p2y,int p3x,int p3y,int col, CGraphicsBuffer* aDarkBuffer,CGraphicsBuffer* aDrawBuffer,const CRect<int>& aDrawRect);
	inline void DrawPolyTri(int k1,int k2,int x,int y,int aHeight, int col, CGraphicsBuffer* aDarkBuffer, CGraphicsBuffer* aDrawBuffer);
	int CalcX(CCoord<int>& aP1,CCoord<int>& aP2,int y);
	int CalcY(CCoord<int>& aP1,CCoord<int>& aP2,int x);

private:
	CColorTable const* iColorTable;
};

#endif
