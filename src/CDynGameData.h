#ifndef __CDYNGAMEDATA_H
#define __CDYNGAMEDATA_H

#include <vector>
#include <list>
#include "common/CCoord.h"
#include "common/defines.h"
#include "common/CLevel.h"
#include "common/CRect.h"
#include "CStatViewData.h"
#include "IGraphicsModeObserver.h"
#include "CMessageBoard.h"

const int KFPSAVGAmount = 15; // average of fps will be counted over KFPSAVGAmount samples.
const int KUpdFPSInterval = 10;

class CBodyPart;
class CEnemy;
class CEffect;
class CBullet;
class CCrate;
class CLightTable;
class CGameSystem;
class CPlayer;
class CGameGraphicsInterface;
class CGameData;
class CSoundPlayer;
class CCharacter;
class CGameStatisticsData;
class IBulletObserver;
class CGraphicsBuffer;

class CLevelRuntimeData;
class IGameMode;

class CDynGameData : public IGraphicsModeObserver
{
public: // Constructor & Destructor
	CDynGameData(CGameGraphicsInterface* aGGI, CGameData* aGameData, CSoundPlayer* aSP,CGameSystem* aGS,COptions* aOptions,int aPlayerAmount);
	~CDynGameData();

public: // from IGraphicsModeObserver

    void GraphicsModeChanged();

public:

	// Initialization methods:
	void PreparePlayersForLevel();
	void Setup( IGameMode* aFightMode, TNetworkMode aNetMode, int aEpisode );
	// return true when everything is ok
	bool SetLevels( int aLevels, int aStartLevel );
	
	// utilities for initialization:
	int CheckCratePlace(int x,int y,int iSize);
	bool TestEnemyLocation( CCoord<int>& aPos, bool aStrictCheck );

	// player handling
	void SetupPlayers();
	void RemoveLocalPlayer( CPlayer *aPlayer );
	// move dead player to list of dead players
	void MoveToDeads( CPlayer* aPlayer );

	// everyframe must run:
	void AnimatePlayers();
	void PushPlayers();
	void CheckBurn();
	void DecHits();
	void ShowSteam();

	void Explo(const CCoord<int>& aPos,int aSize, float aShare, IBulletObserver* aObserver );
	// Statistics view spesific data
	CStatViewData& StatViewData(); // data for statistics view
	// return current game mode object
	IGameMode* GameMode();
	inline CLevelRuntimeData* LevelRuntime(){ return iRuntimeData; };
	CGameStatisticsData* Statistics();
	int CurrentLevel() const;
	int LevelsInEpisode() const;
	int Episode() const;
	// increment iCurrentLevel variable by 1
	void NextLevel();

	// Initialization methods
	void SetupLevelStart();
	TNetworkMode NetworkMode() const { return iNetworkMode; };
	bool AllPlayersDead() const { return iAllPlayersDead;};
	bool iUserQuit;
	bool iShowMap;
	bool iDrawFrames;
	int iNextLevel;

	// game data
	bool CharCollision(const CRect<int>& aRect,CCharacter* aSelf);

	// timing stuff
	void FPSCounter( int aClockVal );
	void ResetClocks();
	inline unsigned int StartTime() const { return iStartTime; };
	inline int LowFramerate() const { return iLowFramerate; };
	inline int LastFrame() const { return iLastFrame; };
	int iCompleteTime;
	unsigned int iEndTime;
	float CurrentFPS() const;

	// Player stuff
	void MapLocalPlayer(int aLocalIndex,int aAnyIndex);
	void SetLocalPlayers(int aAmount);
	inline std::vector<CPlayer*>& Players(){ return iAnyPlayer; };
	inline std::vector<CPlayer*>& LocalPlayers(){ return iLocalPlayer; };
	void UpdateMouseOwner();
	inline CPlayer* MouseOwner(){ return iMouseOwner;};

	// Drawing aids
	const CRect<int>& PlayerDrawRect( int aPlayerNo );
	const int BlackRects( int aPlayers );
	const CRect<int>& BlackRect( int aRect, int aPlayers );
	inline CGraphicsBuffer* DarkBuffer(){ return iDarkBuf; };

	// network chat message methods
	void SetMessageWritingMode(bool aMode);
	inline bool MessageWritingMode(){ return iMessageWritingMode;};
	const char* NetMsg();
	void NetMsgAppendChar(char aChar);
	void NetMsgReset();
	void NetMsgEraseChar();

protected:
	void Reset();
	void PlacePlayersForLevel();
	void CheckPlayersLiving();

protected: // not owned
	CSoundPlayer* iSP;
	CGameData* iGameData;
	CGameGraphicsInterface* iGGI;
	CGameSystem* iGameSystem;
	COptions* iOptions;

protected: // data
	int iLevelsInEpisode;
	int iCurrentLevel;
	bool iMessageWritingMode;
	int iNetworkMessageCursorPos;
	char iNetworkMessageBuffer[KNetMessageMaxLength];

	CLevelRuntimeData* iRuntimeData;
	CGraphicsBuffer* iDarkBuf;
	std::vector< CRect<int> > iDrawRects[KMaxLocalPlayers];
	std::vector< CRect<int> > iBlackRects[KMaxLocalPlayers];

	CGameStatisticsData* iStatistics;

	std::vector<CPlayer *> iAnyPlayer;// PLAYERS 
	std::vector<CPlayer *> iLocalPlayer;

	CPlayer *iMouseOwner;

	int iFramesPerSecData[ KFPSAVGAmount ];
	int iLastFPSClockVal;
	int iLowFramerate;
	unsigned int iLastFrame;
	unsigned int iStartTime;
	int iFPSIndex;
	int iEpisode;
	IGameMode* iGameMode;
	TNetworkMode iNetworkMode;
	bool iAllPlayersDead;

	CStatViewData iStatViewData;
};


#endif



