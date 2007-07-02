#include <stdlib.h>

#include "CEnemy.h"

#include "common/CMath.h"
#include "CGameSystem.h"
#include "CSoundPlayer.h"
#include "IGUIStateController.h"
#include "CDynGameData.h"
#include "CBullet.h"
#include "CLevelRuntimeData.h"

void CEnemy::Animate() 
{
	iAnim=1;
	if (iWalk) 
	{
		if (iWalk-1<KWalkAnimationDelay) 
			iWalk++;
		else 
			iWalk=1;

		iAnim=(iWalk-1)/(KWalkAnimationDelay/4);
		if (iAnim >=3) 
			iAnim=1 ;
	}
	if (iFire) 
	{
		iFire++;
		if (iFire>6) 
			iFire=0;
		iAnim=3;
	}
	if (iEnergy<10) 
		iAnim+=4;
}

void CEnemy::CheckPlayerVisibility() 
{
	int dist, b, min, maximum, hit=0, go_on=0;
	bool saw[KMaxPlayers];
	int closest=0;
	bool seeSome=false;

	min=int(iAngle)-90;
	ANGLEFIX(min);
	maximum=int(iAngle)+90;
	ANGLEFIX(maximum);

	if ( iState != EStill && !iType->Hostile() )
		return; // in this case we don't need to look at players
	
	for (b=0;b<iDynData->Players().size();b++) 
	{
		iSeePlayer[b]=0;

		if (iSeePlayer[b]) 
			saw[b]=1;
		else saw[b]=0;
		

		go_on=0;
		hit=0;
		if (!iDynData->Players()[b]->iDead) 
		{
			int ply_angle = int(CMath::GetAngle(Location(),iDynData->Players()[b]->Location()));
			if (iAngle<270 &&iAngle>90) 
			{
				// näkökenttä 180 astetta
				if (ply_angle>min &&ply_angle<maximum) 
					go_on=1;
			}
			else if (ply_angle>min ||ply_angle<maximum) 
					go_on=1;
		}

		// Visibility ray check ?
		if (go_on)
		{
			// visibility from center to center
			if (CMath::GetDist(
					iLocation.Add(14,14),
					iDynData->Players()[b]->Location().Add(14,14)) <
					iType->SightDistance())
				if (iDynData->LevelRuntime()->Level().VisibilityCheck(iLocation.Add(14,14),iDynData->Players()[b]->Location().Add(14,14) ))
				{
					iSeePlayer[b]=1;
					if (iType->Hostile())
					{
						seeSome=1;
						iState=EAssault; // Set enemy loose
					}
					else
					{
						iState=EWanderAround;
						return;
					}
				};

		}
	}

	if (seeSome) 
	{
		closest = -1;
		for (b=0;b<iDynData->Players().size()&&closest<0;b++) 
			if (!iDynData->Players()[b]->iDead) 
				if (iSeePlayer[b]) 
					closest=b;

		for (b=0;b<iDynData->Players().size();b++) 
			if (!iDynData->Players()[b]->iDead) 
				if (iSeePlayer[b]) 
				{
					if (CMath::GetDist(iDynData->Players()[b]->Location(),iLocation)<
						CMath::GetDist(iDynData->Players()[closest]->Location(),iLocation))
						closest=b;
				}
		
		iTargetAngle=int(CMath::GetAngle(Location(),iDynData->Players()[closest]->Location()));
	}
	else 
	{
		// Go to the last spot where seen someone
		for (b=0;b<iDynData->Players().size();b++) 
		if (saw[b]) 
		{
			dist=int(CMath::GetDist(Location().X(), Location().Y(), iDynData->Players()[b]->Location().X(), iDynData->Players()[b]->Location().Y()));// chk the dist and be clever
			iWalkCounter=int(dist/iType->Speed());
			if (iWalkCounter<0) iWalkCounter=0;
		}

		if (iState!=EStill)
			iState = EWanderAround;
	}
}

void CEnemy::Footprint(int side) 
{
	CCoord<int> footpos;

	footpos = iLocation.Add(15,15).AddMovement(((int)iAngle+90+(side*180))*4,4);

	if (iDynData->LevelRuntime()->Level().LevelData(footpos.Div(20)).iType==EBlockTypeFloor) 
		if ( iDynData->LevelRuntime()->Level().DustBlock(footpos.Div(20)) ) 
		{
			iDynData->LevelRuntime()->NewEffect(EEffectFootprint, footpos, 0, 0);
			return;
		}
}

void CEnemy::DecideAndMove()
{
	int b;

	if (iType->Hostile())
	{
		for(b=0; b<iDynData->Players().size() && iState!=EAssault; b++) 
			if (iSeePlayer[b]) 
				iState = EAssault;
	}

	if (iState == EWanderAround &&
		!iWalkCounter &&
		rand()%125==1) 
		iWalkCounter=(rand()%40)+20;

	if (iWalkCounter>0) 
	{
		Move(int(iAngle),0); 
		iWalkCounter--;
		if (iWalk==0) iWalk=1;
	}
	else iWalk=0;

	if (iTargetAngle==iAngle && 
		!iWalkCounter) 
	{
		if (iState==EWanderAround||
			iState==EStill) 
			if (rand() % 50==1) 
				iTargetAngle=rand() % 360;
	}
	TargetAngle( int(KEnemyRotationSpeed) );
}

void CEnemy::Move(int aAngle, float aSpeed) 
{
	float speedi;

	// valu verta
	if (iEnergy<10) 
	 if (iWalk%4==0) 
	  if (rand()%3==0) 
		iDynData->LevelRuntime()->NewEffect(EEffectBloodOnGround, iLocation.Add(15,15), 0, 0);

	if (aSpeed==0) 
		speedi=iType->Speed();
	else 
		speedi=aSpeed;

	CCharacter::Move(aAngle,speedi,iDynData);
}

bool CEnemy::GetDamage(float damage, IBulletObserver *aObserver) 
{
	int a;
	const int alku=rand()%7;
	float spd = 1 + iPushPower/1.5f;
	
	if (spd>4.5) 
		spd=4.5;
	iEnergy -=damage;
	if (iEnergy <=0) // =Death
	{
		if (iBurning && !iType->ExplosionDeath()) 
		{
			iSP->PlayPosSample(ESoundEffectWoosh, KFullVolume, iLocation);

			iDynData->LevelRuntime()->NewEffect(EEffectExplo, iLocation.Add(12,12), 0, 0);
			iDynData->LevelRuntime()->NewEffect(EEffectExplo, iLocation.Add(18,12), 0, 0);
			iDynData->LevelRuntime()->NewEffect(EEffectExplo, iLocation.Add(15,18), 0, 0);
			for (a=0;a<3;a++) 
				iDynData->LevelRuntime()->NewBodyPart(iLocation.Add(4,4), rand()%360, 2.5, a+EBodyPartBurnedChest);
		}
		if (!iBurning && !iType->ExplosionDeath()) 
		{
			for (a=0;a<16;a++) 
			 if (rand()%2) 
				 iDynData->LevelRuntime()->NewEffect(EEffectBlood, iLocation.Add(15,15), int(a*22.5), 2);
			for (a=0;a<3;a++) 
				iDynData->LevelRuntime()->NewBodyPart(iLocation.Add(4.0f,4.0f), rand()%360, spd+((rand()%2)*0.3f), (a+alku)%7);
			
			iSP->PlayPosSample(ESoundEffectDeath, KFullVolume, iLocation);
		}

		if (iType->ExplosionDeath())
		{
			iDynData->Explo(iLocation.Add(4,4), 2,1, aObserver );
			for (a=0;a<3;a++) 
				iDynData->LevelRuntime()->NewBodyPart(iLocation.Add(4,4), rand()%360, 3, a+EBodyPartBurnedChest);
		}

		if ( aObserver!=NULL )
		{
			aObserver->BulletEnemyKill( this );
		}

		return true;
	}
	return false;
}

void CEnemy::Shoot(int/*hand*/) 
{
	int r, a;
	iFire=1;
	iLoadCounter=0;

	for (a=0;a<iCurrentWeapon->BulletAmount();a++) 
	{
		r=(iCurrentWeapon->AngleAdd()/2) -
		   rand()%iCurrentWeapon->AngleAdd();
		iDynData->LevelRuntime()->NewBullet(iCurrentWeapon->BulletType(), 
							iLocation.Add(14,14).AddMovement(int(iAngle),10),
							(int)iAngle+r, 0,1.0f/CurrentWeapon()->BulletAmount(),
							this );
	}

	if (iCurrentWeapon>0) 
		if (iShootCount%iGM->TypeData()->iBulletType[iCurrentWeapon->BulletType()]->Multiplier()==0) 
		{
			iSP->PlayPosSample(iCurrentWeapon->Sound(), KFullVolume, iLocation);
			
			if (iCurrentWeapon->Smoke()) 
				iDynData->LevelRuntime()->NewEffect( EEffectGunSmoke, 
				iLocation.Add(14,14).AddMovement((int)(iAngle),8).AddMovement((int)(iAngle)+90,5), 
					int(iAngle), 1);
		}
}

void CEnemy::CheckForShoot() 
{
	int dist, a, wannashoot=0;
    
 	for (a=0;a<iDynData->Players().size();a++) 
	if (iSeePlayer[a] &&
		abs((int)(iTargetAngle-iAngle))<10) // jos kulma ja targetkulma ei eroa kun 10 astetta niin anti paukkua vaan je jes !!
		{
  			iWalkCounter=int(20/iType->Speed());
			dist=int( CMath::GetDist(
				Location().X()+14, 
				Location().Y()+14, 
				iDynData->Players()[a]->Location().X()+14, 
				iDynData->Players()[a]->Location().Y()+14) );// chk if close enough

			if (dist <= iGM->TypeData()->iBulletType[iCurrentWeapon->BulletType()]->ApproxDistance()+20)
			{
				iShootCount++;
				wannashoot=1;
				if (iLoadCounter==iCurrentWeapon->LoadingTime()) 
					Shoot(1);
			}
		}
    if (!wannashoot) 
		iShootCount=0;
}

CEnemy::CEnemy(int aType, bool aWaitStill, const CCoord<int>& aPos, int aAngle, CGameSystem* aGS,CGameData* aGameData,ICharacterObserver* aObserver): CCharacter(aObserver,aGameData)
{
	iGS=aGS;
	iPushPower=0;
	iBurning=0;

    if (aWaitStill)
		iState = EStill;
	else
		iState = EWanderAround;
	iAngle = iTargetAngle = aAngle;
	iLoadCounter=0;
	iWalk=0;
	iFire=0;
	iLocation.Set(aPos);	

	iGM=aGS->GUIStateController()->GameData();
	iSP=aGS->GUIStateController()->SoundPlayer();
	iDynData=aGS->DynData();

	ASSERT(CMath::Static());
	ASSERT(iGM);
	ASSERT(iSP);
	ASSERT(iDynData);

	for(int d=0; d<iDynData->Players().size(); d++) 
		iSeePlayer[d]=0;

	iWalkCounter=0;

	iType=iGM->TypeData()->iEnemyType[aType];
	SetCurrentWeapon(iType->CurrentWeapon());
	iEnergy=iType->Energy();
}

void CEnemy::TargetAngle(int aSpeed) 
{
    if (iAngle!= iTargetAngle) 
	{
		if (iAngle<180) 
		{
			if (iTargetAngle<iAngle+180 &&iTargetAngle>iAngle) 
				iAngle += aSpeed;
			else 
				iAngle -= aSpeed;
		}
		else
		{
			if (iTargetAngle>iAngle-180 &&iTargetAngle<iAngle)
				iAngle -= aSpeed;
			else 
				iAngle += aSpeed;
		}

		ANGLEFIX(iAngle);

 // 		vähän pyöristää, nin iAngle saavuttaa joskus iTargetAnglen ...
		if (fabs(iAngle-iTargetAngle) < aSpeed ||
			fabs((iAngle+360) - iTargetAngle) <aSpeed) 
			iAngle=iTargetAngle;
	}
}

void CEnemy::GotoBulletOrigin(CBullet* aBullet)
{
	if (iType->Hostile())
	{
		if (iState != EAssault)
		{
			iTargetAngle=aBullet->Angle()+180; // opposite to bullet direction
			ANGLEFIX(iTargetAngle);
			iWalkCounter=int(aBullet->Type()->ApproxDistance()/iType->Speed());
			iState=EAssault;
		}
	}
	else // run for your life...
	{
		iTargetAngle=aBullet->Angle(); 
		ANGLEFIX(iTargetAngle);
		iWalkCounter=int(aBullet->Type()->ApproxDistance()/iType->Speed());
		iState=EWanderAround;
	}
}

void CEnemy::SoundHeard(enum TIngameSoundEffect aEffect,float aVolume,CCoord<int> aPos)
{
	if (!iType->Hostile())
		return;

	if (iState == EAssault) // don't care about sounds if in the middle of man hunt ;)
		return;

	switch (aEffect)
	{
		case ESoundEffectPistol:
		case ESoundEffectShotgun:
		case ESoundEffectUzi:
		case ESoundEffectAutorifle:
		case ESoundEffectBazooka:
		case ESoundEffectHit:
		case ESoundEffectSmallExplosion:
		case ESoundEffectExplosion:
		case ESoundEffectBigExplosion:
		case ESoundEffectDeath:
		case ESoundEffectWoosh:
		case ESoundEffectFlame:
		case ESoundEffectMetalHit:
		case ESoundEffectGrenadeLauncher:
		case ESoundEffectKickHead:
			break; // continue execution

		case ESoundEffectClick:
			return;
	};

	// if closer than sight distance, then we can hear it too ;)
	// Enemy cannot hear around the corner
	if (CMath::GetDist(aPos,iLocation)<iType->SightDistance() && iDynData->LevelRuntime()->Level().VisibilityCheck(iLocation.Add(14,14), aPos) ) 
	{
		if (iState != EAssault)
		{
			iState=EAssault;
			iTargetAngle = int(CMath::GetAngle(iLocation,aPos));

			// goto 0.75 * shooting distance to sound source
			iWalkCounter = int( (CMath::GetDist(aPos,iLocation)-
				(iGM->TypeData()->iBulletType[iCurrentWeapon->BulletType()]->
				 ApproxDistance())*0.75)/iType->Speed() );

			if (iWalkCounter<0)
				iWalkCounter=0;
		}
	}
}

void CEnemy::EnemyCheck()
{
	CheckPlayerVisibility();
	if (Type()->Hostile()) 
	{
		CheckForShoot();
		LoadWeapons();
	}
}
