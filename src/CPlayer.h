#ifndef __CPLAYER_H
#define __CPLAYER_H

#include "CCharacter.h"
#include "CKeys.h"
#include "network.h"
#include "common/CRect.h"
#include "common/debug.h"

const int KStraightTolerance = 10;	// Helps player to get the rambo go straight
const float KPlayerNormalSpeed = 0.75f;
const float KPlayerRunSpeed    = 1.0f;
const int KPlayerMaxEnergy = 100;

const int KShieldEnergyAmount = 10;

class CGraphicsBuffer;
class CDynGameData;
class CGameGraphicsInterface;
class CSoundPlayer;
class CEventHandler;
class CDynGameData;
class CDynShopData;

class CPlayer: public CCharacter
{
public:
	CPlayer(CDynGameData* aDD, CGameGraphicsInterface* aGGI, CGameData* aGameData, CSoundPlayer* aSP,CGameSystem* aGS,ICharacterObserver* aObserver);
	void InitScr(const CRect<int>& aDrawRect);
	void MoveScr(const CRect<int>& aDrawRect);
	void Move(int aAngle, float aSpeed, int aIndex, const CRect<int>& aDrawRect);
	void Reset(const CRect<int>& aDrawRect);
	void TotalReset(const CRect<int>& aDrawRect);
	void RandomPlace();
	void CheckKeys(int iIndex, CEventHandler* aEH, const CRect<int>& aDrawRect);
	void Rotate( float aChange );
	const CCoord<int>& ScreenPos() const;
	void ChangeWeapon(bool aReverse);
	int RealAngle(){ return (int)iAngle;}
	void SetAngle(float aAngle);

	void SetName( const char* aName );
	char* Name();

	void SetSpeed( float aSpeed );

	// for shop usage
	void ShopBuy(int aAmount,CDynShopData* aDSD);
	void ShopSell(int aAmount,CDynShopData* aDSD);
	int ShieldPrice();
	int Color() const; 
	void SetColorByIndex(int aColor);
    
public:  // From CCharacter
	bool GetDamage(float damage, IBulletObserver *aObserver);
	void Animate();
	void Shoot(int hand);
	void Footprint( int side );

public: // from IBulletObserver
	
	void BulletHit( float aShare );
	void BulletPlayerKill( CPlayer* aPlayer );
	void BulletEnemyKill( CEnemy* aEnemy );

public:

	// TODO: This should be boolean not an int. 
	// This should NEVER be used as counter as it is now... huoh.
	int iDead;
	bool iNoDamage;

	bool iWeaponInHand;
	int iHit;
	int iCash;
	int iShield;
	int iBullets[ EBulletAmount ];
	char* iName;
	int iKick;
	CKeys iKeys;
	bool iTargetSystem;
	int iWeapons[ EWeaponAmount ];

private:
	
	void PickupCrates( const CRect<int>& aDrawRect );
	void KickBodyParts();
	void Straighten();
    void ChooseBestWeapon();
	__DECLARE_TEST;

protected:
	int iColor;
	float iSpeed;

private:
	CCoord<int> iScreenPos;
	CGameSystem* iGS;
	CDynGameData* iDynData;
	CGameGraphicsInterface* iGGI;
	CSoundPlayer* iSP;
};

#endif


