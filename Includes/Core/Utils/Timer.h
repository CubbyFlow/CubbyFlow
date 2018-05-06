/*************************************************************************
> File Name: Timer.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
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
		double DurationInSeconds() const;

		//! Resets the timer.
		void Reset();

	private:
		std::chrono::steady_clock m_clock;
		std::chrono::steady_clock::time_point m_startingPoint;
	};
}

#endif