#include "CGraphicsDevice.h"
#include "files.h"

namespace
{
	const CCoord<int> KDefaultResolution(640,480);
};

CGraphicsDevice::CGraphicsDevice(const char *aCaption, const char* aIcon): iWidth(0), iHeight(0), iBits(0), iBasicModes(0), iLocked(0), iDontLock(0), iSurfaceOK(0), iSDLsurface(0), iCursorMode(SDL_DISABLE)
{
	int a=0;
	if (SDL_InitSubSystem(SDL_INIT_VIDEO)<0)
	{
        error("CGraphicsDevice::Init: Unable to init SDL_INIT_VIDEO subsystem: %s\n", SDL_GetError());
    }

	if (aCaption)
		iCaptionText=strdup(aCaption);
	else
		iCaptionText=NULL;

	if (aIcon)
        iIconFile=strdup(aIcon);
	else
		iIconFile=NULL;

	ListVideoModes();

   /* Fetch the video info */
    iVideoInfo = SDL_GetVideoInfo( );
	if ( !iVideoInfo )
	{
	    error("Video query failed: %s\n",SDL_GetError( ) );
	}
}

void CGraphicsDevice::SetCursorMode(int aMode)
{
	DEBUG_ASSERT(aMode==0 || aMode==1);
	iCursorMode=aMode;
}

CGraphicsDevice::~CGraphicsDevice()
{
	std::vector<CCoord<int>*>::iterator iter;

	Close();

	free(iCaptionText);
	free(iIconFile);

	for (iter=iResolutions.begin();iter!=iResolutions.end();iter++)
//		free(*iter);
		delete *iter;
}


CGraphicsBuffer* CGraphicsDevice::NewBuf()
{	CGraphicsBuffer* gb;

	gb=new CGraphicsBuffer(iWidth,iHeight);
	ASSERT(gb!=NULL);
	return gb;
}

bool CGraphicsDevice::FullScreen()
{
	bool booli;
	booli=(iSDLsurface->flags&SDL_FULLSCREEN)==SDL_FULLSCREEN;
	return booli;
}

int CGraphicsDevice::SurfaceOK()
{
	return iSurfaceOK;
}

int CGraphicsDevice::Locked()
{
	return iLocked;
}

int CGraphicsDevice::SetDontLock()
{
	if (!SurfaceOK()) return 1;
	iDontLock++;
	return 0;
}

int CGraphicsDevice::UnSetDontLock()
{
	if (!SurfaceOK()) return 1;
	iDontLock--;
	return 0;
}


int CGraphicsDevice::RefreshAll()
{
	int retval=0;

	SetDontLock();
	if (!Locked())
	{
		if (iSDLsurface!=NULL&&Width()!=0)
			SDL_UpdateRect(iSDLsurface,0,0,0,0);
	}
	else retval=1;

	UnSetDontLock();
	return retval;

}

void CGraphicsDevice::SaveShot(const char* aName)
{
	SDL_SaveBMP(iSDLsurface,aName);
}

void CGraphicsDevice::CopyToSurface(const CGraphicsBuffer* aBuf, const CRect<int>& rect)
{
	SDL_Rect r = rect;
	SDL_Surface *surf = aBuf->CopyToSurface( &iPalette );

    // convert surface to the correct format. Is there a better way of doing that?
    SDL_Surface* surf2 = SDL_ConvertSurface(surf, iSDLsurface->format, iSDLsurface->flags);
    if (surf2)
    {
        aBuf->DeleteSurface( surf );
        surf = surf2;
    }


    SDL_ClearError();
	int result = SDL_BlitSurface( surf, &r, iSDLsurface, &r );
	char* msg = SDL_GetError();



	aBuf->DeleteSurface( surf );
}

int CGraphicsDevice::ShowBuf(const CGraphicsBuffer* aBuf, CDrawArea& aDrawArea)
{
	if (Lock())
		return 1;

	int a=0,siz = aDrawArea.Size();

	for (a=0;a<siz;a++)
	{
		CRect<int> r = aDrawArea.Rect(a, Rect());
		CopyToSurface(aBuf, r);
		iDirtyArea.Combine(r);
	}

	if (UnLock())
		return 1;

	Update();

	return 0;
}


int CGraphicsDevice::ShowBuf(const CGraphicsBuffer* aBuf, const CRect<int>& rect)
{
	if (Lock())
		return 1;

	CopyToSurface(aBuf, rect);
    iDirtyArea.Combine(CRect<int>(rect));

	if (UnLock())
		return 1;

	Update();

	return 0;
}

void CGraphicsDevice::Update()
{
	for (int a=0;a<iDirtyArea.Size();a++)
	{
		iRects[a] = iDirtyArea.Rect(a, Rect());
	}

	SDL_UpdateRects(iSDLsurface, iDirtyArea.Size(), iRects);
	iDirtyArea.Reset();
}

int CGraphicsDevice::Lock()
{
	if (!SurfaceOK()) return 1;

	while (iDontLock);
	if ( SDL_MUSTLOCK(iSDLsurface))
        while ( SDL_LockSurface(iSDLsurface) < 0 )
		{
			SDL_Delay(10);
		}
	iLocked=1;
	return 0;
}

int CGraphicsDevice::UnLock()
{
	if (!SurfaceOK()) return 1;

  	if (SDL_MUSTLOCK(iSDLsurface))
        SDL_UnlockSurface(iSDLsurface);
	iLocked=0;
	return 0;
}

int CGraphicsDevice::Clear()
{
	if (!iSDLsurface ||
        Lock()) return 1;

	SDL_FillRect( iSDLsurface, NULL, 0);

	UnLock();
    SDL_UpdateRect(iSDLsurface, 0, 0,0,0);
	return 0;
}

void CGraphicsDevice::Close()
{
	iSurfaceOK=0;
	iLocked=1;
}

/* Return 0 if no error*/
int CGraphicsDevice::SetMode(int aWidth,int aHeight,int aBits, bool aFullScreen, int aExtraFlags)
{
	unsigned int mode=SDL_HWPALETTE|aExtraFlags;

	Close(); // If we're already in graphics mode
	while (iDontLock);

	if ( aFullScreen && iFullScreenPossible )
	{
		mode |= SDL_FULLSCREEN;
	}

	mode |= SDL_SWSURFACE;

	// This must be done before videomode call...
	if (iIconFile!=NULL && exists(getdatapath(std::string(iIconFile)).c_str()))
	{
		SDL_Surface* icon=SDL_LoadBMP(getdatapath(std::string(iIconFile)).c_str());
		SDL_WM_SetIcon(icon, NULL);
	}

    iSDLsurface=SDL_SetVideoMode(aWidth,aHeight, aBits,mode);

	if (iSDLsurface==NULL)
        error("CGraphicsDevice::SetMode: SDL_SetVideoMode(%d,%d,%d,%x) failed: %s\n",aWidth,aHeight, aBits,mode, SDL_GetError());

	// Make sure initialization worked out as supposed
#ifndef __LINUX__
	ASSERT(iSDLsurface->format->BitsPerPixel==aBits);
#else
	ASSERT(iSDLsurface->format->BitsPerPixel==aBits || !aBits);
#endif
	ASSERT(iSDLsurface->w==aWidth);
	ASSERT(iSDLsurface->h==aHeight);
	iWidth=aWidth;
	iHeight=aHeight;
	iBits=aBits;

	SDL_ShowCursor(iCursorMode);
	SDL_WM_SetCaption(iCaptionText,NULL);

	// Let user fuck up (if that's wanted :)
	iSurfaceOK=1;
	iLocked=0;

	Clear();
	SDL_SetPalette(iSDLsurface, SDL_LOGPAL|SDL_PHYSPAL, iPalette.ColorData(), 0, 256);

	return 0;
}

int CGraphicsDevice::SetPalette(const CPalette& pal,int mul)
{
	int i;

	if (!SurfaceOK()) return 1;

	if (mul<=0) mul=0;
	if (mul>256) mul=256;

	for(i=0;i<256;i++)
	{
		iPalette.Color(i).r=(pal.Color(i).r*mul)>>8;
		iPalette.Color(i).g=(pal.Color(i).g*mul)>>8;
		iPalette.Color(i).b=(pal.Color(i).b*mul)>>8;
	}
	SDL_SetPalette(iSDLsurface, SDL_LOGPAL|SDL_PHYSPAL, iPalette.ColorData(), 0, 256);

	return 0;
}

void CGraphicsDevice::GetPalette(CPalette& pal)
{
	pal = iPalette;
}

void CGraphicsDevice::ListVideoModes()
{
	int i;
	SDL_Rect **modes;
	const SDL_VideoInfo* videoInfo = SDL_GetVideoInfo();
	SDL_PixelFormat fmt;
	if (videoInfo)
    {
        fmt = *videoInfo->vfmt;
    }
#ifndef __LINUX__
	//fmt.BitsPerPixel=8;

	/* Get available fullscreen modes */
	modes=SDL_ListModes(&fmt, SDL_FULLSCREEN);
#else
	modes=SDL_ListModes(0, SDL_FULLSCREEN);	// get modes from the current video mode
#endif

	/* Check if our resolution is unrestricted */
	if(
#ifndef __LINUX__
		modes == (SDL_Rect **)-1 ||
#endif
		modes == (SDL_Rect **)0)
	{
		iFullScreenPossible = false;
	}
	else
	{
		iFullScreenPossible = true;

		for(i=0;modes[i];++i)
		{
			/* We're not interested of modes less than 320x200... are we? */
			if (modes[i]->w>=320&&modes[i]->h>=200)
			{
				#ifdef __LINUX__	// SDL_ListModes() returns often multiple modes with same resolution in linux, ignore those
					bool tmp = true;
					for (std::vector<CCoord<int>*>::iterator a = iResolutions.begin(); a != iResolutions.end(); ++a)
						if ( ((*a)->X() == modes[i]->w) && ((*a)->Y() == modes[i]->h) ) {
							tmp = false;
							break;
						}
					if (!tmp) continue;
				#endif

				CCoord<int>* mode=new CCoord<int>(modes[i]->w,modes[i]->h);
				iResolutions.push_back(mode);
			}
		}
	}

	std::vector<CCoord<int>*>::iterator outer;
	std::vector<CCoord<int>*>::iterator inner;

	// Sort modes, Basic bubble :)
	for (outer=iResolutions.begin();outer<iResolutions.end() /*-1*/ ;outer++) {
	 for (inner=outer+1;inner<iResolutions.end();inner++)
	 {
		 if ((*outer)->X()>(*inner)->X()||
			 ((*outer)->X()==(*inner)->X()&&
			  (*outer)->Y()>(*inner)->Y()))
		 {
			 CCoord<int>* tmp;

			 tmp=*outer;
			 *outer=*inner;
			 *inner=tmp;
		 }
	 }
	}

}

int CGraphicsDevice::ResAmount()
{
	return (int)iResolutions.size();
}

const CCoord<int>* CGraphicsDevice::Res(int a)
{
	ASSERT(a>=0);
	ASSERT(a<iResolutions.size());
	return iResolutions[a];
}

void CGraphicsDevice::NextRes( CCoord<int>& aRes )
{
	int modeNo=-1;
	int a;

	for (a=0;a<iResolutions.size()&&modeNo == -1;a++)
		if ( *iResolutions[a] == aRes )
			modeNo = a;

	if ( modeNo == -1 )
	{
		aRes.Set( KDefaultResolution );
		return;
	}

	modeNo++;
	if ( modeNo >= iResolutions.size() )
		modeNo = 0;

	aRes.Set( *iResolutions[modeNo] );
}

void CGraphicsDevice::PrevRes( CCoord<int>& aRes )
{
	int modeNo=-1;
	int a;

	for (a=0;a<iResolutions.size()&&modeNo == -1;a++)
		if ( *iResolutions[a] == aRes )
			modeNo = a;

	if ( modeNo == -1 )
	{
		aRes.Set( KDefaultResolution );
		return;
	}

	modeNo--;
	if ( modeNo < 0 )
		modeNo = iResolutions.size()-1;

	aRes.Set( *iResolutions[modeNo] );
}
