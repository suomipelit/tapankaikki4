#ifndef __CSTATE_H_
#define __CSTATE_H_

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

