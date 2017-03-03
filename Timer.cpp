#include "Include/Timer.h"

LTimer::LTimer()
{
	//Init vars
	mStartTicks = 0;
	mPausedTicks = 0;

	mPaused = false;
	mStarted = true;
}

void LTimer::start()
{
	//start timer
	mStarted = true;

	//unpause
	mPaused = false;

	//Get current clock time
	mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

void LTimer::stop()
{
	//stop the timer
	mStarted = false;

	//unpause
	mPaused = false;

	//clear ticks
	mStartTicks = 0;
	mPausedTicks = 0;
}

void LTimer::pause()
{
	//if timer running and not paused
	if (mStarted && !mPaused)
	{
		//pause the timer
		mPaused = true;

		//calculate the paused ticks
		mPausedTicks = SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
	}
}

void LTimer::unpause()
{
	//if timer running and not paused
	if (mStarted && mPaused)
	{
		mPaused = false;

		//Reset starting ticks
		mStartTicks = SDL_GetTicks() - mPausedTicks;

		//reset paused ticks
		mPausedTicks = 0;
	}
}

Uint32 LTimer::getTicks()
{
	//Actual timer time
	Uint32 time = 0;

	//if running
	if (mStarted)
	{
		if (mPaused)
		{
			//return # ticks since paused
			time = mPausedTicks;
		}
		else
		{
			//return current time minus start time
			time = SDL_GetTicks() - mStartTicks;
		}
	}

	return time;
}

bool LTimer::isStarted()
{
	//Timer is running and paused or unpaused
	return mStarted;
}

bool LTimer::isPaused()
{
	//Timer is running and paused
	return mPaused && mStarted;
}