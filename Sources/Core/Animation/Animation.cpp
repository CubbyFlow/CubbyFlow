/*************************************************************************
> File Name: Animation.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Abstract base class for animation-related class.
> Created Time: 2017/04/19
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Animation/Animation.h>
#include <Core/Utils/Logging.h>
#include <Core/Utils/Timer.h>

namespace CubbyFlow
{
	Animation::Animation()
	{
		// Do nothing
	}

	Animation::~Animation()
	{
		// Do nothing
	}

	void Animation::Update(const Frame& frame)
	{
		Timer timer;

		CUBBYFLOW_INFO << "Begin updating frame: " << frame.index
			<< " timeIntervalInSeconds: " << frame.timeIntervalInSeconds
			<< " (1/" << 1.0 / frame.timeIntervalInSeconds
			<< ") seconds";

		OnUpdate(frame);

		CUBBYFLOW_INFO << "End updating frame (took " << timer.DurationInSeconds()
			<< " seconds)";
	}
}