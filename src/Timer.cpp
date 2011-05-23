#include "timer.h"
#include <SDL.h>

volatile unsigned TimerCounter = 0;
unsigned TimerSave = 0;
volatile bool TimerUpdated;

SDL_TimerID timer;
Uint32 TimerTick(Uint32 interval, void* /*param*/)
{
	TimerCounter++;
	TimerUpdated=true;
	return interval;
}

void startTimer()
{
	timer=SDL_AddTimer((int)(1000/KTimerFreq), TimerTick,0);
}

void stopTimer()
{
	SDL_RemoveTimer(timer);
}

void saveTimer()
{
	TimerSave = TimerCounter;
}

void restoreTimer()
{
	TimerCounter = TimerSave;
}
