/*************************************************************************
> File Name: GridEmitter2.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Abstract base class for 2-D grid-based emitters.
> Created Time: 2017/08/06
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Emitter/GridEmitter2.h>

namespace CubbyFlow
{
	GridEmitter2::GridEmitter2()
	{
		// Do nothing
	}

	GridEmitter2::~GridEmitter2()
	{
		// Do nothing
	}

	void GridEmitter2::Update(double currentTimeInSeconds, double timeIntervalInSeconds)
	{
		if (m_onBeginUpdateCallback)
		{
			m_onBeginUpdateCallback(this, currentTimeInSeconds, timeIntervalInSeconds);
		}

		OnUpdate(currentTimeInSeconds, timeIntervalInSeconds);
	}

	void GridEmitter2::SetOnBeginUpdateCallback(const OnBeginUpdateCallback& callback)
	{
		m_onBeginUpdateCallback = callback;
	}

	void GridEmitter2::CallOnBeginUpdateCallback(double currentTimeInSeconds, double timeIntervalInSeconds)
	{
		m_onBeginUpdateCallback(this, currentTimeInSeconds, timeIntervalInSeconds);
	}
}