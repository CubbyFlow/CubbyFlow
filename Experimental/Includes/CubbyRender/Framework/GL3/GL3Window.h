/*************************************************************************
> File Name: GL3Window.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: GLFW Wrapper window class
> Created Time: 2020/02/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_GL3WWINDOW_H
#define CUBBYFLOW_GL3WWINDOW_H

#ifdef CUBBYFLOW_USE_GL

#include <Framework/Window.h>
#include <memory>
#include <string>

struct GLFWwindow;
using GLFWWindowPtr = GLFWwindow*;

namespace CubbyFlow {
namespace CubbyRender {
    
    //!
    //! \brief implementation of the abstract window class with GLFW
    //!
    class GL3Window final : public Window
    {
        using super_t = Window;
    public: 
        //! Default Constructor.
        GL3Window(const std::string& title, int width, int height);

        //! Default Destructor.
        ~GL3Window();

        //! Request to render given number of frames to the renderer.
        void requestRender(unsigned int numFrames) override;

        //! Sets swap interval.
        void setSwapInterval(int interval) override;

        //! Returns the framebuffer size.
        //! Note that the framebuffer size can be different from the window size,
        //! especially on a Retina display (2x the window size).
        Point2I getFramebufferSize() const override;

        //! Returns the window size.
        Point2I getWindowSize() const override;

        inline GLFWWindowPtr getGLFWWindow() const
        {
            return _window;
        }

        inline unsigned int getNumRequestedRenderFrames() const 
        {
            return _numRequestedRenderFrames;
        }

        inline unsigned int& getNumRequestedRenderFrames() 
        {
            return _numRequestedRenderFrames;
        }
        
        //! Returns current view controller of the window.
        //const ViewControllerPtr& viewController() const;

        //! Sets view controller to the window.
        //void setViewController(const ViewControllerPtr& viewController);

        //! Action implementation when window is resized.
        void onWindowResized(int width, int height) override;

        //! Action implementation when window is moved.
        void onWindowMoved(int width, int height) override;

        //! Action implementation when any event is happend on "KEY"
        void onKey(int key, int scancode, int action, int mods) override;
    protected:
        void onUpdate() override;

    private:
        GLFWWindowPtr _window;
        unsigned int _numRequestedRenderFrames = 0;
    };

    using GL3WindowPtr = std::shared_ptr<GL3Window>;
} 
}

#endif

#endif 