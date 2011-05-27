#ifndef  __TIMER_H_
#define  __TIMER_H_

#include "common/defines.h"

/***************************************\
*	TIMERS								*
\***************************************/

void startTimer();
void stopTimer();
void saveTimer();
void restoreTimer();

extern volatile unsigned TimerCounter;
extern volatile bool TimerUpdated;

inline int TIMER_START(int& Timer)
{
	if (Timer==0)
	{
		Timer=TimerCounter;
		return 1;
	}
	return 0;
}

inline int TIMER_TICKS(int Secs)
{
	return Secs*KTimerFreq;
}

inline int TIMER_SECS(int Timer)
{
	return (TimerCounter-Timer) / KTimerFreq;
}

#endif



