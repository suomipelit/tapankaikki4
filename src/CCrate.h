#ifndef __CCRATE_H
#define __CCRATE_H

#include  "common/CRect.h"

const int KCrateTextLength=20;

class CPlayer;
class IGUIStateController;

class CCrate
{
public:
	CCrate( const CCoord<int>& aPos, IGUIStateController* aController );
	void MoveText();
	void CheckWakeUp();

	virtual void TryPickup( CPlayer* aPicker, const CRect<int>& aDrawRect ) = 0;
	virtual int  SpriteOffset() = 0;
	virtual const char* Text() = 0;

	// Inline methods
	inline CCoord<int>& Location(){ return iLocation;};

	inline bool InUse(){ return iInUse ;};
	inline int TextAngle(){ return iTextAngle ;};
	inline float TextCounter2(){ return iTextCounter2;};
    inline float TextCounter(){ return iTextCounter;};
	inline int WakeUpCounter(){ return iWakeUpCounter;};
	inline CCoord<float>& TextPos(){ return iTextPos;};

protected:

	void Taken( const CCoord<int>& aScreenPos, const CRect<int>& aDrawRect );
	void SetTextAngle(int aTextAngle);
    void SetTextCounter(float aTextCounter);

protected:
	CCoord<int> iLocation;

	bool iInUse;
	int iTextAngle;
	float iTextCounter2;
    float iTextCounter;
	int iWakeUpCounter;
	CCoord<float> iTextPos;
	IGUIStateController* iController;
};

#endif
