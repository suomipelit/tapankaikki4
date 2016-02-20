#ifndef __IGUISTATECONTROLLER_H
#define __IGUISTATECONTROLLER_H 

#include "CGameData.h"
#include "CState.h"

class CGameState;
class CEventHandler;
class CGameGraphicsInterface;
class CSoundPlayer;
class CMessageBoard;
class IGameSubStateController;
class CGameData;
class COptions;
class CNetworkDevice;

class IGUIStateController
{
public: // State change related methods
	virtual CGameState* State() = 0;
	virtual void StartFadeIn( CGameData::TPaletteNo aPal ) = 0;
	virtual void StartFadeIn( const CPalette& aPalette ) = 0;
	virtual void StartFadeOut(CState aNextState) = 0;
	virtual void StartZoomOut(CState aNextState) = 0;
	virtual void SetFullScreenUpdate(bool aBool) = 0;

public: // UI I/Os 
	virtual CEventHandler* EventHandler() = 0;
	virtual CGameGraphicsInterface* GGI() = 0;
	virtual CSoundPlayer* SoundPlayer() = 0;
	virtual CMessageBoard* MessageBoard() = 0;
	virtual IGameSubStateController* GameSystem() = 0;
	virtual CGameData* GameData() = 0;
	virtual COptions* Options() = 0;
	virtual CNetworkDevice* NetworkDevice() = 0;
	virtual void SetActive( bool aActive ) = 0;

	virtual int DebugStartLevel() = 0;
};

#endif

