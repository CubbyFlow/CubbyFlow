/*************************************************************************
> File Name: GLFWWindow.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: GLFW Wrapper window class
> Created Time: 2020/02/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_GLFWWINDOW_H
#define CUBBYFLOW_GLFWWINDOW_H

#ifdef CUBBYFLOW_USE_GL

#include <Framework/Window.h>
#include <memory>

namespace CubbyFlow {
namespace CubbyRender {
    
    //!
    //! \brief Implementation of the abstract window class with GLFW.
    //!
    class GLFWWindow final : public Window
    {
    public: 
        //! Default Constructor.
        GLFWWindow();

        //! Default Destructor.
        ~GLFWWindow();

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
    };

    using GLFWWindowPtr = std::shared_ptr<GLFWWindow>;
} 
}

#endif

#endif 