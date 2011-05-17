#ifndef __CGUIEFFECTDATA_H_
#define __CGUIEFFECTDATA_H_

#include "common/CPalette.h"
#include "CState.h"

class CGraphicsFader;
class CGraphicsBuffer;

class CGUIEffectData
{
public:
	unsigned int iStarted;
	unsigned int iDuration;
	CState iNextState;
	bool iStateTransition;
	bool iEnabled;
	bool iFadeIn;
	const CPalette* iPalette;
	CPalette iSparePalette;
	// Let me introduce you method pointers.. I just lööv C++
	void (CGraphicsFader :: * iFaderMethod)(const CGraphicsBuffer*,CGraphicsBuffer *, const CPalette*, int, int );

	CGUIEffectData();
};

#endif

