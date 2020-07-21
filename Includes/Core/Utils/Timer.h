/*************************************************************************
> File Name: Timer.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Simple timer class.
> Created Time: 2017/04/19
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_TIMER_H
#define CUBBYFLOW_TIMER_H

#include <chrono>

namespace CubbyFlow
{
	//! Simple timer class.
	class Timer
	{
	public:
		//! Constructs the timer and start ticking.
		Timer();

		//! Returns the time duration since the creation or reset in seconds.
		virtual double DurationInSeconds() const;

		//! Resets the timer.
		virtual void Reset();

	private:
		std::chrono::steady_clock m_clock;
		std::chrono::steady_clock::time_point m_startingPoint;
	};
}

#endif