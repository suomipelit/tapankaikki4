#include "CGameMenuItem.h"
#include "CGameMenuContainer.h"

class CKeys;

class CGameMenuItemResetOptions: public CGameMenuItem
{
public:
	CGameMenuItemResetOptions(CGameMenuBase* aParent, const char *aStr);
	TMenuKeyActions HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI);
};

class CGameMenuItemExit: public CGameMenuItem
{
public:
	CGameMenuItemExit(CGameMenuBase* aParent, const char *aStr);
	TMenuKeyActions HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI);
};

/* Base class for menu starter items*/
class CGameMenuItemStartMenu: public CGameMenuItem
{
public:
	~CGameMenuItemStartMenu();
	CGameMenuItemStartMenu(CGameMenuBase* aParent, const char *aStr,TGameMenuState aState);
	TMenuKeyActions HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI);
private:
	TGameMenuState iNextState;
};

class CGameMenuItemChangeKey: public CGameMenuItem
{
public:
	CGameMenuItemChangeKey(CGameMenuBase* aParent, CKeys* kptr, int index);
	TMenuKeyActions HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI);
	CRect<int> Draw(CGraphicsBuffer* aDst,CGameGraphicsInterface* aGGI, int aX, int aY, int aActive);
protected:
	CKeys* iKeys;
	int iKeyInputMode;
	int iIndex;
};


class CGameMenuItemExecSingleGame: public CGameMenuItem
{
public:
	CGameMenuItemExecSingleGame(CGameMenuBase* aParent, const char *aStr, int aEpisNo);
	TMenuKeyActions HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI);
	void SetEpisodeNo(int a);
	void SetEpisodeName(const char* str);
protected:
	int iEpisodeNumber;
};

class CGameMenuItemExecMultiplay: public CGameMenuItem
{
public:
	CGameMenuItemExecMultiplay(CGameMenuBase* aParent, const char *aStr);
	TMenuKeyActions HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI);
};

class CGameMenuItemTitle: public CGameMenuItem
{
public:
	CGameMenuItemTitle(CGameMenuBase* aParent, const char *aStr);
};

class CGameMenuItemBoolean: public CGameMenuItem
{
public:
	CGameMenuItemBoolean(CGameMenuBase* aParent, const char *aStr, bool* aBool);
	TMenuKeyActions HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI);
	CRect<int> Draw(CGraphicsBuffer* aDst,CGameGraphicsInterface* aGGI, int aX, int aY, int aActive);
protected:
	virtual void UpdateText();
	bool* iBool;
};

class CGameMenuItemSplitSelect;

class CGameMenuItemNetworkModeSelect: public CGameMenuItem
{
public:
	CGameMenuItemNetworkModeSelect(CGameMenuBase* aParent, const char *aStr);
	TMenuKeyActions HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI);
	CRect<int> Draw(CGraphicsBuffer* aDst,CGameGraphicsInterface* aGGI, int aX, int aY, int aActive);
	void SetSplitSelect(CGameMenuItemSplitSelect* aSplitSelect);
	void EnableNetwork();
protected:
	virtual void UpdateText();
	CGameMenuItemSplitSelect* iSplitSelect;
};

class CGameMenuItemSplitSelect: public CGameMenuItem
{
public:
	CGameMenuItemSplitSelect(CGameMenuBase* aParent, const char *aStr);
	TMenuKeyActions HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI);
	CRect<int> Draw(CGraphicsBuffer* aDst,CGameGraphicsInterface* aGGI, int aX, int aY, int aActive);
	void SetNetworkSelect(CGameMenuItemNetworkModeSelect* aNetSelect);
	void EnableSplit();
protected:
	virtual void UpdateText();
	CGameMenuItemNetworkModeSelect* iNetSelect;
};

class CGameMenuItemLevelSelect: public CGameMenuItem
{
public:
	CGameMenuItemLevelSelect(CGameMenuBase* aParent, const char *aStr);
	TMenuKeyActions HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI);
	void ResetLevel();
	void SetEnabled( bool aEnabled );
	void UpdateText();
protected:
	void CheckDisable();
	COptions* iOptions;
	bool iSelfDisabled;
};

class CGameMenuItemEpisodeSelect: public CGameMenuItem
{
public:
	CGameMenuItemEpisodeSelect(CGameMenuBase* aParent, const char *aStr,CGameMenuItemLevelSelect* aLevelSelect);
	TMenuKeyActions HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI);
	void UpdateText();
protected:
	COptions* iOptions;
	CGameMenuItemLevelSelect* iLevelSelect;
};

class CGameMenuItemGameModeSelect: public CGameMenuItem
{
public:
	CGameMenuItemGameModeSelect(CGameMenuBase* aParent, const char *aStr, CGameMenuItemLevelSelect* aLevelSelect, CGameMenuItemEpisodeSelect* aEpisodeSelect );
	TMenuKeyActions HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI);
	CRect<int> Draw(CGraphicsBuffer* aDst,CGameGraphicsInterface* aGGI, int aX, int aY, int aActive);
protected:
	virtual void UpdateText();
	void UpdateLevelSelect();
	CGameMenuItemLevelSelect* iLevelSelect;
	CGameMenuItemEpisodeSelect* iEpisodeSelect;
};

class CGameMenuItemFullScreen: public CGameMenuItemBoolean
{
public:
	CGameMenuItemFullScreen(CGameMenuBase* aParent, const char *aStr, bool* aBool);
	TMenuKeyActions HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI);
};

class CGameMenuItemResolution: public CGameMenuItem
{
public:
	CGameMenuItemResolution(CGameMenuBase* aParent, const char *aStr, CCoord<int>* aGameMode);
	TMenuKeyActions HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI);
	void UpdateText();
protected:

	CCoord<int>* iGameMode;
	CGameGraphicsInterface* iGGI;
};

class CGameMenuItemString: public CGameMenuItem
{
public:
	~CGameMenuItemString();
	CGameMenuItemString(CGameMenuBase* aParent, const char *aTitleStr, char *aParamStr,unsigned int maxlen);
	TMenuKeyActions HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI);
	CRect<int> Draw(CGraphicsBuffer* aDst,CGameGraphicsInterface* aGGI, int aX, int aY, int aActive);
protected:
	char* iParamStr;
	unsigned int iMaxLen;
	int	iInputMode;
};

class CGameMenuItemPercent: public CGameMenuItem
{
public:
	CGameMenuItemPercent(CGameMenuBase* aParent, const char *aStr, float* percent, float lowerlimit, float upperlimit, float step);
	TMenuKeyActions HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI);
	CRect<int> Draw(CGraphicsBuffer* aDst,CGameGraphicsInterface* aGGI, int aX, int aY, int aActive);
protected:
	void UpdateText();
	float* iPercent;
	float iLow, iHigh;
	float iStep;
};

//class CGameMenuItemCredits: public CGameMenuItem
//{
//public:
//	CGameMenuItemCredits(CGameMenuBase* aParent, const char *aStr);
//	TMenuKeyActions HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI);
//};


class CGameMenuItemMouseMode: public CGameMenuItem
{
public:
	CGameMenuItemMouseMode(CGameMenuBase* aParent, const char *aStr);
	TMenuKeyActions HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI);
	CRect<int> Draw(CGraphicsBuffer* aDst,CGameGraphicsInterface* aGGI, int aX, int aY, int aActive);
};

class CGameMenuItemMusicTheme: public CGameMenuItem
{
public:
	CGameMenuItemMusicTheme(CGameMenuBase* aParent, const char *aStr);
	TMenuKeyActions HandleKeyPress(SDL_keysym key,CGameMenuBase* aMenu,CGameGraphicsInterface* aGGI);
	CRect<int> Draw(CGraphicsBuffer* aDst,CGameGraphicsInterface* aGGI, int aX, int aY, int aActive);
protected:
	void UpdateText();
	int iSelected;
};

