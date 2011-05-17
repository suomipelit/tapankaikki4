#include "CGameStatisticsData.h"
#include "common/error.h"

CGameStatisticsData::TGeneralLevelStats::TGeneralLevelStats()
{
	iCompeleteTime = 0;
}

CGameStatisticsData::TGeneralLevelStats::~TGeneralLevelStats()
{
}

CGameStatisticsData::TPlayerKillStats::TPlayerKillStats( CPlayer* aPlayer )
{
	iAmountOfKills = 1;
	iPlayer = aPlayer;
}

CGameStatisticsData::TPlayerKillStats::~TPlayerKillStats()
{
}

CGameStatisticsData::TPlayerLevelStats::TPlayerLevelStats()
{
	int a;

	iOwnDeaths = 0;
	iRoundsHit = 0;
	iRoundsShot = 0;
	for (a=0;a<EEnemyAmount;a++)
		iEnemyKills[a] = 0;
}

CGameStatisticsData::TPlayerLevelStats::~TPlayerLevelStats()
{
}

float CGameStatisticsData::TPlayerLevelStats::Accuracy()
{
	SELFTEST;
	if (iRoundsShot == 0)
		return 0;
	return 100.0f * iRoundsHit / iRoundsShot;
}

int CGameStatisticsData::TPlayerLevelStats::TotalKills()
{
	int total = 0;
	SELFTEST;
	for (int a=0;a<EEnemyAmount;a++)
	{
		total+=iEnemyKills[a];
		ASSERT( iEnemyKills[a]>= 0 );
	}
	SELFTEST;
	return total;
}

void CGameStatisticsData::TPlayerLevelStats::AddShot()
{
	SELFTEST;
	iRoundsShot++;
	SELFTEST;
}

void CGameStatisticsData::TPlayerLevelStats::AddHit( float aFraction )
{
	SELFTEST;
	ASSERT( aFraction <= 1 );
	ASSERT( aFraction >  0 );
	iRoundsHit += aFraction;
	SELFTEST;
}

TEST( CGameStatisticsData::TPlayerLevelStats )
{
	ASSERTPTR(	this );
	ASSERT( iRoundsShot >= 0);
	// this does not apply anymore as killing other people 
	// with explo of enemy is now calculated as a bonus
	// ASSERT( iRoundsHit  <= iRoundsShot ); 
	ASSERT( iRoundsHit  >= 0);
}

int CGameStatisticsData::TPlayerLevelStats::TotalPlayerKills( CPlayer* aSelf )
{
	std::vector< TPlayerKillStats >::iterator i;
	int total = 0;
	
	for (i = iPlayerKills.begin(); i!= iPlayerKills.end(); i++)
	{
		ASSERT( (*i).iAmountOfKills > 0 );
		if ( (*i).iPlayer == aSelf ) // suicide is not considered as a positive thing
			total -= (*i).iAmountOfKills;
		else
			total += (*i).iAmountOfKills;
	}

	return total;
}

void CGameStatisticsData::TPlayerLevelStats::AddPlayerKill( CPlayer* aPlayer )
{
	ASSERT( aPlayer!= NULL );

	// check if player already exists in stats
	for (std::vector< TPlayerKillStats >::iterator i = iPlayerKills.begin(); i!=iPlayerKills.end(); i++)
	{
		if ((*i).iPlayer == aPlayer)
		{
			(*i).iAmountOfKills++;
			return;
		}
	}

	// add new player to stats
	iPlayerKills.push_back( TPlayerKillStats( aPlayer ) ); // with default of one kill
}

CGameStatisticsData::TPlayerStats::TPlayerStats( CPlayer* aPlayer, bool aLocal ) : iPlayer( aPlayer ), iLocal( aLocal ), iLevelOfDeath( -1 )
{
}

CGameStatisticsData::TPlayerStats::~TPlayerStats()
{
}

CGameStatisticsData::TPlayerLevelStats& CGameStatisticsData::TPlayerStats::LevelStats( int aLevelIndex )
{
	if ( aLevelIndex >= iLevelStats.size() )
	{
		iLevelStats.reserve( aLevelIndex );
		while ( aLevelIndex >= iLevelStats.size() )
		{
			iLevelStats.push_back( CGameStatisticsData::TPlayerLevelStats() );
		}
	}
	
	return iLevelStats[ aLevelIndex ];
}


CGameStatisticsData::CGameStatisticsData()
{
	iLatest = 0;
}

CGameStatisticsData::~CGameStatisticsData()
{
}


void CGameStatisticsData::Reset()
{
	iLevelStats.clear();
	iPlayerStats.clear();
}

CGameStatisticsData::TGeneralLevelStats& CGameStatisticsData::LevelStats( int aLevelIndex )
{
	if ( aLevelIndex >= iLevelStats.size() )
	{
		iLevelStats.reserve( aLevelIndex );
		while ( aLevelIndex >= iLevelStats.size() )
		{
			iLevelStats.push_back( CGameStatisticsData::TGeneralLevelStats() );
		}
	}
	
	return iLevelStats[ aLevelIndex ];
}	
void CGameStatisticsData::AddPlayer( CPlayer* aPlayer, bool aLocal )
{
	// add new player to stats
	iPlayerStats.push_back( TPlayerStats( aPlayer, aLocal ) );
}

CGameStatisticsData::TPlayerStats& CGameStatisticsData::PlayerStats( CPlayer* aPlayer )
{
	ASSERT( aPlayer!= NULL );

	// seek player in stats
	for (std::vector< TPlayerStats >::iterator i = iPlayerStats.begin(); i!=iPlayerStats.end(); i++)
	{
		if ((*i).iPlayer == aPlayer)
		{
			return (*i);
		}
	}

	throw( CCriticalException("CGameStatisticsData::PlayerStats: Player does not exist in stats.") ); // Should not get this far!
}

void CGameStatisticsData::SetLatestLevel( int aLevelIndex )
{
	iLatest = aLevelIndex;
}

int CGameStatisticsData::LatestLevel()
{
	return iLatest;
}

CGameStatisticsData::TPlayerStats& CGameStatisticsData::PlayerStats( int aIndex )
{
	return iPlayerStats[ aIndex ];
}

int CGameStatisticsData::Players()
{
	return iPlayerStats.size();
}

