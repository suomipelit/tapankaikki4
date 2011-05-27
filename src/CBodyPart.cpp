#include "common/CMath.h"
#include "common/Types.h"
#include "common/IBodyPartType.h"

#include "CBodyPart.h"
#include "CIngameData.h"
#include "CSoundPlayer.h"
#include "CLevelRuntimeData.h"

void CBodyPart::Bounce(enum TDirection aSide)
{
	ASSERT(aSide>=EUp&&aSide<=ERight);

	if (aSide == EDown||aSide == EUp) iAngle=360-iAngle; // up&down
		else if (aSide == ERight||aSide == ELeft) iAngle=270+(270-iAngle); // left & right
		else ASSERT(0);

	iSP->PlayPosSample(iType->Sound(), KFullVolume, iLocation);
}

void CBodyPart::Push(CCoord<float> aVec)
{
	aVec.AddMovement(iAngle,iSpeed);
	SetAngleAndSpeed( aVec );
}

void CBodyPart::SetAngleAndSpeed(const CCoord<float>& aVec)
{
	iSpeed=aVec.VectorLength();
	if ( iSpeed > 5 )
		iSpeed = 5;

	iAngle=int(aVec.VectorAngle());
}

int CBodyPart::SpriteOffset()
{
	return iType->SpriteOffset();
}

void CBodyPart::Move( CLevelRuntimeData* aLevelRuntimeData ) 
{
	int reuna=4;  //**chk pointit on n„in
	int reuna2=2; //**
				  //**
	CCoord<float> newpos;
	CCoord<float> vec;
	CCoord<int> testpos;

	if (iSpeed>0)
	{
		vec.SetMovement(iAngle,iSpeed);
		
		newpos=iLocation.Add(vec);
		testpos=newpos;

		if (iType->DropBlood()) 
		 if (rand()%2 != 0) 
		  aLevelRuntimeData->NewEffect( EEffectBloodOnGround, iLocation.Add(10,10), 0, 0);

		if (newpos.Y()<iLocation.Y())
		{
		 if (aLevelRuntimeData->Level().LevelData((testpos.X()+10-reuna2)/20,(testpos.Y()+reuna)/20).iType==EBlockTypeFloor && 
			  aLevelRuntimeData->Level().LevelData((testpos.X()+10+reuna2)/20,(testpos.Y()+reuna)/20).iType==EBlockTypeFloor)
				iLocation.SetY(newpos.Y());
			  else
				  if (iType->Bounce())
					  Bounce(EDown);
				  else
				  {
					  vec.SetY(0);
					  SetAngleAndSpeed(vec);
				  }
		}
		else
		 if (newpos.Y()>iLocation.Y())
		 {
	     if( aLevelRuntimeData->Level().LevelData((testpos.X()+10-reuna2)/20,(testpos.Y()+20-reuna)/20).iType==EBlockTypeFloor && 
		         aLevelRuntimeData->Level().LevelData((testpos.X()+10+reuna2)/20,(testpos.Y()+20-reuna)/20).iType==EBlockTypeFloor) 
		         iLocation.SetY(newpos.Y());
			  else
				  if (iType->Bounce())
					  Bounce(EUp);
				  else
				  {
					  vec.SetY(0);
					  SetAngleAndSpeed(vec);
				  }
		}
		 
		 if (newpos.X()<iLocation.X())
		{
		 if (aLevelRuntimeData->Level().LevelData((testpos.X()+reuna) /20,(testpos.Y()+10-reuna2) /20).iType==EBlockTypeFloor &&
		      aLevelRuntimeData->Level().LevelData((testpos.X()+reuna) /20,(testpos.Y()+10+reuna2) /20).iType==EBlockTypeFloor) 
		        iLocation.SetX(newpos.X());
			  else
				  if (iType->Bounce())
					  Bounce(ERight);
				  else
				  {
					  vec.SetX(0);
					  SetAngleAndSpeed(vec);
				  }
		}
		else
		if (newpos.X()>iLocation.X())
		{
		   if (aLevelRuntimeData->Level().LevelData((testpos.X()+20-reuna) /20,(testpos.Y()+10-reuna2) /20).iType==EBlockTypeFloor && 
		       aLevelRuntimeData->Level().LevelData((testpos.X()+20-reuna) /20,(testpos.Y()+10+reuna2) /20).iType==EBlockTypeFloor) 
		         iLocation.SetX(newpos.X());
		   else		
			   if (iType->Bounce())
				   Bounce(ELeft);
				else
				{
					vec.SetX(0);
					SetAngleAndSpeed(vec);
				}
		}
		iSpeed += iType->Slow();
	}
}

CBodyPart::CBodyPart(CCoord<float> aPos, int aAngle,float aSpeed, IBodyPartType* aType,CSoundPlayer* aSP)
{
	iLocation.Set(aPos);

	iAngle=aAngle;
	iSpeed=aSpeed;

	iType=aType;
	iSP=aSP;

	ASSERTPTR(iType);
	ASSERTPTR(iSP);
	ASSERTPTR(CMath::Static());
}

void CBodyPart::Kick(int aAngle)
{
	iAngle=aAngle;
	iSpeed=iType->Kickspeed();
}
