/*************************************************************************
> File Name: App.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The application class for representating application side cpu tasks.
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_APP_HPP
#define CUBBYFLOW_VOX_APP_HPP

#include <Core/Vector/Vector2.h>
#include <Core/Vector/Vector4.h>
#include <memory>
#include <stack>

namespace Vox {

    class FrameContext;

    /**
     *  CPU side tasks collection wrapper class. 
     */
    class App
    {
    public:
        App();
        virtual ~App();

        //! Return the window screen size.
        CubbyFlow::Vector2I GetWindowSize();

        //! Initialize the base application.
        virtual bool Initialize();
        //! OpenGL Commands or other tasks collection for 
        //! preparing call DrawFrame.
        void BeginFrame(std::shared_ptr<FrameContext>& ctx);
        //! Actual Drawing Call
        virtual void DrawFrame() {};
        //! End tasks after drawing call.
        void EndFrame(std::shared_ptr<FrameContext>& ctx);

        //! Return window title.
        virtual const char* GetWindowTitle() { return nullptr; };

        //! Pop frame context from top of the stack.
        std::shared_ptr<Vox::FrameContext> PopFrameContextFromStack()
        {
            if (_ctxStack.empty())
            {
                return nullptr;
            }
            else
            {
                const auto& ctx = _ctxStack.top();
                _ctxStack.pop();
                return ctx;
            }
        }
        //! Push frame context to the stack.
        void PushFrameContextToStack(std::shared_ptr<Vox::FrameContext> ctx)
        {
            _ctxStack.push(ctx);
        }
        //! Set Window screen size.
        void SetWindowSize(CubbyFlow::Vector2I size);
        //! Set screen background color
        void SetBackgroundColor(CubbyFlow::Vector4F color);
    protected:
        CubbyFlow::Vector2I _windowSize { 1200, 900 }; //! window size
        CubbyFlow::Vector4F _bgColor { 0.2f, 0.6f, 0.2f, 1.0f}; //! background color
    private:
        std::stack<std::shared_ptr<Vox::FrameContext>> _ctxStack;
    };

};

#endif