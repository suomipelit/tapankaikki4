#include <string.h>
#include <stdlib.h>

#include "CBullet.h"

#include "common/CMath.h"
#include "common/IWeaponType.h"

#include "CSoundPlayer.h"
#include "CGameApp.h"
#include "CDynGameData.h"
#include "CLevelRuntimeData.h"

CBullet::CBullet(
	IBulletType* aType,
	CCoord<float> aPos,
	IBulletObserver* aObserver,
	int aAngle,
	int aTime,
	float aShareOfOriginalShot,
	CDynGameData* aDynData,
	CSoundPlayer* aSP,
	CGameData* aGM)
{
	ASSERT( aShareOfOriginalShot > 0 );
	ASSERT( aShareOfOriginalShot <= 1 );

	iSP=aSP;
	iGM=aGM;
	iLocation.Set(aPos);

	iDynData=aDynData;

	iType = aType;
	iOwner = aObserver;
	iShareOfOriginalShot = aShareOfOriginalShot;
	iSpeed = iType->Speed();

	ANGLEFIX( aAngle );
	iAngle = aAngle;
	iSpriteAngle = aAngle;
	iSpriteAngleAdd = 0;
	iTime = iType->Time()+aTime;
	iCounter = 0;
	iHitReported = false;
}

bool CBullet::CheckWalls(TDirection aMode, const CCoord<int>& aNewLoc)
{
	CCoord<int> ofs = aNewLoc.Div(KBlockSpriteSize);
	
	switch(aMode)
	{
	case EUp:
		if (iDynData->LevelRuntime()->Level().Collision(ofs.SubY(1)))
			return true;
		break;
	case ELeft:
		if (iDynData->LevelRuntime()->Level().Collision(ofs.SubX(1))) 
			return true;
		break;
	case EDown:
		if (iDynData->LevelRuntime()->Level().Collision(ofs.AddY(1))) 
			return true;
		break;
	case ERight:
		if (iDynData->LevelRuntime()->Level().Collision(ofs.AddX(1))) 
			return true;
		break;
	}
	return false;
}

void CBullet::Bounce(IBulletType::TBounceType aBounceType, const CCoord<int>& aNewLoc)
{
	const int KIso = 10000;
	int min;
	int distUp=KIso,distLeft=KIso,distDown=KIso,distRight=KIso;

	DEBUG_ASSERT( iType->BounceType() != IBulletType::KBounceTypeNone );
 
	if (CheckWalls(EUp, aNewLoc))
		distUp=int(aNewLoc.Y()) % KBlockSpriteSize;
			
	if (CheckWalls(ELeft, aNewLoc))
		distLeft=int(aNewLoc.X()) % KBlockSpriteSize;
 
	if (CheckWalls(EDown, aNewLoc))
		distDown=KBlockSpriteSize-int(aNewLoc.Y()) % KBlockSpriteSize;
 
	if (CheckWalls(ERight, aNewLoc))
		distRight=KBlockSpriteSize-int(aNewLoc.X()) % KBlockSpriteSize;
 
	min=distUp;
	if (distLeft<min) min=distLeft;
	if (distDown<min) min=distDown;
	if (distRight<min) min=distRight;
 		
	if ( distUp!=KIso ||
		 distLeft!=KIso || 
		 distDown!=KIso ||
		 distRight!=KIso )
	{

		if ( iType->BounceType() == IBulletType::KBounceTypeNormal )
		{
			if (min==distUp||min==distDown) 
				iAngle=360-iAngle;// up & down
			if (min==distLeft||min==distRight) 
				iAngle=180-iAngle;//right & left
		}
		else
		{
			CCoord<float> tmp;
			tmp.SetMovement( iAngle, iSpeed );

			if (min==distUp||min==distDown)
				tmp.SetY( 0 );

			if (min==distLeft||min==distRight) 
				tmp.SetX( 0 );

			iAngle = int(tmp.VectorAngle());
			iSpeed = tmp.VectorLength();
		}
        
		ANGLEFIX(iAngle);

		if (iAngle<180)
			iSpriteAngleAdd=-iSpeed*15; 
		else 
			iSpriteAngleAdd=iSpeed*15; 
	} 
	else 
		iSpeed=0;
}

int CBullet::Move() 
{
	int a;
	unsigned int b;
	bool hit=false, hitwall=false;
	CCoord<int> rpos(iLocation);
	CCoord<int> vpos(0,0);
	int loopTimes;
	float bp;
	int alive=1;

    loopTimes=(int)iSpeed >> 2;
	if ( loopTimes < 1 ) 
		loopTimes=1;
	iTime--;
	iCounter++;

	// Spinning sprite update (grenade etc.)
    if ( iType->TimeBeforeSpin() > 0 )  // check if bullet is to be spinned (grenade)
		if ( iCounter == iType->TimeBeforeSpin() ) // if time is up then spin
		{
			if (iAngle<180)
				iSpriteAngleAdd = -iSpeed * 15; 
			else 
				iSpriteAngleAdd = iSpeed * 15; 
		}

	// Check for collision
    for(a=0;a<loopTimes && !hit && !hitwall;a++) 
	{
		if ( iSpeed > 0.001f ) 
		{
			CCoord<float> oldPos = iLocation;
			CCoord<float> proposal = iLocation.AddMovement(iAngle,(float)iSpeed/loopTimes);
			CCoord<float> newPos;
			DEBUG_ASSERT( proposal != iLocation ); // This should not be possible
			if ( !iDynData->LevelRuntime()->Level().VisibilityCheck( iLocation,proposal,&newPos ) )
			{
				if ( iType->CollisionTrigger() )
				{
					hitwall=true;
				}
				else 
				{
					 // I don't know what to do in this situation
					ASSERT( iType->BounceType() != IBulletType::KBounceTypeNone );
					Bounce( iType->BounceType(), newPos );
					newPos = iLocation.AddMovement(iAngle,(float)iSpeed/loopTimes); // bounce changes direction

					iSP->PlayPosSample(iType->BounceSound(), KFullVolume, iLocation);
				}
			}
	
			iLocation = newPos;
			rpos = iLocation;
			//DEBUG_ASSERT( !iDynData->LevelRuntime()->Level().Collision( rpos.Div( KBlockSpriteSize ) ) );
		}

		// Check collision with enemies & players
		if (iCounter>iType->SleepTime() && !hitwall) 
		{
			if (!iDynData->NetworkMode()==ENetworkModeClient)
			{
				std::vector< CEnemy >::iterator iter=iDynData->LevelRuntime()->Enemies().begin();

				while(iter != iDynData->LevelRuntime()->Enemies().end())
				{	
					CRect<int> limit;
					limit.Set(iter->Location().Add(5,5),iter->Location().Add(22,22));

					// Collision square
					if (limit.Inside(CCoord<int>(iLocation)) &&
						// Let's keep enemy from flaming or beating himself 
						(iOwner != static_cast<IBulletObserver*>(&*iter) || 
						(	iType->Type() != IBulletType::EBulletTypeIncendiary && 
							iType->Type() != IBulletType::EBulletTypeFist )
						)
						)
					{
						hit=true;
						// enemy is interested about bullets
						iter->GotoBulletOrigin(this);

						vpos = iter->Location().Add(14,14).AddMovement(iAngle,4);
						iter->iPushAngle=iAngle;// äijä lentää
						if (iType->PushPower()>iter->iPushPower) 
							iter->iPushPower=(float) iType->PushPower()*0.75f;// äijä lentää

						if (iType->Type()==IBulletType::EBulletTypeIncendiary) 
						{
							iter->iBurning += 15;
							iter->iFryingObserver=iOwner;
						}

						bool dead=false;
						if (iType->Power()>0) 
							dead = iter->GetDamage(iType->Power(), iOwner);

						if (dead)
						{
							iter = iDynData->LevelRuntime()->KillEnemy( iter );
						}
						else
							++iter;
					}
					else
						++iter;
				}
			}

			for (b=0;b<iDynData->Players().size();b++)
			{
				CRect<int> limit;
				limit.Set(CCoord<int>(iDynData->Players()[b]->Location()).Add(5,5),
						CCoord<int>(iDynData->Players()[b]->Location()).Add(22,22));

				if (!iDynData->Players()[b]->iDead&&
					limit.Inside(rpos) && 
					// Let's keep player from flaming or beating himself 
					(iOwner != static_cast<IBulletObserver*>(iDynData->Players()[b]) || 
						( iType->Type() != IBulletType::EBulletTypeIncendiary && 
						  iType->Type() != IBulletType::EBulletTypeFist )
						)
					)
				{
					hit=true;
					vpos = iDynData->Players()[b]->Location().Add(14,14).AddMovement(iAngle,4);
						
					if ((iDynData->NetworkMode()==ENetworkModeNone))// || (iDynData->LocalPlayer(0)==iDynData->Players()[b]))
					{
						iDynData->Players()[b]->iPushAngle=iAngle;// „ij„lent„„
						if (iType->PushPower()>iDynData->Players()[b]->iPushPower) 
							iDynData->Players()[b]->iPushPower=(float) iType->PushPower()*0.75f;// „ij„lent„„
						if (iType->Type()==IBulletType::EBulletTypeIncendiary) 
						{
							iDynData->Players()[b]->iBurning += 15;
							iDynData->Players()[b]->iFryingObserver=iOwner;
						}

						if (iType->Power()>0) 
							iDynData->Players()[b]->GetDamage(iType->Power(), iOwner);

					}
				}
			}
		}
	}

	// tämä tietysti on vähän kyseenalaista...
	if (iType->Type()==IBulletType::EBulletTypeNormal||
		iType->Type()==IBulletType::EBulletTypeFist) 
		if (hitwall) 
			if (rand()%8==1) 
				iDynData->LevelRuntime()->NewEffect(EEffectMetal, rpos, rand()%360, (rand()%2) +2);

	if ( hit ) 
	{
		if ( iType->Stop() ) 
			iSpeed=0;

		if (iType->Type()==IBulletType::EBulletTypeNormal||
			iType->Type()==IBulletType::EBulletTypeFist) 
		{
			bp=0.5f+(iType->Power() / 2) +rand()%2;
			if (bp>4) bp=4;
				for (a=0;a<(iType->Power() / 5) +1;a++) 
					iDynData->LevelRuntime()->NewEffect(EEffectBlood, vpos.Add((rand()%6) -3, (rand()%6) -3), iAngle+((rand()%30) -15), bp);
		}

		// add to statistics
		if ( iOwner != NULL && iType->Power()>0 && !iHitReported )
		{
			iOwner->BulletHit( iShareOfOriginalShot );
			iHitReported = true;
		}

		if (iType->Type()==IBulletType::EBulletTypeFist)
		{
			iSP->PlayPosSample( iGM->TypeData()->iWeaponType[0]->Sound(), KFullVolume,rpos);
		}
	}

	if ((iType->Type()==IBulletType::EBulletTypeNormal||
		 iType->Type()==IBulletType::EBulletTypeFist)&&
		(iTime==0||hitwall) && !hit &&
		iType->Speed()>10 )
		iDynData->LevelRuntime()->NewEffect(EEffectSmoke, rpos, 0, 0);

    if ((iType->CollisionTrigger() && (hit || hitwall)) || iTime==0) 
	{
        if (iType->Type()==IBulletType::EBulletTypeExplosive)	
			iDynData->Explo(rpos.AddMovement(iAngle,-1), iType->Explosion(), iShareOfOriginalShot, iOwner );
	
		alive=0;
	}

	if (iType->DoEffect( iCounter ))
		iDynData->LevelRuntime()->NewEffect( iType->Effect(), rpos, iType->EffectAngle( iAngle ), iType->EffectSpeed() );
	if (iType->DoEffect2( iCounter ))
		iDynData->LevelRuntime()->NewEffect( iType->Effect2(), rpos, iType->Effect2Angle( iAngle ), iType->Effect2Speed() );
	
	if (iSpeed>0) 
		iSpeed+=iType->Slow();
    if (iSpeed<0) 
		iSpeed=0; 

    iSpriteAngle+=iSpriteAngleAdd;
	ANGLEFIX(iSpriteAngle);

    if (iSpriteAngleAdd>0) 
		iSpriteAngleAdd-=0.4f;
    if (iSpriteAngleAdd<0) 
		iSpriteAngleAdd+=0.4f;
    if (iSpriteAngleAdd>=-0.4f && iSpriteAngleAdd<=0.4f) 
		iSpriteAngleAdd=0;

	if (!alive) return 1;
	return 0;
}

