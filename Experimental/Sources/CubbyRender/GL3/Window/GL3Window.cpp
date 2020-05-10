/*************************************************************************
> File Name: GL3Window.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: GLFW Wrapper window class
> Created Time: 2020/02/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Framework/Utils/Common.h>
#include <algorithm>

#ifdef CUBBYFLOW_USE_GL

#include <GL3/Window/GL3Window.h>
#include <GL3/Renderer/GL3Renderer.h>
#include <GL3/Utils/GL3Common.h>
#include <GLFW/glfw3.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    GL3Window::GL3Window()
    {
        //! Do nothing.
    }

    GL3Window::GL3Window(const std::string& title, int width, int height)
        : Window(title, width, height)
    {
        // Use OpenGL 3.3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, CUBBYFLOW_GL_MAJOR_VERSION);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, CUBBYFLOW_GL_MINOR_VERSION);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	    glfwWindowHint(GLFW_SAMPLES, 4);
	    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
#ifdef CUBBYFLOW_MACOSX
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        _glfwWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(_glfwWindow);
        setSwapInterval(_swapInterval);

        RendererPtr renderer = std::make_shared<GL3Renderer>();
        renderer->initializeGL();
        setRenderer(renderer);
        
        CUBBYFLOW_INFO << "Create GLFWwindow with value (" << title << ", " << width << ", " << height << ")";
    }

    GL3Window::~GL3Window()
    {
        glfwDestroyWindow(_glfwWindow);
    }

    void GL3Window::requestRender(unsigned int numFrames)
    {
        super_t::requestRender(numFrames);
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
        glfwGetFramebufferSize(_glfwWindow, &width, &height);
        return Size2 {width, height};
    }

    Size2 GL3Window::getWindowSize() const
    {
        int width, height;
        glfwGetWindowSize(_glfwWindow, &width, &height);
        return Size2 {width, height};
    }

    GLFWWindowPtr GL3Window::getGLFWWindow()
    {
        return _glfwWindow;
    }
    
}
}   


#endif 