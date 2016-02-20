#ifndef __CGRAPHICSDEVICE_H
#define __CGRAPHICSDEVICE_H

#include "CGraphicsBuffer.h"
#include "CDrawArea.h"
#include "CPalette.h"

template <class T> class CCoord;

class CGraphicsDevice
{
public:
	EXPORT CGraphicsDevice(const char *aCaption = NULL, const char* aIcon = NULL);
	EXPORT ~CGraphicsDevice();

	EXPORT bool FullScreen();

	EXPORT int SetPalette(const CPalette& pal,int mul);
	EXPORT void GetPalette(CPalette& pal);

	EXPORT int ShowBuf(const CGraphicsBuffer* aBuf, const CRect<int>& rect);
	EXPORT int ShowBuf(const CGraphicsBuffer* aBuf, CDrawArea& aDrawArea);
	EXPORT inline int ShowBuf(const CGraphicsBuffer* aBuf){return ShowBuf(aBuf,aBuf->Rect() );}
	EXPORT void Update();

	EXPORT void SaveShot(const char* aName);

	EXPORT int RefreshAll();
	
	EXPORT int Clear();
	EXPORT CGraphicsBuffer* NewBuf();
	inline CRect<int> Rect() const {return CRect<int>(0,0,iWidth,iHeight);}
	inline int Width() const{ return iWidth; }
	inline int Height() const{ return iHeight; }
	EXPORT int SetMode(int aWidth,int aHeight,int aBits, bool aFullScreen, int aExtraFlags);
	EXPORT void SetCursorMode(int aMode);
	EXPORT void NextRes( CCoord<int>& aRes );
	EXPORT void PrevRes( CCoord<int>& aRes );
	EXPORT int ResAmount();
	EXPORT const CCoord<int>* Res(int a);
	inline bool FullScreenPossible(){ return iFullScreenPossible;}

	EXPORT void Close();

	EXPORT int SurfaceOK();

private:

	void CopyToSurface(const CGraphicsBuffer* aBuf, const CRect<int>& rect);
	
	void ListVideoModes();

	int Lock();
	int UnLock();
	int SetDontLock();
	int UnSetDontLock();
	int Locked();

private:

	int iWidth;
	int iHeight;
	int iBits;
	int iBasicModes;
	bool iSurfaceOK;
	int iCursorMode;
	bool iFullScreenPossible;

	char* iCaptionText;
	char* iIconFile;

	CDrawArea iDirtyArea;
	SDL_Rect iRects[KMaxRectsInDrawArea];
    const SDL_VideoInfo *iVideoInfo;
 
	volatile int iLocked,iDontLock;
	std::vector<CCoord<int>*> iResolutions;
	SDL_Surface *iSDLsurface;
	CPalette iPalette;
};

#endif


