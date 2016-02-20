#ifndef __CGRAPHICSFADER_H
#define __CGRAPHICSFADER_H

class CGraphicsBuffer;
class CPalette;
class CGameGraphicsInterface;

class CGraphicsFader
{
public:
	CGraphicsFader(CGameGraphicsInterface* aGGI);

	void FadeIn(const CGraphicsBuffer *image, CGraphicsBuffer *aDest, const CPalette* pal, int StartTime, int LastingTime);
	void FadeOut(const CGraphicsBuffer  *image, CGraphicsBuffer *aDest, const CPalette* pal, int StartTime, int LastingTime);
    void FadeZoomOut(const CGraphicsBuffer *image, CGraphicsBuffer *aDest, const CPalette* pal, int StartTime, int LastingTime);
    
private:
	void DrawPhase(int phase, const CGraphicsBuffer* image, CGraphicsBuffer *aDest);
    void DrawZoom(float mul,const CGraphicsBuffer* image, CGraphicsBuffer* aDest);
    
	CGameGraphicsInterface* iGGI;
	int iSine[256];
};
#endif
