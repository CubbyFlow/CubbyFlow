/*************************************************************************
> File Name: Thread.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The thread wrapper class
> Created Time: 2020/08/27
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Thread.hpp>
#include <Vox/FrameContext.hpp>
#include <chrono>

namespace Vox {

    Thread::Thread()
    {
        //! Do nothing.
    }

    Thread::~Thread()
    {
        //! Do nothing.
    }

	void Thread::Start(const std::shared_ptr<FrameContext>& ctx)
    {
        _handle = std::thread(&Thread::Run, this, ctx, _exitSignal.get_future());
    }

    void Thread::Run(const std::shared_ptr<FrameContext>& ctx, std::future<void> future)
    {
        while(future.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
        {
            Task(ctx);
        }
    }

    void Thread::WaitForEnd()
    {
        _handle.join();
    }

    void Thread::Terminate()
    {
        _exitSignal.set_value();
    }

}