/*************************************************************************
> File Name: GL3Timer.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The timer for measure performance of gpu computation such as DMA.
> Created Time: 2020/07/21
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_GL3_TIMER_H
#define CUBBYFLOW_GL3_TIMER_H

#include <Core/Utils/Timer.h>

#ifdef CUBBYFLOW_USE_GL
namespace CubbyFlow {
namespace CubbyRender {
    class GL3Timer : public Timer
    {
    public:
        //! Default Constructor
        GL3Timer();

        //! Default Destructor
        ~GL3Timer();

		//! Returns the time duration since the creation or reset in seconds.
		double DurationInSeconds() const override;

		//! Resets the timer.
		void Reset() override;
    private:
        unsigned int _queryID[2];
    };
}
}

#endif
#endif