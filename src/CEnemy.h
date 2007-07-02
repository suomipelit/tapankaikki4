#ifndef __ENEMY_H_
#define __ENEMY_H_

#include "CCharacter.h"

class CGameSystem;
class CSoundPlayer;
class CDynGameData;
class CBullet;
class IEnemyType;

class CEnemy: public CCharacter
{
	enum TState
	{
		EStill,
		EAssault,
		EWanderAround
	};

public:
	CEnemy(int aType, bool aWaitStill, const CCoord<int>& aPos, int aAngle, CGameSystem* aGS,CGameData* aGameData,ICharacterObserver* aObserver);
	void EnemyCheck();
	void DecideAndMove();
	void Move(int aAngle, float aSpeed);
	void GotoBulletOrigin(CBullet* aBullet);
	void SoundHeard(enum TIngameSoundEffect aEffect,float aVolume,CCoord<int> aPos);

	// Inline
	inline const IEnemyType* Type() const { return iType; };
	inline bool WaitStill(){ return iState==EStill; };

	// for binary search algorithms
	operator const CCoord<float>& () const
	{
		return iLocation;
	}

public: // From CCharacter
	bool GetDamage( float damage, IBulletObserver *aObserver);
	void Shoot(int/*hand*/);

	void Animate();
	void Footprint( int side );

protected:
	void TargetAngle(int speed);
	void CheckPlayerVisibility();
	void CheckForShoot();

protected:

	TState iState;
    int iWalkCounter;
	bool iSeePlayer[KMaxPlayers];
	const IEnemyType* iType;

	CGameData* iGM;
	CGameSystem* iGS;
	CSoundPlayer* iSP;
	CDynGameData* iDynData;
};

#endif


