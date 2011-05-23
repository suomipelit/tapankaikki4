#include<stdlib.h>
#include<string.h>

#include <algorithm>

#include "CPlayer.h"

#include "common/CEventHandler.h"
#include "common/CMath.h"
#include "common/Types.h"
#include "common/IBodyPartType.h"
#include "common/IWeaponType.h"
#include "common/IShield.h"

#include "CCrate.h"
#include "CGameSystem.h"
#include "CSoundPlayer.h"
#include "IGUIStateController.h"
#include "CDynGameData.h"
#include "CDynShopData.h"
#include "CGameStatisticsData.h"
#include "CIngameData.h"
#include "CLevelRuntimeData.h"

const float KPlayerRotationSpeed=3.4f; // players rotate speed keyb

CPlayer::CPlayer(CDynGameData* aDD, CGameGraphicsInterface* aGGI, CGameData* aGameData, CSoundPlayer* aSP,CGameSystem* aGS,ICharacterObserver* aObserver): CCharacter(aObserver,aGameData), iGS(aGS), iGGI(aGGI), iSP(aSP), iDynData(aDD)
{
	SetColorByIndex( 0 );
	iDead = false;
	iShield = 0;
	iCash = 0;
	iName = new char[1];
	iName[0] = 0;
	iSpeed = KPlayerNormalSpeed;
}

void CPlayer::RandomPlace() 
{
	int a;
	int testCount=0;
	SELFTEST;

	bool messageShown = false;
	bool badLocation = false;

	ASSERT( iDynData->LevelRuntime()->Level().Width() != 0 );
	ASSERT( iDynData->LevelRuntime()->Level().Height() != 0 );
	
	do 
	{
		badLocation = false;
		bool easyMode = false;
		int x,y;

		if (testCount>200)
		{
			easyMode = true;
		}

		if (easyMode)
		{
			x = (rand() % iDynData->LevelRuntime()->Level().Width()) * KBlockSpriteSize;
			y = (rand() % iDynData->LevelRuntime()->Level().Height()) * KBlockSpriteSize;
			if (EF_DEBUG && !messageShown)
			{
				iGS->GUIStateController()->MessageBoard()->AddMessage("CPlayer::RandomPlace(): Hard to place player...");
				messageShown = true;
			}
		}
		else
		{
			x = rand() % ( iDynData->LevelRuntime()->Level().Width() * KBlockSpriteSize );
			y = rand() % ( iDynData->LevelRuntime()->Level().Height() * KBlockSpriteSize );
		}

		iLocation.SetX( x );
		iLocation.SetY( y );

		if (iDynData->LevelRuntime()->Level().LevelData(iLocation.Div(KBlockSpriteSize)).iType!= EBlockTypeFloor) 
			badLocation = true;

		if (y%KBlockSpriteSize)
			if (iDynData->LevelRuntime()->Level().LevelData(iLocation.Div(KBlockSpriteSize).Add(0,1)).iType!= EBlockTypeFloor) 
				badLocation = true;
		if ((y%KBlockSpriteSize)&&
			(x%KBlockSpriteSize))
			if (iDynData->LevelRuntime()->Level().LevelData(iLocation.Div(KBlockSpriteSize).Add(1,1)).iType!= EBlockTypeFloor) 
				badLocation = true;
		if (x%KBlockSpriteSize)
			if (iDynData->LevelRuntime()->Level().LevelData(iLocation.Div(KBlockSpriteSize).Add(1,0)).iType!= EBlockTypeFloor) 
				badLocation = true;

		if (!badLocation)
		{
			std::vector< CEnemy >::iterator iter=iDynData->LevelRuntime()->Enemies().begin();
			int eta = 100;

			if (easyMode)
			{
				eta = 30;
			}

			while(iter != iDynData->LevelRuntime()->Enemies().end() &&!badLocation)
			{	
				if (CMath::GetDist(iLocation, iter->Location() )<eta) 
					badLocation = true;
				++iter;
			}
		}
		for (a = 0;a<iDynData->Players().size() && !badLocation;a++) 
		 if (iDynData->Players()[a]!= this) 
			  if (CMath::GetDist(Location(), iDynData->Players()[a]->Location())<30.0) 
				  badLocation = true; //not good place

		testCount++;
		if (testCount >= 10000)
			throw CCriticalException("CPlayer::RandomPlace(): Placing of a player was impossible");
	} 
	while (badLocation);
	SELFTEST;
}

void CPlayer::KickBodyParts() 
{
	std::vector<CBodyPart>::iterator iter = iDynData->LevelRuntime()->Bodyparts().begin();

	const int lx = KCharacterSpriteSize/2-KBodyPartSpriteSize;
	const int rx = KCharacterSpriteSize/2;
	CCoord<float> left = iLocation.AddX(lx);
	CCoord<float> right = iLocation.AddX(rx);

	std::vector< CBodyPart >::iterator low = lower_bound(iDynData->LevelRuntime()->Bodyparts().begin(),iDynData->LevelRuntime()->Bodyparts().end(),left,SortOperator);
	std::vector< CBodyPart >::iterator high = upper_bound(iDynData->LevelRuntime()->Bodyparts().begin(),iDynData->LevelRuntime()->Bodyparts().end(),right,SortOperator);

	while(low != high)
	{
		if ((iLocation.X()+rx>low->Location().X())&&
		    (iLocation.X()+lx<low->Location().X())&&
		    (iLocation.Y()+rx>low->Location().Y())&&
		    (iLocation.Y()+lx<low->Location().Y()))
		{
			iKick = 1;
			int ang = (RealAngle()-5)+(rand()%10);
			ANGLEFIX(ang);
			low->Kick(ang);
			iSP->PlaySample( iDynData->LevelRuntime()->IngameData()->SoundFX( low->Type()->Sound()), KFullVolume, PAN_MIDDLE);

		}
		low++;
	}
}

void CPlayer::ChooseBestWeapon()
{
	int best = 0, selection = 0;

	SELFTEST;
	//SetCurrentWeapon(0); // start seeking from fist

	for (int a = 0;a<EWeaponAmount;a++)
	{
		if (iWeapons[a]>0 &&  // player has it
			iGameData->TypeData()->iWeaponType[a]->Priority() > best && // this is better than currect suggestion
			iBullets[iGameData->TypeData()->iWeaponType[a]->BulletType()]>0 ) // player has bullets 
		{
			best = iGameData->TypeData()->iWeaponType[a]->Priority();
			selection = a;
		}
	}
	SetCurrentWeapon(selection);
}

void CPlayer::CheckKeys(int aIndex, CEventHandler* aEH, const CRect<int>& aDrawRect) 
{
	SELFTEST;

	int walk_chk = 0;
	float speedi = 0;
	TDirection strafe = ENowhere;
	TDirection forward = ENowhere;
	int FAngle = 0;

	if (aEH->State(SDLK_6)&&EF_DEBUG)
		iNoDamage = true;
	else
		iNoDamage = false;

	if (aEH->State(SDLK_7)&&EF_DEBUG)
	{
		iWeapons[ EWeaponAutoRifle ] = 1;
		iWeapons[ EWeaponHeavyLauncher ] = 1;
		iBullets[ EBulletHeavyRockets ] = 500;
		iBullets[ EBullet12mm ] = 500;
	}

	// If I'm not the mouseman...
	if (iDynData->MouseOwner()!= this&& 
		!aEH->State(iKeys.keys[EKeyLEFT]) && 
		!aEH->State(iKeys.keys[EKeyRIGHT]) && 
		iTargetAngle == iAngle) 
		Straighten();

	// Rotate player
	if (aEH->State(iKeys.keys[EKeyLEFT])) 
		if (!aEH->State(iKeys.keys[EKeySTRAFE])) 
			Rotate(-KPlayerRotationSpeed);

	if (aEH->State(iKeys.keys[EKeyRIGHT])) 
		if (!aEH->State(iKeys.keys[EKeySTRAFE])) 
			Rotate(KPlayerRotationSpeed);

	// Chk strafe
	if ((aEH->State(iKeys.keys[EKeySTRAFE])&&aEH->State(iKeys.keys[EKeyLEFT])) ||
		 aEH->State(iKeys.keys[EKeyLSTRAFE])) 
	{
		strafe = ELeft;//left
		walk_chk = 1;
	}
	if ((aEH->State(iKeys.keys[EKeySTRAFE])&&aEH->State(iKeys.keys[EKeyRIGHT])) ||
		 aEH->State(iKeys.keys[EKeyRSTRAFE])) 
	{
		strafe = ERight;//right
		walk_chk = 1;
	}

	if (aEH->State(iKeys.keys[EKeyUP])) 
	{
        forward = EUp;//forward
		walk_chk = 1;
        if (rand()%40 == 0) KickBodyParts();
	}

	if (aEH->State(iKeys.keys[EKeyDOWN])) 
	{
        forward = EDown;//backwards
		walk_chk = 1;
	}
	
	//Movement start
	if (iDynData->MouseOwner() == this)
		FAngle = RealAngle();
	else
		FAngle = (int)(SpriteAngle()*4.5f);

	if (forward == EUp)
	{//Eteen
		if (strafe == ELeft)
			FAngle-= 45;
		if (strafe == ERight)
			FAngle+= 45;
		speedi = iSpeed;
	}

	if (forward == EDown)
	{//Taakse
		FAngle+= 180;
		if (strafe == ELeft)
			FAngle+= 45;
		if (strafe == ERight)
			FAngle-= 45;
		speedi = iSpeed*0.75f;
	}

	if (forward == ENowhere)
	{//Pelkästään sivulle
		if (strafe == ELeft)
			FAngle-= 90;
		if (strafe == ERight)
			FAngle+= 90;
		if (strafe>0) 
			speedi = iSpeed; 
		else 
			speedi = 0;
	}

	ANGLEFIX(FAngle);
	if (speedi!= 0) 
		Move(FAngle, speedi, aIndex, aDrawRect);
	//Movement End
	
	if (walk_chk && iWalk == 0) 
		iWalk = 1;

	if (!walk_chk) 
		iWalk = 0;

	if (aEH->State(iKeys.keys[EKeySHOOT]) || 
		(iDynData->MouseOwner() == this && aEH->GetMouse().ChkButtons(CMouse::EMouseCheckModeLeft) ) )
	{ 
		iShootCount++;

		if (iCurrentWeapon->BulletType()!= EBulletFist)
			if (iLoadCounter == iCurrentWeapon->LoadingTime())
			{ 
				if (iBullets[iCurrentWeapon->BulletType()]>0)
				{
					Shoot(1);
					iWeaponInHand = true;
				}
				else 
				{
					SetSecondaryWeapon( 0 );				
					iLoadCounter2 = 0;
					iLoadCounter = 0;
					ChooseBestWeapon();
				}
			}
          
		if (iSecondaryWeapon!= 0)
			if (iSecondaryCounter == 0) 
				if (iLoadCounter2 == iSecondaryWeapon->LoadingTime())
				{ 
					if (iBullets[iSecondaryWeapon->BulletType()]>0) 
						Shoot(2);
					else 
					{
						SetSecondaryWeapon( 0 );
						iLoadCounter2 = 0;
				    }	
				}
	 
		if (iCurrentWeapon->BulletType() == EBulletFist) 
	        if (iLoadCounter>= iCurrentWeapon->LoadingTime())
			{  
				Shoot(1);
				KickBodyParts();
			}
	}
	else 
	{
		iShootCount = 0;
		iWeaponInHand = false;
	}
	
	if (!aEH->GetMouse().ChkButtons(CMouse::EMouseCheckModeRight)) 
		aEH->GetMouse().SetRightPressed(0);
    
	if (aEH->State(iKeys.keys[EKeySHIFT])  || 
		(iDynData->MouseOwner() == this && 
		aEH->GetMouse().ChkButtons(CMouse::EMouseCheckModeRight) && 
		!aEH->GetMouse().RightPressed() ) ) 
	{
		ChangeWeapon(false);
		aEH->State(iKeys.keys[EKeySHIFT]) = 0;

		if (iDynData->MouseOwner() == this) 
			aEH->GetMouse().SetRightPressed(1);
	}
	SELFTEST;
}

void CPlayer::Reset(const CRect<int>& aDrawRect)
{
	iNoDamage = false;
	iAnim = 0;
	iPushPower = 0;
	iBurning = 0;
	iDead = 0;

	int curWeapIndex = iGameData->TypeData()->ReverseMapWeapon( iCurrentWeapon );
	
	if (iWeapons[ curWeapIndex ]<=0)
	{
		SetCurrentWeapon( 0 );
		SetSecondaryWeapon( 0 );
	}

	if (iSecondaryWeapon != NULL)
	{
		int secWeapIndex = iGameData->TypeData()->ReverseMapWeapon( iSecondaryWeapon );

		if (iWeapons[ secWeapIndex ]<2)
		{
			SetSecondaryWeapon( 0 );
		}
	}
	iKick = 0;
	iHit = 0;
	iFire = 0;
	iFire2 = 0;
	SetAngle(0);
	InitScr( aDrawRect );
	iLoadCounter = 0;
	iLoadCounter2 = 0;
	
	iSpeed = KPlayerNormalSpeed;
	SELFTEST;
}

void CPlayer::TotalReset(const CRect<int>& aDrawRect)
{
	int a;
	
	iTargetSystem = false;
	iEnergy = 0;
	iShield = 0;
	iCash = 0;
	for(a = 1; a< EWeaponAmount; a++)
		iWeapons[a] = 0;

	for(a = 1; a< EBulletAmount;a++)
		iBullets[a] = 0;
	
	iWeapons[0] = 1;
	iBullets[0] = 1;

	Reset(aDrawRect);
 
	SELFTEST;
}

void CPlayer::Shoot(int hand) 
{
	SELFTEST;
	int r, a;
	const IWeaponType* weaponPtr = NULL;
	CCoord<int> rpos = iLocation;
	CCoord<int> ofs;
		
    if (hand == 1) 
	{
        iFire = 1; 
	    iLoadCounter = 0;
        weaponPtr = iCurrentWeapon;
		ofs.SetMovement(RealAngle(),8);
		ofs = ofs.AddMovement(RealAngle()+90,4);
        iSecondaryCounter = iCurrentWeapon->LoadingTime()/2;
    } 
    if (hand == 2)
	{
        iFire2 = 1; 
	    iLoadCounter2 = 0;
        weaponPtr = iSecondaryWeapon;
		ofs.SetMovement(RealAngle(),8);
		ofs = ofs.AddMovement(RealAngle()+270,4);
    }
	
	iDynData->Statistics()->PlayerStats( this ).LevelStats( iDynData->CurrentLevel() ).AddShot();

	for (a = 0;a<weaponPtr->BulletAmount();a++) 
	{
		if (iSecondaryWeapon == 0)
            r = (weaponPtr->AngleAdd()>> 1)-rand()%weaponPtr->AngleAdd();
		else
            r = weaponPtr->AngleAdd()- rand()%(2*weaponPtr->AngleAdd());
        iDynData->LevelRuntime()->NewBullet(weaponPtr->BulletType(),
					rpos.Add(14,14).AddMovement(RealAngle(),10), 
					RealAngle()+r, 
					0,
					1.0f/weaponPtr->BulletAmount(),
					this );
	}

	if (weaponPtr!= NULL)
	{
		if (weaponPtr->ConsumesBullets())
			iBullets[weaponPtr->BulletType()]--;

		if ( iShootCount % iGameData->TypeData()->iBulletType[weaponPtr->BulletType()]->Multiplier() == 0) 
		{
			iSP->PlayPosSample( weaponPtr->Sound(), KFullVolume, iLocation );

			if (weaponPtr->Smoke()) 
				iDynData->LevelRuntime()->NewEffect(EEffectGunSmoke, rpos.Add(14, 14).Add(ofs), RealAngle(), 1);
		}
	}
	SELFTEST;    
}

void CPlayer::Animate() 
{
	SELFTEST;
    iAnim = 1;
	if (iWalk) 
	{
		if (iWalk-1<KWalkAnimationDelay) 
			iWalk++;
		else 
			iWalk = 1;

		iAnim = (iWalk-1)/(KWalkAnimationDelay/4);
		if (iAnim>= 3) iAnim = 1;
	}
    if (! iFire && iWeaponInHand)
        iAnim = 3;
	if (iFire) 
	{
		iFire++;
		if (iFire>7) iFire = 0;
		iAnim = 4;
	}
	if (iFire2) 
	{
		iFire2++;
		if (iFire2>7) iFire2 = 0;
		iAnim = 5;
	}
	if (iDead) 
	{
		if (iDead-1<KDeadAnimationDelay) iDead++;
		iAnim = (int)(6+((float)(iDead-1)/KDeadAnimationDelay)*3);

        if (iAnim>8) 
			iAnim = 8;

		if (iDead-1 == KDeadAnimationDelay)
		{
			iObserver->CharacterDied(this);
		}
	}
	if (iKick) 
	{
		iKick++;
		if (iKick>6) 
			iKick = 0;
		if (iSecondaryWeapon == 0) 
			iAnim = 5; 
		else 
			iAnim = 6;
	}
	SELFTEST;
}

void CPlayer::ChangeWeapon(bool aReverse) 
{
	SELFTEST;
	int curWeapIndex = iGameData->TypeData()->ReverseMapWeapon(iCurrentWeapon);

	if (!aReverse)
	{
		if (iCurrentWeapon->SingleHand() && 
			iWeapons[curWeapIndex] == 2 && 
			iSecondaryWeapon == NULL)
			iSecondaryWeapon = iCurrentWeapon; 
		else 
			{
				curWeapIndex++;
				curWeapIndex%= EWeaponAmount;
				SetSecondaryWeapon(0);
			}
		while (iWeapons[curWeapIndex%(EWeaponAmount)] == 0) 
		{
			curWeapIndex++;
			curWeapIndex%= EWeaponAmount;
		}
		SetCurrentWeapon(curWeapIndex);
	}
	else
	{
		if (iSecondaryWeapon!= NULL)
			SetSecondaryWeapon(0);
		else
		{
			curWeapIndex--;
			if (curWeapIndex<0)
				curWeapIndex+=EWeaponAmount;

			while (iWeapons[curWeapIndex%(EWeaponAmount)] == 0) 
			{
				curWeapIndex--;
				curWeapIndex%= EWeaponAmount;
			}

			SetCurrentWeapon(curWeapIndex);

			if (iCurrentWeapon->SingleHand()&&
				iWeapons[curWeapIndex] == 2)
				SetSecondaryWeapon(curWeapIndex);
		}
	}

    iLoadCounter = 0;
	iLoadCounter2 = 0;
	SELFTEST;
}

void CPlayer::Footprint(int side) 
{
	SELFTEST;
	CCoord<int> footpos;
	
	footpos = iLocation.Add(15,15).AddMovement(RealAngle()+90+(side*180),4);

	if (iDynData->LevelRuntime()->Level().LevelData(footpos.Div(KBlockSpriteSize)).iType == EBlockTypeFloor) 
		if ( iDynData->LevelRuntime()->Level().DustBlock(footpos.Div(KBlockSpriteSize)) )
		{
			iDynData->LevelRuntime()->NewEffect(EEffectFootprint, footpos, 0, 0);
			return;
		}
	SELFTEST;
}

void CPlayer::Move(int aAngle, float aSpeed, int aIndex, const CRect<int>& aDrawRect ) 
{
	SELFTEST;
	// Generate Blood effect
	if (iEnergy<20&&
		iWalk%4 == 0&&
		rand()%3 == 0) 
	{
		iDynData->LevelRuntime()->NewEffect(EEffectBloodOnGround, iLocation.Add(15,15), 0, 0);
	}

	CCharacter::Move(aAngle,aSpeed,iDynData);

	PickupCrates( aDrawRect );
	SELFTEST;
}

void CPlayer::PickupCrates( const CRect<int>& aDrawRect )
{
	SELFTEST;
	std::list< CCrate* >::iterator iter = iDynData->LevelRuntime()->Crates().begin();

	if ( iDead != 0 ) // dead men don't collect
		return;

	while(iter != iDynData->LevelRuntime()->Crates().end())
	{
		if ((*iter)->InUse()&& 
			(*iter)->TextCounter() == 0&&
			(*iter)->Location().X()+7>iLocation.X()&& 
			(*iter)->Location().X()+7<iLocation.X() + KCharacterSpriteSize&& 
			(*iter)->Location().Y()+7>iLocation.Y()&& 
			(*iter)->Location().Y()+7<iLocation.Y() + KCharacterSpriteSize) 
		{
			(*iter)->TryPickup( this, aDrawRect );
		}
		iter++;
	}
	SELFTEST;
}

void CPlayer::SetAngle(float aAngle)
{
	iAngle = aAngle;
	ANGLEFIX(iAngle);
	iTargetAngle = int(iAngle);
}

void CPlayer::Straighten()
{
	SELFTEST;
	//automatic iAngle straightener (0,90,180,270)
	if (iAngle>= 360-KStraightTolerance || iAngle<= KStraightTolerance) 
	{
		SetAngle(0);
		return;
	}
	if (iAngle>= 90-KStraightTolerance && iAngle<= 90+KStraightTolerance)  
	{
		SetAngle(90);
		return;
	}
	if (iAngle>= 180-KStraightTolerance && iAngle<= 180+KStraightTolerance)  
	{
		SetAngle(180);
		return;
	}
	if (iAngle>= 270-KStraightTolerance && iAngle<= 270+KStraightTolerance)  
	{
		SetAngle(270);
		return;
	}
	SELFTEST;
}

void CPlayer::Rotate(float aChange) 
{	
	SELFTEST;
	SetAngle(iAngle+aChange);
	SELFTEST;
}

void CPlayer::InitScr(const CRect<int>& aDrawRect)
{
	SELFTEST;
	iScreenPos.Set(iLocation.Sub(aDrawRect.Size().Div(2)));
	SELFTEST;
}

void CPlayer::MoveScr(const CRect<int>& aDrawRect)
{
	SELFTEST;
	int spd;

	if (abs((iScreenPos.X()+aDrawRect.Width()/2)-(iLocation.X()+14))>100) 
		iScreenPos.SetX(int(iLocation.X()-aDrawRect.Width()/2));
	spd = int(abs((iScreenPos.X()+aDrawRect.Width()/2)-((35*CMath::Static()->Cose(RealAngle()))+iLocation.X()+14))/4);
	if ((iScreenPos.X()+aDrawRect.Width()/2)<(25*CMath::Static()->Cose(RealAngle()))+iLocation.X()+14) 
#ifndef __LINUX__
		iScreenPos = iScreenPos.AddX((float)spd);
#else
		iScreenPos = iScreenPos.AddX(spd);
#endif
	if ((iScreenPos.X()+aDrawRect.Width()/2)>(25*CMath::Static()->Cose(RealAngle()))+iLocation.X()+14) 
#ifndef __LINUX__
		iScreenPos = iScreenPos.AddX((float)-spd);
#else
		iScreenPos = iScreenPos.AddX(-spd);
#endif

	spd = int(abs((iScreenPos.Y()+aDrawRect.Height()/2)-((25*CMath::Static()->Sine(RealAngle()))+iLocation.Y()+14))/4);
	if (iScreenPos.Y()+aDrawRect.Height()/2<(25*CMath::Static()->Sine(RealAngle()))+iLocation.Y()+14) 
#ifndef __LINUX__
		iScreenPos = iScreenPos.AddY((float)spd);
#else
		iScreenPos = iScreenPos.AddY(spd);
#endif
	if (iScreenPos.Y()+aDrawRect.Height()/2>(25*CMath::Static()->Sine(RealAngle()))+iLocation.Y()+14) 
#ifndef __LINUX__
		iScreenPos = iScreenPos.AddY((float)-spd);
#else
		iScreenPos = iScreenPos.AddY(-spd);
#endif
    if (abs((iScreenPos.Y()+aDrawRect.Height()/2)-(iLocation.Y()+14))>100) 
		iScreenPos.SetY(int(iLocation.Y()-(aDrawRect.Height()/2)));
	SELFTEST;
}


bool CPlayer::GetDamage(float damage, IBulletObserver *aObserver) 
{   
	SELFTEST;
	int a;

	if ( iDead )
		return true;

	if ( iNoDamage ) // for debugging only
		return false;

	iHit = 15;
	iEnergy-= damage;

	if (iEnergy<= 0) 
	{
		iEnergy = 0;

		if (iDynData->GameMode()->Frags() && aObserver != NULL )
		{
			aObserver->BulletPlayerKill( this );
		}

		if (iBurning) 
		{
			iSP->PlaySample(iDynData->LevelRuntime()->IngameData()->SoundFX( ESoundEffectWoosh ), KFullVolume, PAN_MIDDLE);
			iDynData->LevelRuntime()->NewEffect(EEffectExplo, iLocation.Add(12,12), 0, 0);
			iDynData->LevelRuntime()->NewEffect(EEffectExplo, iLocation.Add(18,12), 0, 0);
			iDynData->LevelRuntime()->NewEffect(EEffectExplo, iLocation.Add(15,18), 0, 0);
		}
		else 
		{
			for (a = 0;a<16;a++) 
				if (rand()%2) 
					iDynData->LevelRuntime()->NewEffect(EEffectBlood,iLocation.Add(15,15), int(a*22.5f), 2);
			iSP->PlaySample(iDynData->LevelRuntime()->IngameData()->SoundFX( ESoundEffectDeath ), KFullVolume, PAN_MIDDLE);
		}

		iDynData->Statistics()->PlayerStats( this ).LevelStats( iDynData->CurrentLevel() ).iOwnDeaths++;
		iDynData->Statistics()->PlayerStats( this ).iLevelOfDeath = iDynData->CurrentLevel();

		iDead = 1;

		SELFTEST;
		return true;
	}
	SELFTEST;
	return false;
}

void CPlayer::ShopBuy(int aAmount, CDynShopData* aDSD)
{
	SELFTEST;
	for (int a = 0;a<aAmount;a++)
	{
		if (aDSD->Cat() == 0)
		{
			if (iGameData->TypeData()->iWeaponType[aDSD->Item()+1]->Cost()> iCash)
				return;

			if (iWeapons[aDSD->Item()+1]<iGameData->TypeData()->iWeaponType[aDSD->Item()+1]->SingleHand()+1)
			{
				iCash-= iGameData->TypeData()->iWeaponType[aDSD->Item()+1]->Cost();
				iWeapons[aDSD->Item()+1]++;
			}
			else return;
		}

		if (aDSD->Cat() == 1)
		{
			IBulletType* bulletType = iGameData->TypeData()->iBulletType[aDSD->Item()+1];

			if (bulletType->Cost() > iCash)
				return;

			if ( iBullets[aDSD->Item()+1] < 
				 bulletType->MaximumForPlayer()*
				  bulletType->Multiplier() )
			{
				iCash -= bulletType->Cost();
				iBullets[aDSD->Item()+1]+= bulletType->Multiplier();
			}
			else return;
		}

		if (aDSD->Cat() == 2&&aDSD->Item() == 0)
		{
			if (ShieldPrice()>iCash)
				return;

			if (iShield<iGameData->TypeData()->iShieldType->Maximum())
			{
				iCash -= ShieldPrice();
				iShield++;				
			}
			else return;
				
		}

		if (aDSD->Cat() == 2 && 
			aDSD->Item() == 1 )
		{
			if (aDSD->iTargetPrice>iCash)
				return;

			if (!iTargetSystem)
			{
				iCash-= aDSD->iTargetPrice;
				iTargetSystem = true;
			}
			else return;
		}
	}
	SELFTEST;
}

void CPlayer::ShopSell(int aAmount,CDynShopData* aDSD)
{
	SELFTEST;
	int a;

	for (a = 0;a<aAmount;a++)
	{
		if (aDSD->Cat() == 0)
		{
			if (iWeapons[aDSD->Item()+1])
			{
				iCash+= aDSD->iWeaponSellPrice[aDSD->Item()];
				iWeapons[aDSD->Item()+1]--;
			}
			else return;
		}

		if (aDSD->Cat() == 1)
		{
			if (iBullets[aDSD->Item()+1]>0)
			{
				IBulletType* bulletType = iGameData->TypeData()->iBulletType[aDSD->Item()+1];

				iCash += bulletType->Cost();
				iBullets[aDSD->Item()+1] -= bulletType->Multiplier();
				if (iBullets[aDSD->Item()+1]<0) 
					iBullets[aDSD->Item()+1] = 0;
			}
			else return;
		}

		if (aDSD->Cat() == 2&&aDSD->Item() == 0)
		{
			if (iShield)
			{
				iCash+= ShieldPrice()/2;
				iShield--;				
			}
			else return;
		}

		if (aDSD->Cat() == 2&&aDSD->Item() == 1)
		{
			if (iTargetSystem)
			{
				iCash+= aDSD->iTargetPrice;
				iTargetSystem = false;
			}
			else return;
		}
	}
	SELFTEST;
}

const CCoord<int>& CPlayer::ScreenPos() const
{ 
	return iScreenPos; 
}


int CPlayer::ShieldPrice()
{
	return (iGameData->TypeData()->iShieldType->Cost()+iShield*15);
}

int CPlayer::Color() const
{
	return iColor;
}

void CPlayer::SetColorByIndex(int aColor)
{
	ASSERT( aColor >= 0 );
	iColor = KPlayerColor[ aColor % (sizeof KPlayerColor / sizeof KPlayerColor[0]) ];
}

void CPlayer::SetName( const char* aName )
{
	delete[] iName;
	iName = new char[ strlen(aName)+1 ];
	strcpy(iName, aName);
}

char* CPlayer::Name()
{
	return iName;
}

void CPlayer::SetSpeed( float aSpeed )
{
	ASSERT( aSpeed > 0 );
	ASSERT( aSpeed < 10 ); // sanity check
	iSpeed = aSpeed;
}

void CPlayer::BulletHit( float aShare )
{
	iDynData->Statistics()->PlayerStats( this ).LevelStats( iDynData->CurrentLevel() ).AddHit( aShare );
}

void CPlayer::BulletPlayerKill( CPlayer* aPlayer )
{
	char buf[KMaxMessagesLengthOnBoard];

	if (aPlayer!= this)		
		ASSERT(_snprintf(buf,KMaxMessagesLengthOnBoard,iDynData->LevelRuntime()->IngameData()->RandomKillText(),iName,aPlayer->iName)>0);
	if (aPlayer == this) // has to be this way. Some problems with ASSERT macro
		ASSERT(_snprintf(buf,KMaxMessagesLengthOnBoard,iDynData->LevelRuntime()->IngameData()->RandomSuicideText(),iName)>0);

	iGS->GUIStateController()->MessageBoard()->AddMessage(buf);
	
	iDynData->Statistics()->PlayerStats( this ).LevelStats( iDynData->CurrentLevel() ).
		AddPlayerKill( aPlayer );
}

void CPlayer::BulletEnemyKill( CEnemy* aEnemy )
{
	iDynData->Statistics()->PlayerStats( this ).LevelStats( iDynData->CurrentLevel() ).iEnemyKills
		[ iGameData->TypeData()->ReverseMapEnemy(aEnemy->Type()) ]++;}

TEST( CPlayer )
{
	ASSERT( EF_DEBUG || !iNoDamage );
	ASSERTPTR(	this );
	ASSERT( iShield >= 0);
	ASSERT( iShield <= 1000 ); // just some limit
	ASSERT( iEnergy >= 0);
	ASSERT( iKick   <= 6);
	ASSERT( iKick   >= 0);
	ASSERT( iCash   >= 0);
	ASSERT( iName   != NULL );
	ASSERT( iSpeed  >  0);
	
	for (int a=0;a<EWeaponAmount;a++)
	{
		ASSERT( iWeapons[a]>=0 );
		ASSERT( iWeapons[a]<=2 );
	}

	bool test = false;
	for (int a=0;a<(sizeof KPlayerColor / sizeof KPlayerColor[0]);a++)
	{
		if ( iColor == KPlayerColor[ a ] )
			test = true;
	}
	ASSERT( test );
}

