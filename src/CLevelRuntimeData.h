#ifndef __CLEVELRUNTIMEDATA_H
#define __CLEVELRUNTIMEDATA_H 

#include <vector>
#include <list>

#include "common/CCoord.h"
#include "common/CLevel.h"
#include "CEnemy.h"
#include "CBodyPart.h"

class CPlayer;


inline bool SortOperator (const CCoord<float>& v1,const CCoord<float>& v2) // ascending
{
	return v1.X() < v2.X();
}

template <typename T>
inline bool PtrSortOperator (T* v1,T* v2) // ascending
{
	return v1->Location().X() < v2->Location().X();
}

class CIngameData;
class CEffect;
class CCrate;
class IBulletObserver;
class CGameData;
class CSoundPlayer;
class CGameSystem;

class CLevelRuntimeData
{
public:
	CLevelRuntimeData( CGameData* aGameData, CSoundPlayer* aSP,CGameSystem* aGS, CDynGameData* aDynData );
	~CLevelRuntimeData();

public:

	// every frame stuff:
	void PerFrameCalc();
	void EnemyUpdate();
	void RollEffects();

	void ChkWakeUps();

	// modify object tables
	void NewBodyPart(const CCoord<float>& aPos, int aAngle, float aSpeed, int aNum );
	void NewEffect(int type, const CCoord<float>& aPos, int angle, float speed );
	void NewBullet( int aType, const CCoord<float>& aPos, int aAngle, int aTimeAdd, float aShare, IBulletObserver* aObserver );
	void NewEnemy(int aType, bool aWaitStill,const CCoord<int>& aPos, int aAngle);
	void NewCrate( const CCoord<int>& aPos,int aType,int aValue );
	void Explo(const CCoord<int>& aPos,int aSize, float aShare, IBulletObserver* aObserver);
	std::vector< CEnemy >::iterator KillEnemy( std::vector< CEnemy >::iterator& aIter );
	
	CIngameData* IngameData() { return iIngameData;};
	inline CLevel& Level(){return iLevel;};

	bool SteamAnimationCount( int aMod ) const { return (iSteamAnimationCount % aMod) == 0;};

	const std::list< CCoord<int> >& DeadEnemies() const;
	
	std::vector<CPlayer *>& DeadPlayers();
	std::list< CEffect* >& Effects();
	std::vector< CBodyPart >& Bodyparts();
	std::vector< CEnemy >& Enemies();
	std::list< CBullet >& Bullets();
	std::list< CCrate* >& Crates();
	void PlaceGameCrates( bool aDM );
	void InitEnemies();

protected: // protected methods
	void MoveBullets();
	void MoveBodyParts();
	void MoveCrateTexts();

	bool PlaceEnemy( int aType );

	void ResetCrates();
	void ResetBullets();
	void ResetDeadPlayers();
protected:
	CLevel iLevel;

	// For animation:
	int iSteamAnimationCount;
	std::vector<CPlayer *> iDeadPlayers;
	std::list< CEffect* > iEffects;
	std::vector< CBodyPart > iBodyparts;
	std::vector< CEnemy > iEnemies;
	std::list< CBullet > iBullets;
	std::list< CCrate* > iCrates;

	std::list< CEffect* > iNewEffects; // tmp buf
	std::list< CCoord<int> > iDeadEnemies;

	CIngameData* iIngameData;
protected: // not owned
	CGameData* iGameData;
	CSoundPlayer* iSP;
	CGameSystem* iGS; 
	CDynGameData* iDynData;
};

#endif
