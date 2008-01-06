#ifndef __CMOUSE_H
#define __CMOUSE_H

#include "defines.h"

class CMouse
{
public:
	enum TMouseCheckMode
	{
		EMouseCheckModeLeft=1,
		EMouseCheckModeRight,
		EMouseCheckModeBoth
	};
	enum TButtons
	{
		EButtonLeft=0,
		EButtonMiddle,
		EButtonRight,
		EButtonWheelUp,
		EButtonWheelDown,
		EButtonAmount
	};
public:
	EXPORT CMouse();

	EXPORT int RightPressed();
	EXPORT void SetRightPressed(int a);
	EXPORT void SetXPos(int x);
	EXPORT void SetYPos(int y);
	EXPORT void AddXRel(int x);
	EXPORT void AddYRel(int y);
	EXPORT void SetXRel(int x);
	EXPORT void SetYRel(int y);
	EXPORT int XPos();
	EXPORT int YPos();
	EXPORT int XRel();
	EXPORT int YRel();

	EXPORT int Button(enum TButtons aButton);
	EXPORT void IncButton(enum TButtons aButton);
	EXPORT void DecButton(enum TButtons aButton);

    EXPORT int ChkButtons(enum TMouseCheckMode aCheckMode);
private:
    int iX,iY;
    int iXRel,iYRel; 
    int iRightPressed; // used in weapon change-> 0 if right not pressed 1 if weapon changed
	int iButtons[ EButtonAmount ];
    
};


#endif

