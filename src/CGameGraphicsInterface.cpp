#include <string.h>
#include <vector>
#include <stdlib.h>
#include "CGameGraphicsInterface.h"

#include "common/CGraphicsDevice.h"
#include "common/CFonts.h"
#include "common/CColorTable.h"

#include "CGraphicsFader.h"
#include "texts.h"
#include "CMessageBoard.h"
#include "IGraphicsModeObserver.h"


CGameGraphicsInterface::CGameGraphicsInterface(COptions* aOptions, CGameData* aGameData, CMessageBoard* aMB, const char* aIcon, const char* aCaption ): 
	iOptions(aOptions), iGameData(aGameData), iMB(aMB), iExtraFlags(0)
{
	ASSERT(aOptions);
	ASSERT(aGameData);
	ASSERT(aMB);

	iGD = new CGraphicsDevice(aCaption,aIcon);

	iFader = new CGraphicsFader(this);

	iSystemFonts[EBigMenuFont] = new CFonts("fnts/menu25x30light.fnt",0);
	iSystemFonts[EBigMenuFontDark] = new CFonts("fnts/menu25x30dark.fnt",0);
	iSystemFonts[EBigMenuFontGrey] = new CFonts("fnts/menu25x30gray.fnt",0);
	iSystemFonts[ESmallMenuFont] = new CFonts("fnts/menu15x18light.fnt",0);
	iSystemFonts[ESmallMenuFontDark] = new CFonts("fnts/menu15x18dark.fnt",0);
	iSystemFonts[ESmallFont] = new CFonts("fnts/8x8b.fnt",1);	
	iSystemFonts[EShopNumberFont] = new CFonts("fnts/snumbers.fnt",0);

	iColorTable = new CColorTable();

	iVirBuf = new CGraphicsBuffer();
	iPostBuf = new CGraphicsBuffer();
}

CGameGraphicsInterface::~CGameGraphicsInterface()
{
	int a;

	delete iFader;
	delete iGD;

	for (a=0;a<EGameFontAmount;a++)
		delete iSystemFonts[a];

	delete iVirBuf;
	delete iPostBuf;
}

void CGameGraphicsInterface::SetMenuMode()
{
	ChangeMode(CCoord<int>(640,480));
	NotifyObservers();
	iMenuFlag=1;
}

void CGameGraphicsInterface::SetGameMode()
{
	ChangeMode( iOptions->Data().iInGameVideoMode );
	NotifyObservers();
	iMenuFlag=0;
}

void CGameGraphicsInterface::UpdateMode()
{
	const CCoord<int>* nmode;
	char msg[100];

	nmode = &iOptions->Data().iInGameVideoMode;

	if (iGD->FullScreen()!=iOptions->Data().iFullScreen && 
		iGD->FullScreenPossible() )
	{
		ASSERT(EF_SNPRINTF(msg,100,"Setting FULL SCREEN %s",KBoolCatStr[iOptions->Data().iFullScreen])>0);
		iMB->AddMessage(msg);
		ChangeMode( iGD->Rect().BottomRight() );

		NotifyObservers();
		return;
	}
	else
	{
		ASSERT(EF_SNPRINTF(msg,100,"Game resolution %d x %d",nmode->X(),nmode->Y())>0);
		iMB->AddMessage(msg);
						
		if ( !iMenuFlag )
		{
			ChangeMode( *nmode );
			NotifyObservers();
		}
	}
}

void CGameGraphicsInterface::ChangeMode(const CCoord<int>& aMode)
{
	CPalette pal;
	int ret=0;

	if ( !iGD->FullScreenPossible() )
		iOptions->Data().iFullScreen = false;

	if (iGD->Rect().BottomRight() == aMode &&
		iGD->FullScreen()==iOptions->Data().iFullScreen) 
		return;

	iGD->GetPalette(pal);
	iGD->SetPalette(pal,0);
	
	ret=iGD->SetMode(aMode.X(),aMode.Y(),KBitDepth,iOptions->Data().iFullScreen, iExtraFlags );
	iOptions->UpdateGammaChanges();

	if (ret!=0) 
		error("CGameGraphicsInterface::ChangeMode: Couldn't set videomode!");

	iGD->SetPalette(pal,256);

	iVirBuf->Resize(aMode);
	iPostBuf->Resize(aMode);
	//iGameData->ResizeUpdate(aMode);
} 

char* CGameGraphicsInterface::ScreenCapture() 
{
	int free=0,num=0;
	char fname[FILENAME_MAX];
	
	while(free==0 &&num<999) 
	{
		ASSERT(EF_SNPRINTF(fname,FILENAME_MAX,"Screen Shot %03d.bmp",num)>0);
		if (!exists(getsavepath(std::string(fname)).c_str())) free=1;
		num++;
	}

	iGD->SaveShot(fname);
	return strdup(fname);
}

const CFonts* CGameGraphicsInterface::Font(enum TGameFont aFont) const
{
	return iSystemFonts[aFont];
}

void CGameGraphicsInterface::AddObserver( IGraphicsModeObserver* aObserver )
{
	iObservers.push_back( aObserver );
}

void CGameGraphicsInterface::NotifyObservers()
{
	std::list< IGraphicsModeObserver* >::iterator ite = iObservers.begin();

	while (ite!= iObservers.end())
	{
		(*ite)->GraphicsModeChanged();
		ite++;
	}
}

void CGameGraphicsInterface::RemoveObserver( IGraphicsModeObserver* aObserver )
{
	iObservers.remove( aObserver );
}

void CGameGraphicsInterface::SetExtraFlags( int aExtraFlags )
{
	iExtraFlags = aExtraFlags;
}
