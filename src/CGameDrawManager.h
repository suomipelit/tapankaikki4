#ifndef __GAMEDRAWMANAGER_H_
#define __GAMEDRAWMANAGER_H_

#include <vector>
#include <list>
#include "common/CRect.h"

class CEnemy;
class CLevel;
class CPlayer;
class CBodyPart;
class CCrate;
class CBullet;
class CEffect;
class CDynGameData;
class CSpotLight;
class CEffectType;
class CGameDrawer;
class CGameGraphicsInterface;
class COptions;
class CGameData;
class CDynGameData;
class CGraphicsBuffer;
class CEventHandler;
class CMessageBoard;
class IEffectType;
class CDrawArea;

class CGameDrawManager
{
public:
	CGameDrawManager(CGameGraphicsInterface* aGGI,CGameData* aGameData,COptions* aOptions, CDynGameData* aDynData, CMessageBoard* aMessageBoard);
	~CGameDrawManager();

	void DrawFrame(CDrawArea& aDrawArea,CEventHandler* aEventHandler);

	int ChkForDraw(CCoord<int> aPos, const CCoord<int>& aPlayerScreenPos, int aSize, const CRect<int>& aDrawRect);
	void DrawAliveEnemies(std::vector< CEnemy >& aEnemies,const CRect<int>& aDrawRect,const CCoord<int>& aPlayerScreenPos);
	void DrawBlackPartsOfLevel(CLevel* aLevel, CPlayer* aPlayer, const CRect<int>& aDrawRect, const CCoord<int>& aPlayerScreenPos);
	void DrawBodyPart(const CCoord<int>& aPos, int aSpriteOffset, const CRect<int>& aDrawRect);
	void DrawBodyParts(std::vector< CBodyPart >& aBodyParts, const CRect<int>& aDrawRect, const CCoord<int>& aPlayerScreenPos);
	void DrawBullets(std::list< CBullet >& aBullet ,const CRect<int>& aDrawRect, const CCoord<int>& aPlayerScreenPos);
	void DrawCharacterShadows(std::vector<CPlayer*>& aPlayer,std::vector< CEnemy >& aEnemy,const CRect<int>& aDrawRect, const CCoord<int>& aPlayerScreenPos);
	void DrawCrate(int aX,int aY,int aSpriteOffset,const CRect<int>& aDrawRect);
	void DrawCrates(std::list< CCrate*>& aCrates,const CRect<int>& aDrawRect, const CCoord<int>& aPlayerScreenPos);
	void DrawCrateTexts(std::list< CCrate*>& aCrates);
	void DrawEnemy(int aX,int aY,int aAngle,int aNumber,const CGraphicsBuffer& aSprite,const CRect<int>& aDrawRect);
	void DrawEnergies(CPlayer const* aPlayer, const CRect<int>& aDrawRect);
	void DrawFireLights(CRect<int> aDrawRect, const CCoord<int>& aPlayerScreenPos);
	void DrawFloor(CLevel* aLevel,const CRect<int>& aDrawRect, const CCoord<int>& aPlayerScreenPos);
	void DrawHits(CPlayer* aPlayer,const CRect<int>& aDrawRect);
	void DrawInfos(CPlayer const* aPlayer, const CRect<int>& aDrawRect);
	void DrawKills(std::vector<CPlayer*>& aPlayer);
	void DrawMap(CLevel* aLevel, std::vector< CEnemy >& aEnemies, const std::list< CCoord<int> >& aDeadEnemies, std::vector<CPlayer*>& aPlayers, bool aDM,bool aDrawEnemies, bool aDrawReachable);
	void DrawPlayers(int pos,std::vector<CPlayer*>& aPlayer,const CRect<int>& aDrawRect, const CCoord<int>& aPlayerScreenPos);
	void DrawShadows(CLevel* aLevel,const CRect<int>& aDrawRect, const CCoord<int>& aPlayerScreenPos);
	void DrawSpots(CSpotLight* aSpots, int aSpotAnimCount,const CRect<int>& aDrawRect, const CCoord<int>& aPlayerScreenPos);
	void DrawTarget(int x,int y,const CRect<int>& aDrawRect);
	void DrawTargets(CPlayer* aPlayer,CLevel* aLevel,const CRect<int>& aDrawRect, const CCoord<int>& aPlayerScreenPos);
	void DrawWalls(CLevel* aLevel,CPlayer* aPlayer,const CRect<int>& aDrawRect, const CCoord<int>& aPlayerScreenPos);
	std::string FormatTime(int aika);
	
	//void DrawNetworkMessage();
	void DrawComplete();
	void DrawTime(); 
	void DrawFPS();

private:
	void DrawEffect(int aX,int aY, IEffectType const * aType,int aNumber,const CRect<int>& aDrawRect);
	void DrawEffects(std::list<CEffect*>& aEffects, int pos,const CRect<int>& aDrawRect, const CCoord<int>& aPlayerScreenPos);


private:

	int  CheckCasting(int aXOfs,int aYOfs,int aMode,CLevel* aLevel);
	CGameDrawer* iGameDrawer;
	CGameGraphicsInterface* iGGI;
	CGameData* iGameData;
	CDynGameData* iDynData;
	COptions* iOptions;
	CMessageBoard* iMessageBoard;
};

#endif

