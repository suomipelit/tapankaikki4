#ifndef __CGAMESTATISTICSDATA_H_
#define __CGAMESTATISTICSDATA_H_ 

#include <vector>
#include "common/defines.h"
#include "common/CLevel.h"
#include "common/debug.h"

class CPlayer;

class CGameStatisticsData
{
public:

	class TGeneralLevelStats
	{
	public:
		TGeneralLevelStats();
		~TGeneralLevelStats();

		CLevel::TGeneralLevelInfo iLevelInfo;
		int iCompeleteTime;
	};

	class TPlayerKillStats
	{
	public:
		TPlayerKillStats( CPlayer* aPlayer );
		~TPlayerKillStats();

		int iAmountOfKills;
		CPlayer* iPlayer;
	};

	class TPlayerLevelStats
	{
	public:
		TPlayerLevelStats();
		~TPlayerLevelStats();

		void AddShot();
		void AddHit( float aFraction );

		int iOwnDeaths;
		// returns accuracy in procents. e.g. 67.3
		float Accuracy();
		int TotalKills();
		int TotalPlayerKills( CPlayer* aSelf ); // need CPlayer* so that suicides get counted correctly
		void AddPlayerKill( CPlayer* aPlayer );

		std::vector< TPlayerKillStats > iPlayerKills;
		int iEnemyKills[ EEnemyAmount ];
	private:
		__DECLARE_TEST;
		float iRoundsShot;
		float iRoundsHit;
	};

	class TPlayerStats
	{
	public:
		TPlayerStats( CPlayer* aPlayer, bool aLocal );
		~TPlayerStats();
		TPlayerLevelStats& LevelStats( int aLevelIndex );

		int iLevelOfDeath;
		CPlayer* iPlayer;
		bool iLocal;
	private:
		std::vector< TPlayerLevelStats > iLevelStats;
	};

public:
	CGameStatisticsData();
	~CGameStatisticsData();

	void Reset();

	TGeneralLevelStats& LevelStats( int aLevelIndex );
	void AddPlayer( CPlayer* aPlayer, bool aLocal );
	TPlayerStats& PlayerStats( CPlayer* aPlayer );

	// Use these two together
	TPlayerStats& PlayerStats( int aIndex ); // Index refers to index in CGameStatisticsData, not in CDynGameData
	int Players();

	int LatestLevel();
	void SetLatestLevel( int aLevelIndex );


private:

	int iLatest;
	std::vector< TGeneralLevelStats > iLevelStats;
	std::vector< TPlayerStats > iPlayerStats;
};

#endif

