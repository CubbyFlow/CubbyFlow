/*************************************************************************
> File Name: Timer.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The timer for measure performance of gpu computation such as DMA.
> Created Time: 2020/07/21
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_TIMER_HPP
#define CUBBYFLOW_VOX_TIMER_HPP

#include <Core/Utils/Timer.h>

namespace Vox {

    class DeviceTimer
    {
    public:
        //! Default Constructor
        DeviceTimer();

        //! Default Destructor
        ~DeviceTimer();

		//! Returns the time duration since the creation or reset in seconds.
		double DurationInSeconds() const;

		//! Resets the timer.
		void Reset();
    private:
        unsigned int _queryID[2];
    };

    using HostTimer = CubbyFlow::Timer;
}

#endif