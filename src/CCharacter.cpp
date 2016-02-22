#include <vector>
#include "CCharacter.h"

#include "common/IWeaponType.h"
#include "common/CMath.h"

#include "CDynGameData.h"
#include "CGameData.h"
#include "CLevelRuntimeData.h"

CCharacter::CCharacter(ICharacterObserver* aObserver,CGameData* aGameData): iTargetAngle(0), iBurning(0),iFryingObserver(NULL),
						iPushPower(0),iPushAngle(0),iAngle(0),iEnergy(0),iWalk(0),iFire(0),iFire2(0),
						iAnim(0),iLoadCounter(0), iLoadCounter2(0),iSecondaryCounter(0),iShootCount(0),
						iSecondaryWeapon(NULL), iObserver(aObserver),
						iGameData(aGameData)
{
	ASSERT(aGameData!=NULL);
	ASSERT(aObserver!=NULL);
	SetCurrentWeapon(0);
}

void CCharacter::LoadWeapons() 
{
	if (iLoadCounter<iCurrentWeapon->LoadingTime()) 
		iLoadCounter++;

	if (iSecondaryWeapon>0)
        if (iLoadCounter2<iSecondaryWeapon->LoadingTime()) 
			iLoadCounter2++;

    if (iSecondaryCounter>0) 
		iSecondaryCounter--;
}

bool CCharacter::CollisionCheck(const CRect<int>& aRect)
{
	return aRect.Inside(CCoord<int>(iLocation));
}

void CCharacter::Move(int aAngle, float aSpeed, CDynGameData* aDynData)
{
	CCoord<int> blockpos[8];// **
	const int reuna=6;      //*  * chk pointit on näin
	const int reuna2=4;     //*  *
				           // **
	CCoord<int> testpos;
	CCoord<float> newpos;

	// Generate Footprints
	if (iWalk==(int) (KWalkAnimationDelay*0.25) ||
		iWalk==(int) (KWalkAnimationDelay*0.75)) 
		Footprint(0);

	if (iWalk==(int) (KWalkAnimationDelay*0.5) ||
		iWalk==KWalkAnimationDelay-1)			 
		Footprint(1);

	newpos=iLocation.AddMovement(aAngle,aSpeed);
	testpos = newpos;

	blockpos[0].Set((testpos.X()+14-reuna2)/KBlockSpriteSize,(testpos.Y()+reuna)/KBlockSpriteSize);
	blockpos[1].Set((testpos.X()+14+reuna2)/KBlockSpriteSize,(testpos.Y()+reuna)/KBlockSpriteSize);
	blockpos[2].Set((testpos.X()+14-reuna2)/KBlockSpriteSize,(testpos.Y()+KCharacterSpriteSize-reuna)/KBlockSpriteSize);
	blockpos[3].Set((testpos.X()+14+reuna2)/KBlockSpriteSize,(testpos.Y()+KCharacterSpriteSize-reuna)/KBlockSpriteSize);
	blockpos[4].Set((testpos.X()+reuna)/KBlockSpriteSize,(testpos.Y()+14-reuna2)/KBlockSpriteSize);
	blockpos[5].Set((testpos.X()+reuna)/KBlockSpriteSize,(testpos.Y()+14+reuna2)/KBlockSpriteSize);
	blockpos[6].Set((testpos.X()+KCharacterSpriteSize-reuna)/KBlockSpriteSize,(testpos.Y()+14-reuna2)/KBlockSpriteSize);
	blockpos[7].Set((testpos.X()+KCharacterSpriteSize-reuna)/KBlockSpriteSize,(testpos.Y()+14+reuna2)/KBlockSpriteSize);

	if (newpos.Y()<iLocation.Y()&&
	    aDynData->LevelRuntime()->Level().LevelData(blockpos[0]).iType==EBlockTypeFloor&&
	    aDynData->LevelRuntime()->Level().LevelData(blockpos[1]).iType==EBlockTypeFloor) 
	{
		if (!aDynData->CharCollision(CRect<int>(testpos.X()-15,testpos.Y()+2*reuna-KCharacterSpriteSize,testpos.X()+15,testpos.Y()),this)) 
		{
			if (newpos.Y()<0)
				iLocation.SetY(0);
			else
				iLocation.SetY(newpos.Y());
		}
	}
	if (newpos.Y()>iLocation.Y()&&
	    aDynData->LevelRuntime()->Level().LevelData(blockpos[2]).iType==EBlockTypeFloor&&
	    aDynData->LevelRuntime()->Level().LevelData(blockpos[3]).iType==EBlockTypeFloor) 
	{
		if (!aDynData->CharCollision(CRect<int>(testpos.X()-15,testpos.Y(),testpos.X()+15,testpos.Y()+KCharacterSpriteSize-2*reuna),this)) 
		{
			if (newpos.Y()>aDynData->LevelRuntime()->Level().Height()*KBlockSpriteSize-KCharacterSpriteSize)
				iLocation.SetY(aDynData->LevelRuntime()->Level().Height()*KBlockSpriteSize-KCharacterSpriteSize);
			else
				iLocation.SetY(newpos.Y());
		}
	}
	if (newpos.X()<iLocation.X()&&
	    aDynData->LevelRuntime()->Level().LevelData(blockpos[4]).iType==EBlockTypeFloor&&
	    aDynData->LevelRuntime()->Level().LevelData(blockpos[5]).iType==EBlockTypeFloor) 
	{
		if (!aDynData->CharCollision(CRect<int>(testpos.X()+2*reuna-KCharacterSpriteSize,testpos.Y()-15,testpos.X(),testpos.Y()+15),this)) 
		{
			if (newpos.X()<0)
				iLocation.SetX(0);
			else
				iLocation.SetX(newpos.X());
		}
	}
	if (newpos.X()>iLocation.X()&&
	    aDynData->LevelRuntime()->Level().LevelData(blockpos[6]).iType==EBlockTypeFloor&&
	    aDynData->LevelRuntime()->Level().LevelData(blockpos[7]).iType==EBlockTypeFloor) 
	{
		if (!aDynData->CharCollision(CRect<int>(testpos.X(),testpos.Y()-15,testpos.X()+KCharacterSpriteSize-2*reuna,testpos.Y()+15),this)) 
		{
			if (newpos.X()>aDynData->LevelRuntime()->Level().Width()*KBlockSpriteSize-KCharacterSpriteSize)
				iLocation.SetX(aDynData->LevelRuntime()->Level().Width()*KBlockSpriteSize-KCharacterSpriteSize);
			else
				iLocation.SetX(newpos.X());
		}
	}
}

void CCharacter::SetCurrentWeapon(int aIndex)
{
	DEBUG_ASSERT( aIndex >= 0 ); // zero means fist
	DEBUG_ASSERT( aIndex < EWeaponAmount );

	iCurrentWeapon = iGameData->TypeData()->iWeaponType[aIndex];
	DEBUG_ASSERT(iCurrentWeapon);
}

void CCharacter::SetSecondaryWeapon(int aIndex)
{
	DEBUG_ASSERT( aIndex >= 0 );
	DEBUG_ASSERT( aIndex < EWeaponAmount );

	if ( aIndex == 0 )
		iSecondaryWeapon = NULL;
	else
		iSecondaryWeapon = iGameData->TypeData()->iWeaponType[aIndex];
}

const IWeaponType*  CCharacter::CurrentWeapon() const
{
	return iCurrentWeapon;
}

const IWeaponType*  CCharacter::SecondaryWeapon() const
{
	return iSecondaryWeapon;
}

bool CCharacter::CheckBurning(CDynGameData* aDynData)
{
	if (iBurning) 
	{
		iBurning--;
		if (aDynData->LevelRuntime()->SteamAnimationCount(15)) 
			aDynData->LevelRuntime()->NewEffect(EEffectSteam,Location().Add(14,14),315,1);

		int mod = 3;
		if (iBurning>150) 
			mod=2; 
		if (aDynData->LevelRuntime()->SteamAnimationCount( mod )) 
			aDynData->LevelRuntime()->NewEffect(EEffectFlame2,Location().Add(10+rand()%8,10+rand()%8),315,1);

		return GetDamage(0.1f,iFryingObserver);
	}
	
	return false;
}

void CCharacter::BulletHit( float aShare )
{
}

void CCharacter::BulletPlayerKill( CPlayer* aPlayer )
{
}

void CCharacter::BulletEnemyKill( CEnemy* aEnemy )
{
}

