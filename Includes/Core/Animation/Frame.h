/*************************************************************************
> File Name: Frame.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Representation of an animation frame.
> Created Time: 2017/04/19
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FRAME_H
#define CUBBYFLOW_FRAME_H

namespace CubbyFlow
{
	//!
	//! \brief Representation of an animation frame.
	//!
	//! This struct holds current animation frame index and frame interval in
	//! seconds.
	//!
	struct Frame final
	{
		//! Frame index.
		int index = 0;

		//! Time interval in seconds between two adjacent frames.
		double timeIntervalInSeconds = 1.0 / 60.0;

		//! Constructs Frame instance with 1/60 seconds time interval.
		Frame();

		//! Constructs Frame instance with given time interval.
		Frame(int newIndex, double newTimeIntervalInSeconds);

		//! Returns the elapsed time in seconds.
		double TimeInSeconds() const;

		//! Advances single frame.
		void Advance();

		//! Advances multiple frames.
		//! \param delta Number of frames to advance.
		void Advance(int delta);

		//! Advances single frame (prefix).
		Frame& operator++();

		//! Advances single frame (postfix).
		Frame operator++(int);
	};
}

#endif