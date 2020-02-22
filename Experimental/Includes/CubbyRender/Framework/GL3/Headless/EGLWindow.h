/*************************************************************************
> File Name: EGLWindow.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: EGL Wrapper window class
> Created Time: 2020/02/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_EGLWWINDOW_H
#define CUBBYFLOW_EGLWWINDOW_H

#ifdef CUBBYFLOW_USE_GL

#include <Framework/Window.h>
#include <EGL/egl.h>
#include <memory>
#include <string>

namespace CubbyFlow {
namespace CubbyRender {
    
    //!
    //! \brief implementation of the abstract window class with EGL (headless opengl library)
    //!
    class EGLWindow final : public Window
    {
    public: 
        //! Default Constructor.
        EGLWindow(EGLDisplay display, const std::string& title, int width, int height);

        //! Default Destructor.
        ~EGLWindow();

        //! Returns the framebuffer size.
        //! Note that the framebuffer size can be different from the window size,
        //! especially on a Retina display (2x the window size).
        Size2 framebufferSize() const override;

        //! Returns the window size.
        Size2 windowSize() const override;

        //! Returns current view controller of the window.
        //const ViewControllerPtr& viewController() const;

        //! Sets view controller to the window.
        //void setViewController(const ViewControllerPtr& viewController);
    protected:
    private:
        EGLDisplay _display;
        EGLSurface _surface;
    };

    using EGLWindowPtr = std::shared_ptr<EGLWindow>;
} 
}

#endif

#endif 