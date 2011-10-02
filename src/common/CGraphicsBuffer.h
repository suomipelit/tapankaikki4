#ifndef __GRAPHICSBUFFER_H_
#define __GRAPHICSBUFFER_H_

#include "defines.h"
#include "CRect.h"
#include <SDL.h>

class CPalette;

class CGraphicsBuffer
{
public:
	
	EXPORT CGraphicsBuffer(int aWidth,int aHeight);
	EXPORT CGraphicsBuffer(const CRect<int>& aRect);
	EXPORT CGraphicsBuffer(CGraphicsBuffer& aBuf);
	EXPORT CGraphicsBuffer();
	EXPORT ~CGraphicsBuffer();

	EXPORT void Reset();

	inline void PutPixel(int aX, int aY,unsigned char aColor)
	{
		DEBUG_ASSERT(aX>=0);
		DEBUG_ASSERT(aY>=0);
		DEBUG_ASSERT(aX<iWidth);
		DEBUG_ASSERT(aY<iHeight);
		DEBUG_ASSERT(iBuf);
	
		iBuf[aY*iWidth+aX]=aColor;
	}

	inline void PutPixelClip(int aX, int aY,unsigned char aColor,const CRect<int>& aDrawRect)
	{
		if (aDrawRect.Inside(aX,aY))
			PutPixel(aX,aY,aColor);
	}
	
	EXPORT void LoadEFP(const std::string& aFilename, CPalette* aPalette);
	EXPORT void LoadPCX(const std::string& aFilename, CPalette* aPalette);
	EXPORT void LoadEFP2(const std::string& aFilename, CPalette* aPalette);
	EXPORT void LoadSCI(const std::string& aFilename, CPalette* aPalette);
	EXPORT void Load(std::string aFilename, CPalette* aPalette);

	EXPORT void Save(const std::string& aFilename, const CPalette* aPalette) const;
	EXPORT void SaveEFP(const std::string& aFilename, const CPalette* aPalette) const;
	EXPORT void SaveBMP(const std::string& aFilename,const CPalette* aPalette) const;
	EXPORT void SavePCX(const std::string& aFilename, const CPalette* aPalette) const;
	EXPORT void SaveSCI(const std::string& aFilename, const CPalette* aPalette) const;
	EXPORT void SaveEFP2(const std::string& aFilename, const CPalette* aPalette) const;

	EXPORT void Clear(); 
	EXPORT void Stretch(int aNewWidth,int aNewHeight);

	// Copy into CGraphicsBuffer
	EXPORT CRect<int> TransCopy(int aDX,int aDY, const CGraphicsBuffer* buf,int aSX, int aSY,int aWidth, int aHeight, int aColorKey);
	EXPORT CRect<int> Copy(int aDX,int aDY, const CGraphicsBuffer* buf,int aSX, int aSY,int aWidth, int aHeight);
	EXPORT CRect<int> Copy(const CGraphicsBuffer* buf,int aSX, int aSY,int aWidth, int aHeight);
	EXPORT CRect<int> Copy(const CGraphicsBuffer* src);
	EXPORT CRect<int> Copy(const unsigned char* buf,int aWidth, int aHeight);
	EXPORT CRect<int> Copy(const unsigned char* buf,int aSX, int aSY,int aWidth, int aHeight);
	EXPORT CRect<int> Copy(int aDX, int aDY,const unsigned char* buf,int aWidth, int aHeight);
	EXPORT CRect<int> Copy(int aDX, int aDY,const CGraphicsBuffer* buf);

	EXPORT CRect<int> DrawBox( const CRect<int>& aRect,int aAdd);
	EXPORT CRect<int> Rectangle(int aX1,int aY1,int aX2,int aY2,unsigned char aCol,int aOffset);
	EXPORT CRect<int> FillRectangle( const CRect<int>& aRect, unsigned char aColor );
	
	EXPORT virtual void Resize(int aWidth,int aHeight);
	inline void Resize( const CCoord<int>& aSize ){ Resize(aSize.X(), aSize.Y()); }

	// DrawPolygon - Draws polygon with color aCol. aClip: doesn't draw over buffer borders if enabled
	EXPORT void DrawPolygon(int aX1,int aY1,int aX2,int aY2,int aX3,int aY3,int aCol,int aClip);

	// Inlining made TK run like blessed :) 
	inline CRect<int> Rect() const 
	{
		DEBUG_ASSERT(iWidth>=0);
		DEBUG_ASSERT(iHeight>=0);
		return CRect<int>(0,0,iWidth,iHeight);
	}

	inline const unsigned char *Ptr() const
	{
		DEBUG_ASSERT(iBuf);
		return iBuf;
	}

	inline unsigned char *Ptr()
	{
		DEBUG_ASSERT(iBuf);
		return iBuf;
	}

	inline int Width() const
	{
		DEBUG_ASSERT(iWidth>=0);
		return iWidth;
	}

	inline int Height() const
	{
		DEBUG_ASSERT(iHeight>=0);
		return iHeight;
	}

	inline unsigned char& operator [](const int a)
	{
		DEBUG_ASSERT(a<iWidth*iHeight);
		return iBuf[a];
	}

	inline const unsigned char& operator [](const int a) const 
	{
		DEBUG_ASSERT(a<iWidth*iHeight);
		return iBuf[a];
	}
	
	// For now these could be here ;)
	SDL_Surface* CopyToSurface(const CPalette* aPalette) const;
	void DeleteSurface(SDL_Surface* aSurface) const;
	int CopyFromSurface(SDL_Surface* aSurface,CPalette* aPalette);

protected:

	void DrawHorizLineClip(unsigned char* aLinePtr, int aCol,int aX1,int aX2);
	void DrawHorizLineNoClip(unsigned char* aLinePtr, int aCol,int aX1,int aX2);
	void DrawPolyTri(int aK1,int aK2,int aX,int aY,int aHeight, int aCol, int aClip);

	int iWidth,iHeight;
	unsigned char* iBuf;

};

#endif
