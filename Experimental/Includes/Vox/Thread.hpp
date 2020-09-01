/*************************************************************************
> File Name: Thread.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The thread wrapper class
> Created Time: 2020/08/27
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_THREAD_HPP
#define CUBBYFLOW_VOX_THREAD_HPP

#include <thread>
#include <memory>
#include <future>

namespace Vox {

    class FrameContext;

    class Thread
    {
    public:
        //! Default Constructor
        Thread();

        //! Default Destructor
        virtual ~Thread();

        //! Start the thread worker
		virtual void Start(const std::shared_ptr<FrameContext>& ctx);

        //! Thread task implementation.
    	virtual void Task(const std::shared_ptr<FrameContext>& ctx) {};

        //! Stop the thread.
    	virtual void Stop() {};

        //! Run the task method.
        void Run(const std::shared_ptr<FrameContext>& ctx, std::future<void> future);

        //! Wait for thread end.
    	void WaitForEnd();

        //! Terminate the thread.
    	void Terminate();

    private:
        std::promise<void> _exitSignal;
        std::thread _handle;
    };
}

#endif