#ifndef __CGAMEGRAPHICSINTERFACE_H
#define __CGAMEGRAPHICSINTERFACE_H

#include <list>
#include "common/defines.h"
#include "common/CRect.h"

class CFonts;
class CGraphicsFader;
class COptions;
class CGameData;
class CMessageBoard;
class CColorTable;
class CGraphicsBuffer;
class IGraphicsModeObserver;
class CGraphicsDevice;

class CGameGraphicsInterface
{
public: 

	enum TGameFont
	{
		EBigMenuFont=0,
		EBigMenuFontDark,
		EBigMenuFontGrey,
		ESmallMenuFont,
		ESmallMenuFontDark,
		ESmallFont,
		EShopNumberFont,
		EGameFontAmount
	};

public:
	CGameGraphicsInterface( COptions* aOptions, CGameData* aGameData, CMessageBoard* aMB,const char* aIcon, const char* aCaption );
	~CGameGraphicsInterface();

	void AddObserver( IGraphicsModeObserver* aObserver );
	void RemoveObserver( IGraphicsModeObserver* aObserver );

    void SetMenuMode();
	void SetGameMode();
	void UpdateMode();
	void SetExtraFlags( int aExtraFlags );

	char* ScreenCapture();

	inline CGraphicsBuffer* DrawBuffer(){ return iVirBuf; };
	inline CGraphicsBuffer* PostProBuffer(){ return iPostBuf; };

	inline CGraphicsFader* Fader(){ return iFader; }
	inline CGraphicsDevice* GD(){ return iGD; }
	inline const CColorTable* ColorTable(){ return iColorTable; }

	const CFonts* Font(enum TGameFont aFont) const;

private:
	void NotifyObservers();
    void ChangeMode(const CCoord<int>& aMode);

private:
	CFonts* iSystemFonts[EGameFontAmount];
	CGraphicsBuffer* iVirBuf;
	CGraphicsBuffer* iPostBuf;
	int iMenuFlag;
	int iExtraFlags;

	CMessageBoard* iMB;
	CGameData* iGameData;
	CGraphicsDevice* iGD;
	CGraphicsFader* iFader;
	CColorTable* iColorTable;
	COptions* iOptions;
	std::list< IGraphicsModeObserver* > iObservers;
};

#endif


