#include "CStatViewData.h"

CStatViewData::CStatViewData()
{
	iDone = true;
	iPlayer = 0;
}

void CStatViewData::SetDone()
{
	iDone = true;
}

bool CStatViewData::Done()
{
	return iDone;
}

int CStatViewData::FirstPlayer()
{
	return iPlayer;
}

void CStatViewData::SetFirstPlayer(int aPlayer)
{
	iPlayer = aPlayer;
}

void CStatViewData::Reset()
{
	iDone = false;
	iPlayer = 0;
}

