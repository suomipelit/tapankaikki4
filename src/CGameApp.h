#ifndef __GAMEAPP_H_
#define __GAMEAPP_H_

#include "IGUIStateController.h"
#include "IGraphicsModeObserver.h"

class COptions;
class CNetworkDevice;
class CGameData;
class CGameSystem;
class CMessageBoard;
class CPalette;
class CWebVersionChecker;
class CEventFilter;
class CGameSystem;
class CGUIEffectData;

class CGameApp : public IGUIStateController, public IGraphicsModeObserver
{
public:
	CGameApp(const char* aIcon, const char* aCaption);
	~CGameApp();

	void Run(int argc,char *argv[]);

public: // from IGraphicsModeObserver
	void GraphicsModeChanged();

public: // from IGUIStateController
	CGameState* State();
	void StartFadeIn( const CPalette& aPalette );
	void StartFadeIn(CGameData::TPaletteNo aPal);
	void StartFadeOut(CState aNextState);
	void StartZoomOut(CState aNextState);
	void SetFullScreenUpdate(bool aBool);
	CEventHandler* EventHandler();
	CGameGraphicsInterface* GGI();
	CSoundPlayer* SoundPlayer();
	CMessageBoard* MessageBoard();
	IGameSubStateController* GameSystem();
	CGameData* GameData();
	COptions* Options();
	CNetworkDevice* NetworkDevice();
	void SetActive( bool aActive );

	int DebugStartLevel();
#ifdef _DEBUG
	void SelfTest();
#endif

private: // public data
	int iDebugStartLevel;

private:

	void Loop();
	void WriteQuitText();

	CMessageBoard* iMBoard;
	CGameState* iState;
	CEventHandler* iEventHandler;
	CEventFilter* iEventFilter;
	CGameGraphicsInterface* iGGI;
	CSoundPlayer* iSoundPlayer;
	COptions* iOptions;
	CNetworkDevice* iNetwork;
	CGameData* iGameData;
	CGameSystem* iGS;
	CWebVersionChecker* iWVC;

	CGUIEffectData* iEffectData;
	bool iFullUpdate;
	bool iActive;
};

#endif


