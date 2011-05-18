#include "CMouse.h"

#include <math.h>

#include "CMath.h"
#include "error.h"

CMouse::CMouse(): iX(0),iY(0),iXRel(0),iYRel(0),iRightPressed(0)
{
	int a;

	for (a=0;a< EButtonAmount;a++)
		iButtons[a]=0;
}

int CMouse::RightPressed()
{
	return iRightPressed;
}

void CMouse::SetRightPressed(int a)
{
	iRightPressed=a;
}

int CMouse::XPos()
{
	return iX;	
};

int CMouse::YPos()
{
	return iY;	
};

int CMouse::XRel()
{
	return iXRel;	
};

int CMouse::YRel()
{
	return iYRel;	
};

void CMouse::SetXPos(int x)
{
	iX=x;
}

void CMouse::SetYPos(int y)
{
	iY=y;
}

void CMouse::SetXRel(int x)
{
	iXRel=x;
}

void CMouse::SetYRel(int y)
{
	iYRel=y;
}

void CMouse::AddXRel(int x)
{
	iXRel+=x;
}

void CMouse::AddYRel(int y)
{
	iYRel+=y;
}

int CMouse::Button( enum TButtons aButton )
{
	return iButtons[ aButton ];
}

void CMouse::IncButton( enum TButtons aButton )
{
	iButtons[ aButton ]=1;
}

void CMouse::DecButton( enum TButtons aButton )
{
	iButtons[ aButton ]=0;
}


int CMouse::ChkButtons(enum TMouseCheckMode aCheckMode)
{
	ASSERT(aCheckMode==EMouseCheckModeBoth||
		   aCheckMode==EMouseCheckModeLeft||
		   aCheckMode==EMouseCheckModeRight);
	if (aCheckMode==EMouseCheckModeBoth && Button(EButtonLeft) && Button(EButtonRight)) return 1;   
	if (aCheckMode==EMouseCheckModeLeft && Button(EButtonLeft)) return 1;   
	if (aCheckMode==EMouseCheckModeRight && Button(EButtonRight)) return 1;   
	return 0;
}
