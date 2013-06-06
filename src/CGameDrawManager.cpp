#include "CGameDrawManager.h"
#include <algorithm>

#include "common/CFonts.h"
#include "common/CMath.h"
#include "common/CSpotlight.h"
#include "common/CEventHandler.h"
#include "common/IEffectType.h"
#include "common/IEnemyType.h"
#include "common/IWeaponType.h"
#include "common/CColorTable.h"

#include "CLevelRuntimeData.h"
#include "CIngameData.h"
#include "CGameGraphicsInterface.h"
#include "CBullet.h"
#include "CDynGameData.h"
#include "CGameDrawer.h"
#include "timer.h"
#include "CGameData.h"
#include "CEffect.h"
#include "CCrate.h"
#include "CGameStatisticsData.h"


CGameDrawManager::CGameDrawManager(CGameGraphicsInterface* aGGI, CGameData* aGameData, COptions* aOptions, CDynGameData* aDynData,CMessageBoard* aMessageBoard):
	iGGI(aGGI), iGameData(aGameData), iOptions(aOptions), iDynData(aDynData), iMessageBoard(aMessageBoard)
{
	iGameDrawer = new CGameDrawer(iGGI->ColorTable());
	ASSERT(aGGI);
	ASSERT(aGameData);
	ASSERT(aOptions);
	ASSERT(aDynData);
	ASSERT(aMessageBoard);
}

CGameDrawManager::~CGameDrawManager()
{
	delete iGameDrawer;
}

void CGameDrawManager::DrawAliveEnemies(std::vector< CEnemy >& aEnemies, const CRect<int>& aDrawRect, const CCoord<int>& aPlayerScreenPos)
{
	std::vector< CEnemy >::iterator iter = aEnemies.begin();

	while(iter != aEnemies.end() )
	{

		if (ChkForDraw((CCoord<int>)(iter->Location().Add(aDrawRect.TopLeft())),aPlayerScreenPos, KCharacterSpriteSize, aDrawRect))
		{
			int sprno = (KEnemySpriteAmount-iter->SpriteAngle()/2)+(KEnemySpriteAmount/4); // +90 degrees
			while (sprno>=KEnemySpriteAmount) sprno-=KEnemySpriteAmount;
			while (sprno<0) sprno+=KEnemySpriteAmount;

			DrawEnemy((int)(aDrawRect.iLeft+iter->Location().X()-aPlayerScreenPos.X()),
						(int)(aDrawRect.iTop+iter->Location().Y()-aPlayerScreenPos.Y()),
						sprno, iter->iAnim,
						*iDynData->LevelRuntime()->IngameData()->EnemySprite(iter->Type()->Sprite()), aDrawRect);
		}
		++iter;
	}
}

void CGameDrawManager::DrawCharacterShadows(std::vector<CPlayer*>& aPlayer, std::vector< CEnemy >& aEnemies, const CRect<int>& aDrawRect, const CCoord<int>& aPlayerScreenPos)
{
	const int xo=-1, yo=7;

	std::vector< CEnemy >::iterator iter = aEnemies.begin();

	while(iter != aEnemies.end() )
	{
		if ( ChkForDraw(
			 iter->Location().Add(aDrawRect.TopLeft()),
			 aPlayerScreenPos, KCharacterSpriteSize, aDrawRect) )
			iGameDrawer->DrawHumanShadow(int(aDrawRect.iLeft+iter->Location().X()-aPlayerScreenPos.X()+xo),
											int(aDrawRect.iTop+iter->Location().Y()-aPlayerScreenPos.Y()+yo), 5,
											iDynData->LevelRuntime()->IngameData()->GameSprite(EGameSpriteShadow)->Ptr(),
											iGGI->DrawBuffer(), aDrawRect);
		++iter;
	}

	for (int a=0; a<aPlayer.size(); a++)
		if (!aPlayer[a]->iDead)
			if (ChkForDraw(
				aPlayer[a]->Location().Add(aDrawRect.TopLeft()),
				aPlayerScreenPos, KCharacterSpriteSize, aDrawRect))

				iGameDrawer->DrawHumanShadow(int(aDrawRect.iLeft+aPlayer[a]->Location().X()-aPlayerScreenPos.X()+xo),
				int(aDrawRect.iTop+aPlayer[a]->Location().Y()-aPlayerScreenPos.Y()+yo), 5,
				iDynData->LevelRuntime()->IngameData()->GameSprite(EGameSpriteShadow)->Ptr(), iGGI->DrawBuffer(), aDrawRect);
}

void CGameDrawManager::DrawEffect(int aX, int aY,IEffectType const* aType, int aNumber, const CRect<int>& aDrawRect)
{
	void (CGameDrawer::*drawFunc)(int aX,int aY,int aXS,int aYS, const CGraphicsBuffer* aSprite,int aOffset,CGraphicsBuffer* aDrawBuffer,const CRect<int>& aDrawRect);

	if (!aType->TransparentSprite())
		drawFunc = &CGameDrawer::DrawSprite;
	else
		drawFunc = &CGameDrawer::DrawTransSprite;

	(iGameDrawer->*drawFunc)
		(
			aX-aType->XS()/2,
			aY-aType->YS()/2,
			aType->XS(),
			aType->YS(),
			iDynData->LevelRuntime()->IngameData()->GameSprite(EGameSpriteEffect),
			aType->Offset()+((aType->XS()+1) *aNumber) +321,iGGI->DrawBuffer(),
			aDrawRect
		);
}

void CGameDrawManager::DrawBodyParts(std::vector< CBodyPart >& aBodyParts, const CRect<int>& aDrawRect, const CCoord<int>& aPlayerScreenPos)
{
	std::vector< CBodyPart >::iterator iter = aBodyParts.begin();

	while(iter != aBodyParts.end() )
	{
		if (ChkForDraw(iter->Location().Add(aDrawRect.TopLeft()), aPlayerScreenPos, KBodyPartSpriteSize, aDrawRect))
			DrawBodyPart(aDrawRect.TopLeft().Add(iter->Location()).Sub(aPlayerScreenPos), iter->SpriteOffset(), aDrawRect);
		++iter;
	}
}

void CGameDrawManager::DrawBullets(std::list< CBullet >& aBullet , const CRect<int>& aDrawRect, const CCoord<int>& aPlayerScreenPos)
{
	std::list< CBullet >::iterator iter = aBullet.begin();
	int offset;

	while(iter != aBullet.end())
	{
		if (ChkForDraw(iter->Location().Add(aDrawRect.TopLeft()), aPlayerScreenPos,20, aDrawRect))
		{
			if (iter->Type()->Light()!=ELightTableNoLight)
				iGameDrawer->DrawLight(iter->Type()->Light(),
							aDrawRect.TopLeft().Add(iter->Location()).Sub(aPlayerScreenPos),
							iter->Type()->LightSize(),
							int(iter->Type()->LightLuminance()+iter->Counter()*iter->Type()->LightAdd()),
							iGGI->DrawBuffer(),
							aDrawRect);

			if (iter->Type()->Sprite() != EGameSpriteNone)
			{
				if (iter->Type()->SpriteAngles()>0)
					offset =((int)(iter->SpriteAngle()/(360/iter->Type()->SpriteAngles())) % iter->Type()->SpriteAngles()) * iter->Type()->XS();
                else
                    offset=0;

				iGameDrawer->DrawSprite(
					int(aDrawRect.iLeft+iter->Location().X()-(int) (iter->Type()->XS()>>1) -aPlayerScreenPos.X()),
					int(aDrawRect.iTop+iter->Location().Y()-(int) (iter->Type()->YS()>>1) -aPlayerScreenPos.Y()),
					iter->Type()->XS(),
					iter->Type()->YS(),
					iDynData->LevelRuntime()->IngameData()->GameSprite(iter->Type()->Sprite()),
					offset,
					iGGI->DrawBuffer(), aDrawRect);
			}
			if (iter->Type()->RocketLength() !=0)
				iGameDrawer->DrawRocket(int(aDrawRect.iLeft+iter->Location().X() - aPlayerScreenPos.X()),
				 int(aDrawRect.iTop+iter->Location().Y() - aPlayerScreenPos.Y()), iter->Angle(), iter->Type()->RocketLength(), iter->Type()->RocketColor(), iGGI->DrawBuffer(), aDrawRect);
		}
		iter++;
	}
}

//TODO: optimize
void CGameDrawManager::DrawBlackPartsOfLevel(CLevel* aLevel, CPlayer* aPlayer, const CRect<int>& aDrawRect, const CCoord<int>& aPlayerScreenPos)
{
	// Really funny procedure !!!There are 8 different situations as shown.
	//	1  2  3
	//	   |
	//	4--*--5
	//	   |
	//	6  7  8
	int a, b, xOffset, yOffset, blockx, blocky, px[5], py[5];
	//int level_size=aLevel->Width()*aLevel->Height();
	int eye_x, eye_y, scr_x_size=aDrawRect.Width();
    int draw_polys=0;

	eye_x=int(aPlayer->Location().X()+14-aPlayerScreenPos.X());
	eye_y=int(aPlayer->Location().Y()+14-aPlayerScreenPos.Y());

	if (aPlayerScreenPos.X()>=0)
		xOffset=(aPlayerScreenPos.X()/KBlockSpriteSize);
	else
		xOffset=(aPlayerScreenPos.X()+1)/KBlockSpriteSize-1;

	if (aPlayerScreenPos.Y()>=0)
		yOffset=(aPlayerScreenPos.Y()/KBlockSpriteSize);
	else
		yOffset=(aPlayerScreenPos.Y()+1)/KBlockSpriteSize-1;

	for (a=0; a<aDrawRect.Height()/KBlockSpriteSize; a++)
		for (b=0; b<(scr_x_size/KBlockSpriteSize)+1; b++)
			if (xOffset+b>0 && xOffset+b<aLevel->Width() -1&&
				yOffset+a>0 && yOffset+a<aLevel->Height()-1)
				if (aLevel->LevelData(xOffset+b, yOffset+a).iCastDarkness)
				{
					draw_polys=0;
					blockx=(xOffset+b)*KBlockSpriteSize-aPlayerScreenPos.X();
					blocky=(yOffset+a)*KBlockSpriteSize-aPlayerScreenPos.Y();

					//Situation 1
					if (blockx+KBlockSpriteSize<=eye_x && blocky+KBlockSpriteSize<eye_y)
						if (CheckCasting(xOffset+b, yOffset+a, 1, aLevel))
						{
							px[0]=blockx;
							py[0]=blocky+KBlockSpriteSize;
							px[1]=blockx+KBlockSpriteSize;
							py[1]=blocky;
							py[2]=py[0]-int((float)(eye_y-py[0])/(eye_x-px[0])*px[0]);
							if (py[2]<0)
							{
								px[2]=px[0]-int(py[0]/((float)(eye_y-py[0])/(eye_x-px[0])));
								py[2]=0;
							}
							else
								px[2]=0;

							if (px[2]>px[0]) px[2]=px[0];
							if (py[2]>py[0]) py[2]=py[0];
							py[3]=py[1]-int((float)(eye_y-py[1])/(eye_x-px[1])*px[1]);
							if (py[3]<=0)
							{
								px[3]=px[1]-int(py[1]/((float)(eye_y-py[1])/(eye_x-px[1])));
								py[3]=0;
							}
							else
								px[3]=0;
							if (px[3]>px[1]) px[3]=px[1];
							if (py[3]>py[1]) py[3]=py[1];

							if (px[2] !=px[3])
								iGameDrawer->DrawPolygon(px[2], py[2], px[3], py[3], 0, 0, -1,iDynData->DarkBuffer(), iGGI->DrawBuffer(), aDrawRect);
							draw_polys=1;
						}

					//Situation 2
					if (blockx+KBlockSpriteSize>=eye_x && blockx<=eye_x && blocky+KBlockSpriteSize<eye_y)//2
						if (CheckCasting(xOffset+b, yOffset+a, 2, aLevel))
						{
							px[0]=blockx-1;
							py[0]=blocky+KBlockSpriteSize;
							px[1]=blockx+KBlockSpriteSize;
							py[1]=blocky+KBlockSpriteSize;
							if (eye_x !=px[0])
								px[2]=px[0]-int(py[0]/((float)(eye_y-py[0])/(eye_x-px[0])));
							else
								px[2]=px[0];
							if (px[2]<0)
							{
								py[2]=py[0]-int((float)(eye_y-py[0])/(eye_x-px[0])*px[0]);
								px[2]=0;
							}
							else
								py[2]=0;
							if (eye_x !=px[1])
								px[3]=px[1]-int(py[1]/((float)(eye_y-py[1])/(eye_x-px[1])));
							else
								px[3]=px[1];
							if (px[3]>scr_x_size-1)
							{
								py[3]=py[1]+int((float)(eye_y-py[1])/(eye_x-px[1])*(scr_x_size-1 -px[1]));
								px[3]=scr_x_size -1;
							}
							else
								py[3]=0;

							if (py[2]>0 && py[3]==0)
								iGameDrawer->DrawPolygon(px[2], py[2], px[3], py[3], 0, 0, -1,iDynData->DarkBuffer(), iGGI->DrawBuffer(), aDrawRect);
							if (py[2]==0 && py[3]>0)
								iGameDrawer->DrawPolygon(px[2], py[2], px[3], py[3], scr_x_size-1, 0, -1,iDynData->DarkBuffer(), iGGI->DrawBuffer(), aDrawRect);
							if (py[2]>0 && py[3]>0)
							{
								iGameDrawer->DrawPolygon(0, 0, px[3], py[3], scr_x_size-1, 0, -1,iDynData->DarkBuffer(), iGGI->DrawBuffer(), aDrawRect);
								iGameDrawer->DrawPolygon(px[2], py[2], px[3], py[3], 0, 0, -1,iDynData->DarkBuffer(), iGGI->DrawBuffer(), aDrawRect);
							}
							draw_polys=1;
						}
					//Situation 3
					if (blockx>eye_x && blocky+KBlockSpriteSize<eye_y)
						if (CheckCasting(xOffset+b, yOffset+a, 3, aLevel))
						{
							px[0]=blockx;
							py[0]=blocky;
							px[1]=blockx+KBlockSpriteSize;
							py[1]=blocky+KBlockSpriteSize;
							py[2]=py[0]+int((float)(eye_y-py[0])/(eye_x-px[0])*(scr_x_size-1-px[0]));
							if (py[2]<0)
							{
								px[2]=px[0]-int(py[0]/((float)(eye_y-py[0])/(eye_x-px[0])));
								py[2]=0;
							}
							else
								px[2]=scr_x_size-1;
							if (px[2]<px[0]) px[2]=px[0];
							if (py[2]>py[0]) py[2]=py[0];
							py[3]=py[1]+int((float)(eye_y-py[1])/(eye_x-px[1])*(scr_x_size-1-px[1]));
							if (py[3]<0)
							{
								px[3]=px[1]-int(py[1]/((float)(eye_y-py[1])/(eye_x-px[1])));
								py[3]=0;
							}
							else
								px[3]=scr_x_size-1;
							if (px[3]<px[1]) {px[1]=scr_x_size-1;px[3]=px[1];}
							if (py[3]>py[1]) py[3]=py[1];

							if (px[2] !=px[3])
								iGameDrawer->DrawPolygon(px[2], py[2], px[3], py[3], scr_x_size-1, 0, -1,iDynData->DarkBuffer(), iGGI->DrawBuffer(), aDrawRect);
							draw_polys=1;
						}
					//Situation 4

					if (blockx<eye_x && blocky<=eye_y && blocky+KBlockSpriteSize>=eye_y)
						if (CheckCasting(xOffset+b, yOffset+a, 4, aLevel))
						{
							px[0]=blockx+KBlockSpriteSize;
							py[0]=blocky+KBlockSpriteSize;
							px[1]=blockx+KBlockSpriteSize;
							py[1]=blocky-1;
							py[2]=py[0]-int((float)(eye_y-py[0])/(eye_x-px[0])*px[0]);
							if (py[2]>aDrawRect.Height()-1)
							{
								px[2]=px[0]+int((aDrawRect.Height()-1-py[0])/((float)(eye_y -py[0])/(eye_x-px[0])));
								py[2]=aDrawRect.Height()-1;
							}
							else
								px[2]=0;
							py[3]=py[1]-int((float)(eye_y-py[1])/(eye_x-px[1])*px[1]);
							if (py[3]<0)
							{
								px[3]=px[1]-int(py[1]/((float)(eye_y-py[1])/(eye_x-px[1])));
								py[3]=0;
							}
							else
								px[3]=0;

							if (px[2]>0 && px[3]==0)
								iGameDrawer->DrawPolygon(px[2], py[2], px[3], py[3], 0, aDrawRect.Height()-1, -1,iDynData->DarkBuffer(), iGGI->DrawBuffer(), aDrawRect);
							if (px[2]==0 && px[3]>0)
								iGameDrawer->DrawPolygon(px[2], py[2], px[3], py[3], 0, 0, -1,iDynData->DarkBuffer(), iGGI->DrawBuffer(), aDrawRect);
							if (px[2]>0 && px[3]>0)
							{
								iGameDrawer->DrawPolygon(0, 0, px[2], py[2], 0, aDrawRect.Height()-1, -1,iDynData->DarkBuffer(), iGGI->DrawBuffer(), aDrawRect);
								iGameDrawer->DrawPolygon(px[2], py[2], px[3], py[3], 0, 0, -1,iDynData->DarkBuffer(), iGGI->DrawBuffer(), aDrawRect);
							}
							draw_polys=1;
						}
					//Situation 5

					if (blockx>eye_x && blocky<=eye_y && blocky+KBlockSpriteSize>=eye_y)
						if (CheckCasting(xOffset+b, yOffset+a, 5, aLevel))
						{
							px[0]=blockx;
							py[0]=blocky+KBlockSpriteSize;
							px[1]=blockx;
							py[1]=blocky-1;
							py[2]=py[0]+int((float)(eye_y-py[0])/(eye_x-px[0])*(scr_x_size-1-px[0]));
							if (py[2]>aDrawRect.Height()-1)
							{
								px[2]=px[0]+int((aDrawRect.Height()-py[0])/((float)(eye_y-py[0])/(eye_x-px[0])));
								py[2]=aDrawRect.Height()-1;
							}
							else
								px[2]=scr_x_size-1;
							py[3]=py[1]+int((float)(eye_y-py[1])/(eye_x-px[1])*(scr_x_size-1-px[1]));
							if (py[3]<0)
							{
								px[3]=px[1]-int(py[1]/((float)(eye_y-py[1])/(eye_x-px[1])));
								py[3]=0;
							}
							else
								px[3]=scr_x_size-1;

							if (px[2]<scr_x_size-1 && px[3]==scr_x_size-1)
								iGameDrawer->DrawPolygon(px[2], py[2], px[3], py[3], scr_x_size-1, aDrawRect.Height()-1, -1,iDynData->DarkBuffer(), iGGI->DrawBuffer(), aDrawRect);
							if (px[2]==scr_x_size-1 && px[3]<scr_x_size-1)
								iGameDrawer->DrawPolygon(px[2], py[2], px[3], py[3], scr_x_size-1, 0, -1,iDynData->DarkBuffer(), iGGI->DrawBuffer(), aDrawRect);
							if (px[2]<scr_x_size-1 && px[3]<scr_x_size-1)
							{
								iGameDrawer->DrawPolygon(scr_x_size-1, 0, px[2], py[2], scr_x_size-1, aDrawRect.Height()-1, -1,iDynData->DarkBuffer(), iGGI->DrawBuffer(), aDrawRect);
								iGameDrawer->DrawPolygon(px[2], py[2], px[3], py[3], scr_x_size-1, 0, -1,iDynData->DarkBuffer(), iGGI->DrawBuffer(), aDrawRect);
							}
							draw_polys=1;
						}
					//Situation 6
					if (blockx+KBlockSpriteSize<eye_x && blocky>eye_y)
						if (CheckCasting(xOffset+b, yOffset+a, 6, aLevel))
						{
							px[0]=blockx+KBlockSpriteSize;
							py[0]=blocky+KBlockSpriteSize;
							px[1]=blockx;
							py[1]=blocky;
							py[2]=py[0]-int((float)(eye_y-py[0])/(eye_x-px[0])*px[0]);
							if (py[2]>aDrawRect.Height()-1)
							{
								px[2]=px[0]+int((aDrawRect.Height()-1-py[0])/((float)(eye_y-py[0])/(eye_x-px[0])));
								py[2]=aDrawRect.Height()-1;
							}
							else
								px[2]=0;
							if (px[2]>px[0]) px[2]=px[0];
							if (py[2]<py[0]) py[2]=py[0];
							py[3]=py[1]-int((float)(eye_y-py[1])/(eye_x-px[1])*px[1]);
							if (py[3]>aDrawRect.Height()-1)
							{
								px[3]=px[1]+int((aDrawRect.Height()-1-py[1])/((float)(eye_y-py[1])/(eye_x-px[1])));
								py[3]=aDrawRect.Height()-1;
							}
							else
								px[3]=0;
							if (px[3]>px[1]) px[3]=px[1];
							if (py[3]<py[1]) py[3]=py[1];

							if (px[2] !=px[3])
								iGameDrawer->DrawPolygon(px[2], py[2], px[3], py[3], 0, aDrawRect.Height()-1, -1,iDynData->DarkBuffer(), iGGI->DrawBuffer(), aDrawRect);
							draw_polys=1;
						}
					//Situation 7
					if (blockx+KBlockSpriteSize>=eye_x && blockx<=eye_x && blocky>eye_y)
						if (CheckCasting(xOffset+b, yOffset+a, 7, aLevel))
						{
							px[0]=blockx-1;
							py[0]=blocky;
							px[1]=blockx+KBlockSpriteSize;
							py[1]=blocky;
							if (eye_x !=px[0])
								px[2]=px[0]+int((aDrawRect.Height()-1-py[0])/((float)(eye_y-py[0])/(eye_x-px[0])));
							else
								px[2]=px[0];
							if (px[2]<0)
							{
								py[2]=py[0]-int(((float)(eye_y-py[0])/(eye_x-px[0])*px[0]));
								px[2]=0;
							}
							else
								py[2]=aDrawRect.Height()-1;
							if (eye_x !=px[1])
								px[3]=px[1]+int((aDrawRect.Height()-1-py[1])/((float)(eye_y-py[1])/(eye_x-px[1])));
							else
								px[3]=px[1];
							if (px[3]>scr_x_size-1)
							{
								py[3]=py[1]+int((float)(eye_y-py[1])/(eye_x-px[1])*(scr_x_size-1 -px[1]));
								px[3]=scr_x_size-1;
							}
							else
								py[3]=aDrawRect.Height()-1;

							if (py[2]<aDrawRect.Height()-1  && py[3]==aDrawRect.Height()-1)
								iGameDrawer->DrawPolygon(px[2], py[2], px[3], py[3], 0, aDrawRect.Height()-1, -1,iDynData->DarkBuffer(), iGGI->DrawBuffer(), aDrawRect);
							if (py[2]==aDrawRect.Height()-1 && py[3]<aDrawRect.Height()-1)
								iGameDrawer->DrawPolygon(px[2], py[2], px[3], py[3], scr_x_size-1, aDrawRect.Height()-1, -1,iDynData->DarkBuffer(), iGGI->DrawBuffer(), aDrawRect);
							if (py[2]<aDrawRect.Height()-1 && py[3]<aDrawRect.Height()-1)
							{
								iGameDrawer->DrawPolygon(0, aDrawRect.Height()-1, px[3], py[3], scr_x_size-1, aDrawRect.Height()-1, -1,iDynData->DarkBuffer(), iGGI->DrawBuffer(), aDrawRect);
								iGameDrawer->DrawPolygon(px[2], py[2], px[3], py[3], 0, aDrawRect.Height()-1, -1,iDynData->DarkBuffer(), iGGI->DrawBuffer(), aDrawRect);
							}
							draw_polys=1;
						}
					//Situation 8
					if (blockx>eye_x && blocky>eye_y)
						if (CheckCasting(xOffset+b, yOffset+a, 8, aLevel))
						{
							px[0]=blockx;
							py[0]=blocky+KBlockSpriteSize;
							px[1]=blockx+KBlockSpriteSize;
							py[1]=blocky;
							py[2]=py[0]+int((float)(eye_y-py[0])/(eye_x-px[0])*(scr_x_size-1-px[0]));
							if (py[2]>aDrawRect.Height()-1)
							{
								px[2]=px[0]+int((aDrawRect.Height()-1-py[0])/((float)(eye_y-py[0])/(eye_x-px[0])));
								py[2]=aDrawRect.Height()-1;
							}
							else
								px[2]=scr_x_size-1;
							if (px[2]<px[0]) px[2]=px[0];
							if (py[2]<py[0]) py[2]=py[0];
							py[3]=py[1]+int((float)(eye_y-py[1])/(eye_x-px[1])*(scr_x_size-1-px[1]));
							if (py[3]>aDrawRect.Height()-1)
							{
								px[3]=px[1]+int((aDrawRect.Height()-1 -py[1])/((float)(eye_y-py[1])/(eye_x-px[1])));
								py[3]=aDrawRect.Height() -1;
							}
							else
								px[3]=scr_x_size-1;
							if (px[3]<px[1]) {px[1]=scr_x_size-1;px[3]=px[1];}
							if (py[3]<py[1]) py[3]=py[1];

							if (px[2] !=px[3])
								iGameDrawer->DrawPolygon(px[2], py[2], px[3], py[3], scr_x_size-1, aDrawRect.Height()-1, -1,iDynData->DarkBuffer(), iGGI->DrawBuffer(), aDrawRect);
							draw_polys=1;
						}

					if (draw_polys)
					{
						iGameDrawer->DrawPolygon(px[0], py[0], px[1], py[1], px[2], py[2], -1,iDynData->DarkBuffer(), iGGI->DrawBuffer(), aDrawRect);
						iGameDrawer->DrawPolygon(px[2], py[2], px[3], py[3], px[1], py[1], -1,iDynData->DarkBuffer(), iGGI->DrawBuffer(), aDrawRect);
					}
		}
}

void CGameDrawManager::DrawFloor(CLevel* aLevel, const CRect<int>& aDrawRect, const CCoord<int>& aPlayerScreenPos)
{
	int a, b, ovx1, ovy1;
	int x_endi, x_ofs, y_ofs;

	x_endi=(int)ceilf( aDrawRect.Width()/(float)KBlockSpriteSize)+1;

    if (aPlayerScreenPos.X()>=0)
    {
        ovx1=aPlayerScreenPos.X()%KBlockSpriteSize;
        x_ofs=aPlayerScreenPos.X()/KBlockSpriteSize;
    }
    else
    {
        x_ofs=(aPlayerScreenPos.X()+1)/KBlockSpriteSize-1;
		// TODO: solve this 10000+ thing
        ovx1=(aPlayerScreenPos.X()+10000)%KBlockSpriteSize;
    }
    if (aPlayerScreenPos.Y()>=0)
    {
        ovy1=aPlayerScreenPos.Y() % KBlockSpriteSize;
        y_ofs=aPlayerScreenPos.Y()/KBlockSpriteSize;
    }
	else
    {
		// TODO: solve this 10000+ thing
        ovy1=(10000+aPlayerScreenPos.Y())%KBlockSpriteSize;
        y_ofs=(aPlayerScreenPos.Y()+1)/KBlockSpriteSize-1;
    }

	for (a=0; a<ceilf(aDrawRect.Height()/(float)KBlockSpriteSize)+1; a ++)
		for (b=0; b<x_endi; b ++)
			if (x_ofs+b>=0 &&
				x_ofs+b<aLevel->Width() &&
				y_ofs+a>=0 &&
				y_ofs+a<aLevel->Height())
			{
				TBlock& block = aLevel->LevelData( x_ofs+b, y_ofs+a );

				if ( CLevel::FloorLevel( block ) )
				{
					iGameDrawer->DrawBlock(
						aDrawRect.iLeft+(b*KBlockSpriteSize) -ovx1,
						aDrawRect.iTop+(a*KBlockSpriteSize) -ovy1,
						block.iType,
						block.iNumber, 0,iDynData->LevelRuntime()->IngameData(), iGGI->DrawBuffer(), aDrawRect);

					iGameDrawer->DrawDarkBlock(
						aDrawRect.iLeft+(b*KBlockSpriteSize) -ovx1,
						aDrawRect.iTop+(a*KBlockSpriteSize) -ovy1,
						block.iType,
						block.iNumber,
						iDynData->DarkBuffer()->Ptr(),
						iDynData->LevelRuntime()->IngameData(),
						iGGI->DrawBuffer(), aDrawRect);
				}
			}
}

void CGameDrawManager::DrawInfos(CPlayer const* aPlayer, const CRect<int>& aDrawRect)
{
	char text[256];
	int x, z;
	const IWeaponType* wtype=aPlayer->CurrentWeapon();

	DEBUG_ASSERT( wtype );

    z=iGGI->GD()->Height()/80;

    strcpy(text,wtype->Name());
    if (aPlayer->SecondaryWeapon()>0)
        strcat(text, " (double)");
    iGGI->Font(CGameGraphicsInterface::ESmallFont)->
		Write(3+aDrawRect.iLeft,
			aDrawRect.Height()-13 -z+aDrawRect.iTop,
			text,
			CFonts::EDrawBelow, CFonts::EDrawToRight,
			iGGI->DrawBuffer());

	// check before using them as index
	DEBUG_ASSERT( wtype->BulletType() < EBulletAmount );
	DEBUG_ASSERT( wtype->BulletType() >= 0 );

	x = aPlayer->iBullets[ wtype->BulletType() ] /
		iGameData->TypeData()->iBulletType[ wtype->BulletType() ]->Multiplier();

	if (x<0)
		x=1;

	sprintf(text, "%d", x);
	if (wtype->BulletType()!=EBulletFist)
		iGGI->Font(CGameGraphicsInterface::ESmallFont)->Write(3+aDrawRect.iLeft, aDrawRect.Height()-21-z+aDrawRect.iTop, text,CFonts::EDrawBelow, CFonts::EDrawToRight,   iGGI->DrawBuffer());
}

void CGameDrawManager::DrawWalls(CLevel* aLevel, CPlayer* aPlayer, const CRect<int>& aDrawRect, const CCoord<int>& aPlayerScreenPos)
{
	int xStart, xEnd, yStart, yEnd;
    int a, b, offsetInsideBlockX, offsetInsideBlockY, xofs1, yofs1, xofs2, yofs2, size;
	int levelOffsetX, levelOffsetY;
    int perspectFixOffsetX[6];
	int perspectFixOffsetY[6];
	unsigned char col1, col2;
	const unsigned char* sprWallClr = iDynData->LevelRuntime()->IngameData()->GameSprite( EGameSpriteWallClr )->Ptr();

    if (aPlayerScreenPos.X()>=0)
    {
        offsetInsideBlockX=aPlayerScreenPos.X() % KBlockSpriteSize;
        levelOffsetX=aPlayerScreenPos.X()/KBlockSpriteSize;
    }
    else
    {
        offsetInsideBlockX=(KBlockSpriteSize-1)+((aPlayerScreenPos.X()+1) % KBlockSpriteSize);
        levelOffsetX=(aPlayerScreenPos.X()+1)/KBlockSpriteSize-1;
    }

    if (aPlayerScreenPos.Y()>=0)
    {
        offsetInsideBlockY=aPlayerScreenPos.Y() % KBlockSpriteSize;
        levelOffsetY=aPlayerScreenPos.Y()/KBlockSpriteSize;
	}
	else
    {
        offsetInsideBlockY=(KBlockSpriteSize-1)+((aPlayerScreenPos.Y()+1) % KBlockSpriteSize);
        levelOffsetY=(aPlayerScreenPos.Y()+1)/KBlockSpriteSize-1;
    }

	int ScrBlockHalfWidth = int(ceilf(aDrawRect.Width()/(KBlockSpriteSize*2.0f)));
	int ScrBlockHalfHeight = int(ceilf(aDrawRect.Height()/(KBlockSpriteSize*2.0f)));

    for (b=0; b<6; b++)
	{
		perspectFixOffsetX[b]=(int)((-offsetInsideBlockX)+((-offsetInsideBlockX)/(float)KBlockSpriteSize-ScrBlockHalfWidth)*b);
		perspectFixOffsetY[b]=(int)((-offsetInsideBlockY)+((-offsetInsideBlockY)/(float)KBlockSpriteSize-ScrBlockHalfHeight)*b);
	}


    yStart=0;
    yEnd=(int)ceilf(aDrawRect.Height()/40.0f);

	for (a=yStart; a<yEnd; a ++)
	{
		xStart=0;
		xEnd=(int)ceilf(aDrawRect.Width()/40.0f);

		yofs1=(a*KBlockSpriteSize)-offsetInsideBlockY;
		//upper left
		for (b=xStart; b<xEnd; b++)
		{
 			TBlock& block = aLevel->LevelData(levelOffsetX+b, levelOffsetY+a);

			xofs1=(b*KBlockSpriteSize)-offsetInsideBlockX;
			if (levelOffsetX+b>=0 && levelOffsetX+b<aLevel->Width() && levelOffsetY+a>=0 &&
				levelOffsetY+a<aLevel->Height())
			{
				if (!CLevel::FloorLevel( block ))
				{
					xofs2=perspectFixOffsetX[block.iHeight]+b*(block.iHeight+KBlockSpriteSize);
					yofs2=perspectFixOffsetY[block.iHeight]+a*(block.iHeight+KBlockSpriteSize);
					size=KBlockSpriteSize+block.iHeight;
					col1=sprWallClr[block.iNumber+16*(block.iNumber/16)];
					col2=sprWallClr[block.iNumber+16*(block.iNumber/16)+16];

					if (!(aLevel->LevelData(levelOffsetX+b, levelOffsetY+a+1).iType==EBlockTypeWall &&
						aLevel->LevelData(levelOffsetX+b, levelOffsetY+a+1).iHeight>=block.iHeight))
					{
						iGameDrawer->DrawPolygon(xofs2-1, yofs2+size,
													xofs1-1, yofs1+KBlockSpriteSize,
													xofs1+KBlockSpriteSize, yofs1+KBlockSpriteSize,
													col2,NULL, iGGI->DrawBuffer(), aDrawRect);
						iGameDrawer->DrawPolygon(xofs2-1, yofs2+size,
													xofs2+size, yofs2+size,
													xofs1+KBlockSpriteSize, yofs1+KBlockSpriteSize,
													col2,NULL, iGGI->DrawBuffer(), aDrawRect);
					}
					if (!(aLevel->LevelData(levelOffsetX+b+1, levelOffsetY+a).iType==EBlockTypeWall &&
						aLevel->LevelData(levelOffsetX+b+1, levelOffsetY+a).iHeight>=block.iHeight))
					{
						iGameDrawer->DrawPolygon(xofs2+size, yofs2+size,
													xofs1+KBlockSpriteSize, yofs1+KBlockSpriteSize,
													xofs2+size, yofs2-1,
													col1+1,NULL, iGGI->DrawBuffer(), aDrawRect);
						iGameDrawer->DrawPolygon(xofs1+KBlockSpriteSize, yofs1-1,
													xofs1+KBlockSpriteSize, yofs1+KBlockSpriteSize,
													xofs2+size, yofs2-1,
													col1 +1,NULL, iGGI->DrawBuffer(), aDrawRect);
					}

					if (xofs2+block.iHeight+KBlockSpriteSize>0 &&
						yofs2+block.iHeight+KBlockSpriteSize>0)
					{
						iGameDrawer->DrawBlock(aDrawRect.iLeft+xofs2, aDrawRect.iTop+ yofs2, block.iType, block.iNumber, block.iHeight,iDynData->LevelRuntime()->IngameData(), iGGI->DrawBuffer(), aDrawRect);
						iGameDrawer->DrawWallShadow(aDrawRect.iLeft+xofs2, aDrawRect.iTop+yofs2, block.iShadow, block.iHeight+KBlockSpriteSize, iGGI->DrawBuffer(), aDrawRect);
					}
				}
			}
			else
			{
				xofs2=int(perspectFixOffsetX[aLevel->OutBlock().iHeight]+b*(aLevel->OutBlock().iHeight+(float)KBlockSpriteSize));
				yofs2=int(perspectFixOffsetY[aLevel->OutBlock().iHeight]+a*(aLevel->OutBlock().iHeight+(float)KBlockSpriteSize));
				iGameDrawer->DrawBlock(aDrawRect.iLeft+xofs2, aDrawRect.iTop+ yofs2, aLevel->OutBlock().iType, aLevel->OutBlock().iNumber, aLevel->OutBlock().iHeight, iDynData->LevelRuntime()->IngameData(),iGGI->DrawBuffer(), aDrawRect);
			}
		}
		//upper right

		xStart=(int)ceilf(aDrawRect.Width()/(float)KBlockSpriteSize);

		for (b=xStart; b>=xEnd; b--)
		{
 			TBlock& block = aLevel->LevelData(levelOffsetX+b, levelOffsetY+a);

			xofs1=(b*KBlockSpriteSize)-offsetInsideBlockX;
			if (levelOffsetX+b>=0 && levelOffsetX+b<aLevel->Width() && levelOffsetY+a>=0 &&
				levelOffsetY+a<aLevel->Height())
			{
				if (!CLevel::FloorLevel( block ))
				{
					size=KBlockSpriteSize+block.iHeight;
					col1=sprWallClr[block.iNumber+16*(block.iNumber/16)];
					col2=sprWallClr[block.iNumber+16*(block.iNumber/16)+16];
					xofs2=perspectFixOffsetX[block.iHeight]+b*(block.iHeight+KBlockSpriteSize);
					yofs2=perspectFixOffsetY[block.iHeight]+a*(block.iHeight+KBlockSpriteSize);

					if (!(aLevel->LevelData(levelOffsetX+b, levelOffsetY+a+1).iType==EBlockTypeWall &&
						aLevel->LevelData(levelOffsetX+b, levelOffsetY+a+1).iHeight>=block.iHeight))
					{
						iGameDrawer->DrawPolygon(xofs2 -1, yofs2+size,
													xofs1-1, yofs1+KBlockSpriteSize,
													xofs1+KBlockSpriteSize, yofs1+KBlockSpriteSize,
													col2,NULL, iGGI->DrawBuffer(), aDrawRect);
						iGameDrawer->DrawPolygon(xofs2 -1, yofs2+size,
													xofs2+size, yofs2+size,
													xofs1+KBlockSpriteSize, yofs1+KBlockSpriteSize,
													col2,NULL, iGGI->DrawBuffer(), aDrawRect);
					}
					if (!(aLevel->LevelData(levelOffsetX+b-1, levelOffsetY+a).iType==EBlockTypeWall &&
						aLevel->LevelData(levelOffsetX+b-1, levelOffsetY+a).iHeight>=block.iHeight) &&
						xofs2>xofs1)
					{
						iGameDrawer->DrawPolygon(xofs2-1, yofs2-1,
													xofs1-1, yofs1-1,
													xofs2-1, yofs2+size,
													col2-1,NULL, iGGI->DrawBuffer(), aDrawRect);
						iGameDrawer->DrawPolygon(xofs1-1, yofs1+KBlockSpriteSize,
													xofs1-1, yofs1-1,
													xofs2-1, yofs2+size,
													col2-1,NULL, iGGI->DrawBuffer(), aDrawRect);
					}

					if (xofs2<aDrawRect.Width() &&
						yofs2+block.iHeight+KBlockSpriteSize>0)
					{
						iGameDrawer->DrawBlock(aDrawRect.iLeft+xofs2, aDrawRect.iTop+ yofs2, block.iType, block.iNumber, block.iHeight, iDynData->LevelRuntime()->IngameData(),iGGI->DrawBuffer(), aDrawRect);
						iGameDrawer->DrawWallShadow(aDrawRect.iLeft+xofs2,
													aDrawRect.iTop+yofs2,
													block.iShadow,
													block.iHeight+KBlockSpriteSize,
													iGGI->DrawBuffer(),
													aDrawRect);
					}
				}
			}
			else
			{
				xofs2=int(perspectFixOffsetX[aLevel->OutBlock().iHeight]+b*(aLevel->OutBlock().iHeight+(float)KBlockSpriteSize));
				yofs2=int(perspectFixOffsetY[aLevel->OutBlock().iHeight]+a*(aLevel->OutBlock().iHeight+(float)KBlockSpriteSize));
				iGameDrawer->DrawBlock(aDrawRect.iLeft+xofs2, aDrawRect.iTop+yofs2, aLevel->OutBlock().iType, aLevel->OutBlock().iNumber, aLevel->OutBlock().iHeight, iDynData->LevelRuntime()->IngameData(),iGGI->DrawBuffer(), aDrawRect);
			}
		}
    }
	//lower left
    yStart=(int)ceilf(aDrawRect.Height()/(float)KBlockSpriteSize);
//    yEnd=(int)(ceilf(aDrawRect.Height()/40.0f) - 1;
    for (a=yStart; a>=yEnd; a--)
	{
		xStart=0;

		xEnd=(int)ceilf(aDrawRect.Width()/40.0f);

		yofs1=(a*KBlockSpriteSize)-offsetInsideBlockY;
		for (b=xStart; b<xEnd; b++)
		{
 			TBlock& block = aLevel->LevelData(levelOffsetX+b, levelOffsetY+a);

			xofs1=(b*KBlockSpriteSize)-offsetInsideBlockX;
			if (levelOffsetX+b>=0 && levelOffsetX+b<aLevel->Width() && levelOffsetY+a>=0 &&
				levelOffsetY+a<aLevel->Height())
			{
				if (!CLevel::FloorLevel( block ))
				{
					size=KBlockSpriteSize+block.iHeight;
					col1=sprWallClr[block.iNumber+16*(block.iNumber/16)];
					col2=sprWallClr[block.iNumber+16*(block.iNumber/16)+16];
					xofs2=perspectFixOffsetX[block.iHeight]+b*(block.iHeight+KBlockSpriteSize);
					yofs2=perspectFixOffsetY[block.iHeight]+a*(block.iHeight+KBlockSpriteSize);

					if (yofs2>yofs1 &&
						!(aLevel->LevelData(levelOffsetX+b, levelOffsetY+a-1).iType==EBlockTypeWall &&
						aLevel->LevelData(levelOffsetX+b, levelOffsetY+a-1).iHeight>=block.iHeight))
					{
						iGameDrawer->DrawPolygon(xofs2-1, yofs2-1,
													xofs1-1, yofs1-1,
													xofs1+KBlockSpriteSize, yofs1-1,
													col1,NULL, iGGI->DrawBuffer(), aDrawRect);
						iGameDrawer->DrawPolygon(xofs2-1, yofs2-1,
													xofs2+size, yofs2-1,
													xofs1+KBlockSpriteSize, yofs1-1,
													col1,NULL, iGGI->DrawBuffer(), aDrawRect);
					}
					if (!(aLevel->LevelData(levelOffsetX+b+1, levelOffsetY+a).iType==EBlockTypeWall &&
						aLevel->LevelData(levelOffsetX+b+1, levelOffsetY+a).iHeight>=block.iHeight))
					{
						iGameDrawer->DrawPolygon(xofs2+size, yofs2+size, xofs1+KBlockSpriteSize, yofs1+KBlockSpriteSize, xofs2+size, yofs2-1, col1+1,NULL, iGGI->DrawBuffer(), aDrawRect);
						iGameDrawer->DrawPolygon(xofs1+KBlockSpriteSize, yofs1-1, xofs1+KBlockSpriteSize, yofs1+KBlockSpriteSize, xofs2+size, yofs2-1, col1+1,NULL, iGGI->DrawBuffer(), aDrawRect);
					}

					if (xofs2+block.iHeight+KBlockSpriteSize>0 &&
						yofs2<aDrawRect.Height())
					{
						iGameDrawer->DrawBlock(aDrawRect.iLeft+xofs2, aDrawRect.iTop+yofs2, block.iType, block.iNumber, block.iHeight, iDynData->LevelRuntime()->IngameData(),iGGI->DrawBuffer(), aDrawRect);
						iGameDrawer->DrawWallShadow(aDrawRect.iLeft+xofs2, aDrawRect.iTop+yofs2, block.iShadow, block.iHeight+KBlockSpriteSize, iGGI->DrawBuffer(), aDrawRect);
					}
				}
			}
			else
			{
				xofs2=int(perspectFixOffsetX[aLevel->OutBlock().iHeight]+b*(aLevel->OutBlock().iHeight+(float)KBlockSpriteSize));
				yofs2=int(perspectFixOffsetY[aLevel->OutBlock().iHeight]+a*(aLevel->OutBlock().iHeight+(float)KBlockSpriteSize));
				iGameDrawer->DrawBlock(aDrawRect.iLeft+xofs2, aDrawRect.iTop+yofs2, aLevel->OutBlock().iType, aLevel->OutBlock().iNumber, aLevel->OutBlock().iHeight, iDynData->LevelRuntime()->IngameData(),iGGI->DrawBuffer(), aDrawRect);
			}
		}
		//lower right

		xStart=(int)ceilf(aDrawRect.Width()/(float)KBlockSpriteSize);

		for (b=xStart; b>=xEnd; b--)
		{
 			TBlock& block = aLevel->LevelData(levelOffsetX+b, levelOffsetY+a);

			xofs1=(b*KBlockSpriteSize)-offsetInsideBlockX;
			if (levelOffsetX+b>=0 && levelOffsetX+b<aLevel->Width() && levelOffsetY+a>=0 &&
				levelOffsetY+a<aLevel->Height())
			{
				if (!CLevel::FloorLevel( block ))
				{
					size=KBlockSpriteSize+block.iHeight;
					col1=sprWallClr[block.iNumber+16*(block.iNumber/16)];
					col2=sprWallClr[block.iNumber+16*(block.iNumber/16)+16];
					xofs2=perspectFixOffsetX[block.iHeight]+b*(block.iHeight+KBlockSpriteSize);
					yofs2=perspectFixOffsetY[block.iHeight]+a*(block.iHeight+KBlockSpriteSize);

					if (yofs2>yofs1 &&
						!(aLevel->LevelData(levelOffsetX+b, levelOffsetY+a-1).iType==EBlockTypeWall &&
						aLevel->LevelData(levelOffsetX+b, levelOffsetY+a-1).iHeight>=block.iHeight))
					{
						iGameDrawer->DrawPolygon(xofs1+KBlockSpriteSize, yofs1-1,
													xofs2-1, yofs2-1,
													xofs1-1, yofs1-1,
													col1,NULL, iGGI->DrawBuffer(), aDrawRect);
						iGameDrawer->DrawPolygon(xofs2-1, yofs2-1,
													xofs2+size, yofs2-1,
													xofs1+KBlockSpriteSize, yofs1-1,
													col1,NULL, iGGI->DrawBuffer(), aDrawRect);
					}
					if (xofs2>xofs1 &&
						!(aLevel->LevelData(levelOffsetX+b-1, levelOffsetY+a).iType==EBlockTypeWall &&
						aLevel->LevelData(levelOffsetX+b-1, levelOffsetY+a).iHeight>=block.iHeight))
					{
						iGameDrawer->DrawPolygon(xofs2-1, yofs2-1,
													xofs1-1, yofs1-1,
													xofs2-1, yofs2+size,
													col2-1,NULL, iGGI->DrawBuffer(), aDrawRect);
						iGameDrawer->DrawPolygon(xofs1-1, yofs1+KBlockSpriteSize,
													xofs1-1, yofs1-1,
													xofs2-1, yofs2+size,
													col2-1,NULL, iGGI->DrawBuffer(), aDrawRect);
					}

					if (xofs2<aDrawRect.Width() &&yofs2<aDrawRect.Height())
					{
						iGameDrawer->DrawBlock(aDrawRect.iLeft+xofs2, aDrawRect.iTop+yofs2, block.iType, block.iNumber, block.iHeight, iDynData->LevelRuntime()->IngameData(),iGGI->DrawBuffer(), aDrawRect);
						iGameDrawer->DrawWallShadow(aDrawRect.iLeft+xofs2, aDrawRect.iTop+yofs2, block.iShadow, block.iHeight+KBlockSpriteSize, iGGI->DrawBuffer(), aDrawRect);
					}
				}
			}
			else
			{
				xofs2=int(perspectFixOffsetX[aLevel->OutBlock().iHeight]+b*(aLevel->OutBlock().iHeight+(float)KBlockSpriteSize));
				yofs2=int(perspectFixOffsetY[aLevel->OutBlock().iHeight]+a*(aLevel->OutBlock().iHeight+(float)KBlockSpriteSize));
				iGameDrawer->DrawBlock(aDrawRect.iLeft+xofs2, aDrawRect.iTop+yofs2, aLevel->OutBlock().iType, aLevel->OutBlock().iNumber, aLevel->OutBlock().iHeight, iDynData->LevelRuntime()->IngameData(),iGGI->DrawBuffer(), aDrawRect);
			}
		}
    }
}

int CGameDrawManager::CheckCasting(int aXOfs, int aYOfs, int aMode, CLevel* aLevel)//mode =situation in draw_black()
{
	//This function checks necessary dark DrawPolygon drawing, returns 1 if necessary
	switch (aMode){
    case 1: if (aLevel->LevelData(aXOfs-1, aYOfs).iCastDarkness &&
                aLevel->LevelData(aXOfs-1, aYOfs-1).iCastDarkness &&
                aLevel->LevelData(aXOfs, aYOfs-1).iCastDarkness) return 0;
		break;
    case 2: if (aLevel->LevelData(aXOfs, aYOfs+1).iCastDarkness) return 0;
		break;
    case 3: if (aLevel->LevelData(aXOfs+1, aYOfs).iCastDarkness &&
                aLevel->LevelData(aXOfs+1, aYOfs-1).iCastDarkness &&
                aLevel->LevelData(aXOfs, aYOfs-1).iCastDarkness) return 0;
		break;
    case 4: if (aLevel->LevelData(aXOfs+1, aYOfs).iCastDarkness) return 0;
		break;
    case 5: if (aLevel->LevelData(aXOfs-1, aYOfs).iCastDarkness) return 0;
		break;
    case 6: if (aLevel->LevelData(aXOfs-1, aYOfs).iCastDarkness &&
                aLevel->LevelData(aXOfs-1, aYOfs+1).iCastDarkness &&
                aLevel->LevelData(aXOfs, aYOfs+1).iCastDarkness) return 0;
		break;
    case 7: if (aLevel->LevelData(aXOfs, aYOfs-1).iCastDarkness) return 0;
		break;
    case 8: if (aLevel->LevelData(aXOfs+1, aYOfs).iCastDarkness &&
                aLevel->LevelData(aXOfs+1, aYOfs+1).iCastDarkness &&
                aLevel->LevelData(aXOfs, aYOfs+1).iCastDarkness) return 0;
		break;
	}

	return 1;
}

void CGameDrawManager::DrawFireLights(CRect<int> aDrawRect, const CCoord<int>& aPlayerScreenPos)
{
	int a;
	CCoord<int> ofs;
	const int fire1ofs(4);
	const int fire2ofs(2);
	const CCoord<int> drawOfs(14,13);

	for(a=0; a<iDynData->Players().size(); a++)
	{
		ASSERT(iDynData->Players()[a]);
		CPlayer* pl = iDynData->Players()[a];

		if ((pl->CurrentWeapon()->Gun()==1)&&
			(pl->iFire || pl->iFire2))
		{
			ofs.SetMovement(pl->RealAngle(),8);

			if (pl->iFire)
			{
				ofs = ofs.AddMovement(pl->RealAngle()+90, fire1ofs);
			}
			else
			{
				ofs = ofs.AddMovement(pl->RealAngle()+270, fire2ofs);
			}
			CCoord<int> pos = pl->Location().Add(drawOfs).Add(ofs).Add(aDrawRect.TopLeft());

			if (ChkForDraw(
				pos,
				aPlayerScreenPos,
				iGGI->ColorTable()->LightMap(3).Size(), aDrawRect))

				iGameDrawer->DrawLight(
					ELightTableNormal,
					pos.Sub( aPlayerScreenPos ),
					3, 5 - pl->iFire, iGGI->DrawBuffer(), aDrawRect);
		}
	}

	std::vector< CEnemy >::iterator iter = iDynData->LevelRuntime()->Enemies().begin();

	while(iter != iDynData->LevelRuntime()->Enemies().end() )
	{
		if (iter->CurrentWeapon()->Gun()==1)
			if (iter->iFire)
			{
				ofs.SetMovement((int)(iter->SpriteAngle()*4.5f),10);
				ofs = ofs.AddMovement((int)((iter->SpriteAngle()*4.5f)+90),3);
				CCoord<int> pos = iter->Location().Add(drawOfs).Add(ofs).Add(aDrawRect.TopLeft());

				if (ChkForDraw(pos,
					aPlayerScreenPos,
					iGGI->ColorTable()->LightMap(3).Size(), aDrawRect))
				iGameDrawer->DrawLight(
					ELightTableNormal,
					pos.Sub( aPlayerScreenPos ),
					3, 2 - iter->iFire, iGGI->DrawBuffer(), aDrawRect);
			}
		++iter;
	}
}

//TODO: optimize
void CGameDrawManager::DrawShadows(CLevel* aLevel, const CRect<int>& aDrawRect, const CCoord<int>& aPlayerScreenPos)
{
	int a, b, ovx1, ovy1;
	int x_endi, xOffset, yOffset;

	if (aPlayerScreenPos.X()>=0)
    {
		ovx1=aPlayerScreenPos.X() % 20;
        xOffset=aPlayerScreenPos.X()/20;
	}
    else
    {
		ovx1=(aPlayerScreenPos.X()+10000) % 20;
		xOffset=(aPlayerScreenPos.X()+1)/20-1;
	}
    if (aPlayerScreenPos.Y()>=0)
    {
		ovy1=aPlayerScreenPos.Y() % 20;
        yOffset=aPlayerScreenPos.Y()/20;
	}
	else
    {
		ovy1=(aPlayerScreenPos.Y()+10000) % 20;
		yOffset=(aPlayerScreenPos.Y()+1)/20-1;
	}

	int y_endi = (int)ceilf(aDrawRect.Height()/20.0f)+1;
	x_endi=(int)ceilf(aDrawRect.Width()/20.0f)+1;

    for (a=min(0,yOffset); a<y_endi; a ++)
		for (b=min(0,-xOffset); b<x_endi; b ++)
			if (xOffset+b>=0 &&
				xOffset+b<aLevel->Width() &&
				yOffset+a>0 &&
				yOffset+a<aLevel->Height())
			{
				if (aLevel->LevelData(xOffset+b, yOffset+a).iShadow>0)
					if (aLevel->LevelData(xOffset+b, yOffset+a).iType==EBlockTypeFloor||
						(aLevel->LevelData(xOffset+b, yOffset+a).iType==EBlockTypeWall &&
						aLevel->LevelData(xOffset+b, yOffset+a).iHeight==0))
						iGameDrawer->DrawWallShadow(aDrawRect.iLeft+(b*20) -ovx1,
									   aDrawRect.iTop+(a*20) -ovy1,
									   aLevel->LevelData(xOffset+b, yOffset+a).iShadow, 20, iGGI->DrawBuffer(), aDrawRect);
			}
}

int CGameDrawManager::ChkForDraw(CCoord<int> aPos, const CCoord<int>& aPlayerScreenPos, int aSize, const CRect<int>& aDrawRect)
{
	CRect<int> testrect(aPlayerScreenPos.X()-aSize+aDrawRect.iLeft,
						aPlayerScreenPos.Y()-aSize+aDrawRect.iTop,
						aPlayerScreenPos.X()+aDrawRect.iRight,
						aPlayerScreenPos.Y()+iGGI->GD()->Height()+aDrawRect.iBottom);

	if (testrect.Inside(aPos))
		return 1;

	return(0);
}


void CGameDrawManager::DrawPlayers(int pos, std::vector<CPlayer*>& aPlayer, const CRect<int>& aDrawRect, const CCoord<int>& aPlayerScreenPos)
{
	int a;
    int spr;

	for (a=0; a<aPlayer.size(); a++)
	{
		if (aPlayer[a]->iDead)
			spr =0;
		else
		{
            if (aPlayer[a]->CurrentWeapon()>0 && aPlayer[a]->SecondaryWeapon()>0)
				spr =2;
            else
				spr =aPlayer[a]->CurrentWeapon()->Sprite();
		}
		if ((pos==0 && aPlayer[a]->iDead)|| (pos==1 && !aPlayer[a]->iDead))
			if (ChkForDraw(aPlayer[a]->Location().Add(aDrawRect.TopLeft()), aPlayerScreenPos, KCharacterSpriteSize, aDrawRect))
				iGameDrawer->DrawRambo(aPlayer[a]->Location().Add(aDrawRect.TopLeft()).Sub(aPlayerScreenPos),
							aPlayer[a]->SpriteAngle(),
							aPlayer[a]->iAnim,
							aPlayer[a]->Color(),
							iDynData->LevelRuntime()->IngameData()->RamboSprite(spr), iGGI->DrawBuffer(), aDrawRect);
	}
}

void CGameDrawManager::DrawEffects(std::list<CEffect*>& aEffects, int aPos, const CRect<int>& aDrawRect, const CCoord<int>& aPlayerScreenPos)
{
	std::list< CEffect* >::iterator low = aEffects.begin();// lower_bound(aEffects.begin(),aEffects.end(),&left,PtrSortOperator< CEffect >);
	std::list< CEffect* >::iterator high = aEffects.end();//upper_bound(aEffects.begin(),aEffects.end(),&right,PtrSortOperator< CEffect >);

	while( low!= high )
	{
		if ((*low)->Type()->Order()==aPos)
		{
			if (ChkForDraw(
				(*low)->Location().Add(aDrawRect.TopLeft()).Add(-(*low)->Type()->XS()/2,-(*low)->Type()->YS()/2),
				aPlayerScreenPos,
				max((*low)->Type()->XS(),(*low)->Type()->YS()),
				aDrawRect))
			{
				if ((*low)->Type()->LightTable() != ELightTableNoLight )
						iGameDrawer->DrawLight(
							(*low)->Type()->LightTable(),
							aDrawRect.TopLeft().Add((*low)->Location()).Sub(aPlayerScreenPos),
							(*low)->Type()->LightSize(),
							(*low)->Type()->LightAdd( (*low)->Counter() ),
							iGGI->DrawBuffer(),
							aDrawRect);

				if ((*low)->Type()->Sprite())
					DrawEffect(
						int(aDrawRect.iLeft+(*low)->Location().X()-aPlayerScreenPos.X()),
						int(aDrawRect.iTop+(*low)->Location().Y()-aPlayerScreenPos.Y()),
						(*low)->Type(),
						(*low)->Counter()/(*low)->Type()->Delay(),
						aDrawRect);
			}
		}
		low++;
	}
}

void CGameDrawManager::DrawSpots(CSpotLight* aSpots, int aSpotAnimCount, const CRect<int>& aDrawRect, const CCoord<int>& aPlayerScreenPos)
{
	CSpotLight* ptr=aSpots;
	int flash;

	while(ptr)
	{
		flash=0;
		if (rand()%50==0)
			flash=-2+rand()%5;

		if (ChkForDraw(ptr->Location().Add(aDrawRect.TopLeft()).Add(-30,-30),aPlayerScreenPos,60, aDrawRect))
			iGameDrawer->DrawLight(ELightTableNormal,
				aDrawRect.TopLeft().Add( ptr->Location() ).Sub( aPlayerScreenPos),
				ptr->Size(), 2 + flash,iGGI->DrawBuffer(), aDrawRect);
		ptr=ptr->Next();
	}
}

void CGameDrawManager::DrawEnergies(CPlayer const* aPlayer, const CRect<int>& aDrawRect)
{
	int a, b, target, offset, yadd;
	const int w = iGGI->DrawBuffer()->Width();

	float div = KPlayerMaxEnergy+aPlayer->iShield*KShieldEnergyAmount;

    target = aDrawRect.Height()/80;
    offset = ((aDrawRect.Height()-3-target+aDrawRect.iTop) *w) +3+aDrawRect.iLeft;

    for (a=0; a<aPlayer->iEnergy/5; a++)
		for (b=0,yadd=0;b<target;b++,yadd+=w)
		{
			DEBUG_ASSERT( a/div <= 1.0f );
			DEBUG_ASSERT( a/div >= 0.0f );
			(*iGGI->DrawBuffer())[offset+a+yadd] = (unsigned char)(239-(int)(5*a*15/div));
		}
}

void CGameDrawManager::DrawMap(CLevel* aLevel, std::vector< CEnemy >& aEnemies, const std::list< CCoord<int> >& aDeadEnemies, std::vector<CPlayer*>& aPlayers, bool aHidePlayers,bool aDrawEnemies, bool aDrawReachable)
{
	int a, b, offset, cx=0, cy=0, type, size;

    if (iGGI->GD()->Width()>=640)
		size=2;
	else
		size =1;

	cx=iGGI->GD()->Width()-(aLevel->Width())*size;
	cy=0;

	offset=cy*iGGI->DrawBuffer()->Width()+cx;
	for (a=0; a<aLevel->Height(); a++, offset+=(iGGI->DrawBuffer()->Width()-aLevel->Width())*size)
		for (b=0; b<aLevel->Width(); b++, offset+=size)
		{
			type=aLevel->LevelData(b, a).iType;
			if (type==EBlockTypeWall)
				iGameDrawer->DrawMapPixel(offset, size, 16,iGGI->DrawBuffer());
			else
				iGameDrawer->DrawMapPixel(offset, size, -1,iGGI->DrawBuffer());

			if (aDrawReachable)
				if (aLevel->Reachable(CCoord<int>(b,a)))
					iGameDrawer->DrawMapPixel(offset, size, 3,iGGI->DrawBuffer());

		}

	std::list< CCoord<int> >::const_iterator iter = aDeadEnemies.begin();

	while( iter != aDeadEnemies.end() )
	{
		iGameDrawer->DrawMapPixel(((int)((iter->Y()+15)/20)*size +cy)*iGGI->DrawBuffer()->Width()+(int) ((iter->X()+15) /20)*size +cx, size, 9,iGGI->DrawBuffer());

		++iter;
	}

	if (aDrawEnemies)
	{
		std::vector< CEnemy >::const_iterator iter2 = aEnemies.begin();
		while(iter2 != aEnemies.end() )
		{
			iGameDrawer->DrawMapPixel(((int)((iter2->Location().Y()+15)/20)*size +cy)*iGGI->DrawBuffer()->Width()+(int) ((iter2->Location().X()+15) /20)*size +cx, size, 3,iGGI->DrawBuffer());
			iter2++;
		}
	}

    if ( !aHidePlayers )
		for (a=0; a<aPlayers.size(); a++)
			iGameDrawer->DrawMapPixel(((int)((aPlayers[a]->Location().Y()+15) /20)*size +cy)*iGGI->DrawBuffer()->Width()+(int) ((aPlayers[a]->Location().X()+15) /20)*size +cx,
						size,
						aPlayers[a]->Color(),iGGI->DrawBuffer());
}

void CGameDrawManager::DrawHits(CPlayer* aPlayer, const CRect<int>& aDrawRect)
{
	if (aPlayer->iHit>0)
		iGGI->DrawBuffer()->Rectangle(
			aDrawRect.iLeft, aDrawRect.iTop,
			aDrawRect.iRight-1, aDrawRect.iBottom-1,
			((4*16) -1) -aPlayer->iHit, 0);
}

void CGameDrawManager::DrawTargets(CPlayer* aPlayer, CLevel* aLevel, const CRect<int>& aDrawRect, const CCoord<int>& aPlayerScreenPos)
{
	int a, dist;
	CCoord<float> pos;
	CCoord<float> endpos;
	static int plot_count=0;

	plot_count++;
	if (!aPlayer->iDead)
		if (aPlayer->CurrentWeapon()->Gun())
			if (aPlayer->iTargetSystem)
			{
				pos = aPlayer->Location().AddMovement(aPlayer->RealAngle(),10).Add(14,14);
				dist = iGameData->TypeData()->iBulletType[aPlayer->CurrentWeapon()->BulletType()]->ApproxDistance();

				aLevel->VisibilityCheck(pos,pos.AddMovement(aPlayer->RealAngle(),dist),&endpos);
				dist = int(CMath::GetDist(pos,endpos));

				for (a=0;
					a<dist &&
					pos.X()-aPlayerScreenPos.X()>=0 &&
					pos.X()-aPlayerScreenPos.X()<aDrawRect.Width() &&
					pos.Y()-aPlayerScreenPos.Y()>=0 &&
					pos.Y()-aPlayerScreenPos.Y()<aDrawRect.Height(); a++)
				{
					if ((a-(int) (plot_count/4))  % 6==0)
						iGGI->DrawBuffer()->PutPixel(int(aDrawRect.iLeft+pos.X()-aPlayerScreenPos.X()), int(aDrawRect.iTop+pos.Y()-aPlayerScreenPos.Y()), 9*16);
					pos = pos.AddMovement(aPlayer->RealAngle(),1);
				}
				if (a==dist||aLevel->LevelData(int(pos.X()/20), int(pos.Y()/20)).iType==EBlockTypeWall)
					DrawTarget(int(aDrawRect.iLeft+pos.X()-3-aPlayerScreenPos.X()), int(aDrawRect.iTop+ pos.Y()-3-aPlayerScreenPos.Y()), aDrawRect);
			}
}

void CGameDrawManager::DrawCrates(std::list< CCrate*>& aCrates, const CRect<int>& aDrawRect, const CCoord<int>& aPlayerScreenPos)
{
	std::list< CCrate*>::iterator iter=aCrates.begin();

	while( iter != aCrates.end() )
	{
		if ((*iter)->InUse())
			if ((*iter)->TextCounter()==0)
			{
				if (ChkForDraw((*iter)->Location().Add(aDrawRect.TopLeft()), aPlayerScreenPos, 14, aDrawRect))
				{
					iGameDrawer->DrawLight(ELightTableNormal,
						aDrawRect.TopLeft().Add((*iter)->Location()).Sub( aPlayerScreenPos).Add(7,7),
						4, 2, iGGI->DrawBuffer(), aDrawRect);
					DrawCrate(aDrawRect.iLeft+(*iter)->Location().X()-aPlayerScreenPos.X(), aDrawRect.iTop+ (*iter)->Location().Y()-aPlayerScreenPos.Y(), (*iter)->SpriteOffset(), aDrawRect);
				}
			}
		iter++;
	}
}

void CGameDrawManager::DrawCrateTexts( std::list< CCrate*>& aCrates )
{
	std::list< CCrate*>::iterator iter=aCrates.begin();

	while( iter != aCrates.end() )
	{
		if ((*iter)->InUse())
			if ((*iter)->TextCounter()>0)
				iGGI->Font(CGameGraphicsInterface::ESmallFont)->Write(int((*iter)->TextPos().X()), int((*iter)->TextPos().Y()), (*iter)->Text(),CFonts::EDrawBelow, CFonts::EDrawToRight,  iGGI->DrawBuffer());
		iter++;
	}
}

void CGameDrawManager::DrawKills(std::vector<CPlayer*>& aPlayers)
{
	char text[50];
	int a, b=0;

	for (a=0; a<aPlayers.size();a++)
	{
		int kills = iDynData->Statistics()->PlayerStats( aPlayers[a] ).LevelStats( iDynData->CurrentLevel() ).TotalPlayerKills( aPlayers[a] );
		ASSERT(_snprintf(text,50, "%s %d", aPlayers[a]->iName, kills )>0);
		iGGI->Font(CGameGraphicsInterface::ESmallFont)->Write(3+(b % 4) *80, 3+(b/4) *10, text,CFonts::EDrawBelow, CFonts::EDrawToRight,  iGGI->DrawBuffer());
		b++;
	}
}

void CGameDrawManager::DrawCrate(int aX, int aY, int aSpriteOffset, const CRect<int>& aDrawRect)
{
	iGameDrawer->DrawSprite(aX, aY, 14, 14, iDynData->LevelRuntime()->IngameData()->GameSprite(EGameSpriteCrate),aSpriteOffset, iGGI->DrawBuffer(), aDrawRect);
}

void CGameDrawManager::DrawEnemy(int aX, int aY, int aAngle, int aNumber, const CGraphicsBuffer& aSprite, const CRect<int>& aDrawRect)
{
	iGameDrawer->DrawSprite(aX, aY, KCharacterSpriteSize, KCharacterSpriteSize, &aSprite, ((aNumber*29)*1160+aAngle*29), iGGI->DrawBuffer(), aDrawRect);
}

void CGameDrawManager::DrawTarget(int aX, int aY, const CRect<int>& aDrawRect)
{
	iGameDrawer->DrawTransSprite(aX, aY, 7, 7, iDynData->LevelRuntime()->IngameData()->GameSprite(EGameSpriteTarget), 0, iGGI->DrawBuffer(), aDrawRect);
}

void CGameDrawManager::DrawBodyPart(const CCoord<int>& aPos, int aSpriteOffset, const CRect<int>& aDrawRect)
{
	iGameDrawer->DrawSprite(aPos.X(), aPos.Y(), KBodyPartSpriteSize, KBodyPartSpriteSize, iDynData->LevelRuntime()->IngameData()->GameSprite(EGameSpriteBodyPart), aSpriteOffset, iGGI->DrawBuffer(), aDrawRect);
}

void CGameDrawManager::DrawFrame(CDrawArea& aDrawArea,CEventHandler* aEventHandler)
{
	int a;
	const int players = iDynData->LocalPlayers().size();

	for (a=0;a<iDynData->BlackRects( players );a++)
	{
		iGGI->DrawBuffer()->FillRectangle( iDynData->BlackRect( a, players ), 0);
	}

	for (a=0;a<players;a++)
	{
		CPlayer* plr = iDynData->LocalPlayers()[a];
		const CCoord<int>& scrPos = plr->ScreenPos();
		const CRect<int>& drawRect = iDynData->PlayerDrawRect(a);

		DrawFloor(&iDynData->LevelRuntime()->Level(),drawRect,scrPos);
		DrawPlayers(0,iDynData->Players(),drawRect,scrPos);// dead players
		DrawEffects(iDynData->LevelRuntime()->Effects(),0,drawRect,scrPos); // low effects
		DrawBodyParts(iDynData->LevelRuntime()->Bodyparts(),drawRect,scrPos);
		DrawCharacterShadows(iDynData->Players(),iDynData->LevelRuntime()->Enemies(),drawRect,scrPos);
		DrawCrates( iDynData->LevelRuntime()->Crates(), drawRect,scrPos);
		DrawBullets( iDynData->LevelRuntime()->Bullets(), drawRect,scrPos);
		DrawAliveEnemies(iDynData->LevelRuntime()->Enemies(),drawRect,scrPos);
		DrawPlayers(1,iDynData->Players(),drawRect,scrPos);// live players

		if (!(aEventHandler->State(SDLK_1)&&EF_DEBUG))
			DrawFireLights(drawRect,scrPos);

		if (!(aEventHandler->State(SDLK_2)&&EF_DEBUG))
			DrawShadows(&iDynData->LevelRuntime()->Level(),drawRect,scrPos); // block shadows

		DrawEffects(iDynData->LevelRuntime()->Effects(),1,drawRect,scrPos); // higher effects

		if (!(aEventHandler->State(SDLK_3)&&EF_DEBUG))
			DrawSpots(iDynData->LevelRuntime()->Level().iSpotLights,iDynData->LevelRuntime()->Level().iCntSpotLights,drawRect,scrPos);

		if (!(aEventHandler->State(SDLK_4)&&EF_DEBUG))
			DrawBlackPartsOfLevel(&iDynData->LevelRuntime()->Level(),plr,drawRect,scrPos);

		DrawWalls(&iDynData->LevelRuntime()->Level(),plr,drawRect,scrPos);

		DrawHits(plr,drawRect);
		DrawTargets(plr,&iDynData->LevelRuntime()->Level(),drawRect,scrPos);
		DrawEnergies(plr,drawRect);
		DrawInfos(plr,drawRect);
	}

	if ( iDynData->GameMode()->Frags() )
		DrawKills(iDynData->LocalPlayers());

	DrawCrateTexts( iDynData->LevelRuntime()->Crates() );

	if (iDynData->iShowMap)
		DrawMap(
			&iDynData->LevelRuntime()->Level(),
			iDynData->LevelRuntime()->Enemies(),
			iDynData->LevelRuntime()->DeadEnemies(),
			iDynData->Players(),
			iDynData->GameMode()->HidePlayersFromMap(),
			EF_DEBUG,
			aEventHandler->State(SDLK_5)&&EF_DEBUG);

	if (iDynData->iDrawFrames)
		DrawFPS();

	if (iDynData->GameMode()->ActionTimeMatters())
		DrawTime();

	if (iDynData->iNextLevel)
		DrawComplete();

	//if (iDynData->MessageWritingMode())
	//	DrawNetworkMessage();

	// TODO: do dynamic versio of this, don't add whole screen always
	aDrawArea.Combine(iGGI->DrawBuffer()->Rect());
}

const unsigned char KMissionCompleted[] = "MISSION COMPLETED";

void CGameDrawManager::DrawComplete()
{
	int a;
	int xpos, ypos;
	float tmp;

	a=(iDynData->iNextLevel)*2;

	tmp = (float) iDynData->iNextLevel / (float) KLevelCompleteDelay;

	xpos = int(iGGI->GD()->Width()/2* (1+((1-tmp) * CMath::Static()->Cose(a))));
	ypos = int((iGGI->GD()->Height() / 2)* (1+((1-tmp) * CMath::Static()->Sine(a))));

	iGGI->Font(CGameGraphicsInterface::ESmallFont)->Write(
		xpos,
		ypos,KMissionCompleted,
		CFonts::EDrawInVertMiddle, CFonts::EDrawInMiddle,
		iGGI->DrawBuffer());
}

void CGameDrawManager::DrawTime()
{
	char col=1;
	const CFonts* fnt = iGGI->Font(CGameGraphicsInterface::ESmallFont);

	if (!iDynData->iNextLevel)
		iDynData->iCompleteTime=(TimerCounter-iDynData->StartTime())/KTimerFreq;

	if ( iDynData->iCompleteTime > iDynData->LevelRuntime()->Level().iGeneralLevelInfo.iTimeLimit )
		col=3*16; // red, I presume?

	fnt->Write(iGGI->GD()->Width()/2,10,FormatTime(iDynData->iCompleteTime).c_str(),CFonts::EDrawBelow, CFonts::EDrawInMiddle, iGGI->DrawBuffer());
}

void CGameDrawManager::DrawFPS()
{
	char text[20];

	ASSERT(_snprintf(text,20,"%d / %d fps", (int)iDynData->CurrentFPS(), KTimerFreq)>0);
	iGGI->Font(CGameGraphicsInterface::ESmallFont)->Write(iGGI->GD()->Width()/2,20,text,CFonts::EDrawBelow, CFonts::EDrawInMiddle, iGGI->DrawBuffer());

	if ( EF_DEBUG)
	{
		ASSERT(_snprintf(text,20,"%d / %d effects", iDynData->LevelRuntime()->Effects().size(), KTimerFreq)>0);
		iGGI->Font(CGameGraphicsInterface::ESmallFont)->Write(iGGI->GD()->Width()/2,40,text,CFonts::EDrawBelow, CFonts::EDrawInMiddle, iGGI->DrawBuffer());
	}
}

std::string CGameDrawManager::FormatTime( int aika )
{
	char text[10];
	ASSERT(_snprintf(text,10,"%d:%02d",aika / 60,aika % 60)>0);
	return text;
}
