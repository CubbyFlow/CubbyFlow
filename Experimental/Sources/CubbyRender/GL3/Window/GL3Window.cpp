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

#include <GL3/Window/GL3Window.h>
#include <GL3/Renderer/GL3Renderer.h>
#include <Framework/Utils/Common.h>
#include <GLFW/glfw3.h>
#include <algorithm>

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
        _numRequestedRenderFrames = std::max(_numRequestedRenderFrames, numFrames);
        glfwPostEmptyEvent();
    }

    void GL3Window::setSwapInterval(int interval)
    {
        super_t::setSwapInterval(interval);
        glfwSwapInterval(interval);
    }


    Size2 GL3Window::getFramebufferSize() const
    {
        int width, height;
        glfwGetFramebufferSize(_window, &width, &height);
        return Size2 {width, height};
    }

    Size2 GL3Window::getWindowSize() const
    {
        int width, height;
        glfwGetWindowSize(_window, &width, &height);
        return Size2 {width, height};
    }

    void GL3Window::onUpdate() 
    {
        for (auto& closure : _updateClosures)
            closure(shared_from_this());
    }

    void GL3Window::onWindowResized(int width, int height)
    {
        _windowSize = Size2 { static_cast<size_t>(width), static_cast<size_t>(height) };
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

    void GL3Window::onMouseButton(int button, int action, int mods)
    {
        UNUSED_VARIABLE(button);
        UNUSED_VARIABLE(action);
        UNUSED_VARIABLE(mods);
    }

    void GL3Window::onMouseCursorEnter(int entered)
    {
        UNUSED_VARIABLE(entered);
    }

    void GL3Window::onMouseCursorPos(double x, double y)
    {
        UNUSED_VARIABLE(x);
        UNUSED_VARIABLE(y);
    }

    void GL3Window::onMouseScroll(double deltaX, double deltaY)
    {
        UNUSED_VARIABLE(deltaX);
        UNUSED_VARIABLE(deltaY);
    }

    void GL3Window::onChar(unsigned int code)
    {
        UNUSED_VARIABLE(code);
    }

    void GL3Window::onDrop(int numDroppedFiles, const char** pathNames)
    {
        UNUSED_VARIABLE(numDroppedFiles);
        UNUSED_VARIABLE(pathNames);
    }
}
}   


#endif 