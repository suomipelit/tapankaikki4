#ifndef __CSTATE_H
#define __CSTATE_H

class CState
{
public: 
	enum TMainState
	{
		EMainStateLogo1=0,
		EMainStateLogo2,
		EMainStateMenu,
		EMainStateGame,
		EMainStateQuit,
		EMainStateConfirmedQuit,
		EMainStateAmount
	};
public:
	CState(TMainState aMainState, int aSubstate);
	bool operator == (const CState& aState);

public: 

	enum TMainState iMainState;
	int	iSubState;
};

#endif

