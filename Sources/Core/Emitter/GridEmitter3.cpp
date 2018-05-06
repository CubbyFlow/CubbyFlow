/*************************************************************************
> File Name: GridEmitter3.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Abstract base class for 3-D grid-based emitters.
> Created Time: 2017/08/06
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Emitter/GridEmitter3.h>

namespace CubbyFlow
{
	GridEmitter3::GridEmitter3()
	{
		// Do nothing
	}

	GridEmitter3::~GridEmitter3()
	{
		// Do nothing
	}

	void GridEmitter3::Update(double currentTimeInSeconds, double timeIntervalInSeconds)
	{
		if (m_onBeginUpdateCallback)
		{
			m_onBeginUpdateCallback(this, currentTimeInSeconds, timeIntervalInSeconds);
		}

		OnUpdate(currentTimeInSeconds, timeIntervalInSeconds);
	}

	void GridEmitter3::SetOnBeginUpdateCallback(const OnBeginUpdateCallback& callback)
	{
		m_onBeginUpdateCallback = callback;
	}

	void GridEmitter3::CallOnBeginUpdateCallback(double currentTimeInSeconds, double timeIntervalInSeconds)
	{
		m_onBeginUpdateCallback(this, currentTimeInSeconds, timeIntervalInSeconds);
	}
}