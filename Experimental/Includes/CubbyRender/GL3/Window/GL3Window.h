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

#include <Framework/Window/Window.h>
#include <memory>
#include <string>

struct GLFWwindow;
using GLFWWindowPtr = GLFWwindow*;

namespace CubbyFlow {
namespace CubbyRender {
    
    //!
    //! \brief implementation of the abstract window class with GLFW
    //!
    class GL3Window : public Window
    {
        using super_t = Window;
    public: 
        //! Default Constructor
        GL3Window();

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
        Size2 getFramebufferSize() const override;

        //! Returns the window size.
        Size2 getWindowSize() const override;

        //! Return the glfw window ptr;
        GLFWWindowPtr getGLFWWindow();
        
        //! Action implementation when window is resized.
        virtual void onWindowResized(int width, int height) override = 0;

        //! Action implementation when any key is pressed or released
        virtual void onKey(int key, int scancode, int action, int mods) override = 0;

        //! Action implementation when any mouse button is pressed or released.
        virtual void onMouseButton(int button, int action, int mods) override = 0;

        //! Action implementation when cursor is moved
        virtual void onMouseCursorPos(double x, double y) override = 0;

        //! Action implementation when mouse scroll is moved
        virtual void onMouseScroll(double deltaX, double deltaY) override = 0;

    protected:
        virtual void onRenderScene() override = 0;

        virtual void onUpdateScene() override = 0;
        
        GLFWWindowPtr _glfwWindow;
     private:
    };

    using GL3WindowPtr = std::shared_ptr<GL3Window>;
} 
}

#endif

#endif 