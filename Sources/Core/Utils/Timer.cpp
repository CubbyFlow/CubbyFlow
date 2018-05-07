/*************************************************************************
> File Name: Timer.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Simple timer class.
> Created Time: 2017/04/22
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Utils/Timer.h>

namespace CubbyFlow
{
	Timer::Timer()
	{
		m_startingPoint = m_clock.now();
	}

	double Timer::DurationInSeconds() const
	{
		auto end = std::chrono::steady_clock::now();
		auto count = std::chrono::duration_cast<std::chrono::microseconds>(end - m_startingPoint).count();
		return count / 1000000.0;
	}

	void Timer::Reset()
	{
		m_startingPoint = m_clock.now();
	}

}