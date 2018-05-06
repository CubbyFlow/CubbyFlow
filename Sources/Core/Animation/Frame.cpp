/*************************************************************************
> File Name: Frame.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Representation of an animation frame.
> Created Time: 2017/04/19
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Animation/Frame.h>
#include <Core/Utils/Macros.h>

namespace CubbyFlow
{
	Frame::Frame()
	{
		// Do nothing
	}

	Frame::Frame(int newIndex, double newTimeIntervalInSeconds) :
		index(newIndex), timeIntervalInSeconds(newTimeIntervalInSeconds)
	{
		// Do nothing
	}

	double Frame::TimeInSeconds() const
	{
		return index * timeIntervalInSeconds;
	}

	void Frame::Advance()
	{
		++index;
	}

	void Frame::Advance(int delta)
	{
		index += delta;
	}

	Frame& Frame::operator++()
	{
		Advance();
		return *this;
	}

	Frame Frame::operator++(int i)
	{
		UNUSED_VARIABLE(i);

		Frame result = *this;
		Advance();
		return result;
	}
}