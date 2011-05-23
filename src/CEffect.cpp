#include "CEffect.h"
#include "CLevelRuntimeData.h"
#include "common/IEffectType.h"
#include "common/CMath.h"

CEffect::CEffect(IEffectType* aType,const CCoord<float>aPos,int aCount, int aAngle, float aSpeed)
{
	ASSERTPTR(aType);

	iType=aType;
	iLocation.Set(aPos);
	iCounter=aCount;
	iVec.SetMovement(aAngle,aSpeed);
	SELFTEST;
}

int CEffect::Roll(CLevelRuntimeData* aLevelRuntime, int aAmountOfEffects) 
{
	int die=0;
	int mul=1;

	SELFTEST;

	// There is overload of effects, have to compensate
	if (aAmountOfEffects>KLightOverloadLimit)
	{
		if (aAmountOfEffects>KHeavyOverloadLimit)
			mul=iType->HeavyOverloadMul();
		else
			mul=iType->LightOverloadMul();
	}

	if (iCounter<iType->TCount()*iType->Delay()) 
		if (!iType->Random()) 
			iCounter+=mul;
		else 
			iCounter+=rand()%(mul*3);

	if (iCounter>=iType->TCount()*iType->Delay()) 
		die=1;

	if ((iVec.X()!=0||iVec.Y()!=0) && !die) 
	{
		iLocation = iLocation.Add(iVec);
		// Effect hits the wall
		if (aLevelRuntime->Level().Collision(iLocation.Div(20)))
			die=1;
	}

	if (die && iType->AfterDeathEffect() )
		aLevelRuntime->NewEffect(iType->AfterDeathEffect(), iLocation, 0, 0);

	SELFTEST;
	return die;
}

TEST( CEffect )
{
	ASSERTPTR( this );
	ASSERTPTR( iType );
	ASSERT( iCounter <= 2*iType->TCount()*iType->Delay() );
	ASSERT( iCounter >= 0 );
	ASSERT( iVec.VectorLength() < 1000 );
	ASSERT( iLocation.Y() < 100000 );
	ASSERT( iLocation.X() < 100000 );
	ASSERT( iLocation.Y() >= 0 );
	ASSERT( iLocation.X() >= 0 );
}

