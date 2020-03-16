/*************************************************************************
> File Name: GL3Window.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: GLFW Wrapper window class
> Created Time: 2020/02/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#ifdef CUBBYFLOW_USE_GL

#include <Framework/GL3/GL3Window.h>
#include <Framework/GL3/GL3Renderer.h>
#include <Framework/Common.h>
#include <GLFW/glfw3.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    GL3Window::GL3Window(const std::string& title, int width, int height)
        : Window(title, width, height)
    {
        _window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(_window);
        setSwapInterval(_swapInterval);

        RendererPtr renderer = std::make_shared<GL3Renderer>();
        renderer->initializeGL();
        setRenderer(renderer);
        
        //!onWindowResized(width, height);
    }

    GL3Window::~GL3Window()
    {
        glfwDestroyWindow(_window);
    }

    void GL3Window::requestRender(unsigned int numFrames)
    {
        super_t::requestRender(numFrames);
        _numRequestedRenderFrames = numFrames;
        glfwPostEmptyEvent();
    }

    void GL3Window::setSwapInterval(int interval)
    {
        super_t::setSwapInterval(interval);
        glfwSwapInterval(interval);
    }


    Point2I GL3Window::getFramebufferSize() const
    {
        int width, height;
        glfwGetFramebufferSize(_window, &width, &height);
        return Point2I {width, height};
    }

    Point2I GL3Window::getWindowSize() const
    {
        int width, height;
        glfwGetWindowSize(_window, &width, &height);
        return Point2I {width, height};
    }

    void GL3Window::onUpdate() 
    {
        //! Do nothing.
    }

    void GL3Window::onWindowResized(int width, int height)
    {
        _windowSize = Point2I { width, height };
    }

    void GL3Window::onWindowMoved(int width, int height)
    {
        UNUSED_VARIABLE(width);
        UNUSED_VARIABLE(height);
    }

    void GL3Window::onKey(int key, int scancode, int action, int mods)
    {
        UNUSED_VARIABLE(scancode);
        UNUSED_VARIABLE(mods);
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		    glfwSetWindowShouldClose(_window, GLFW_TRUE);
    }
}
}


#endif 