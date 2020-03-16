/*************************************************************************
> File Name: GL3Application.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: GLFW application implementation.
> Created Time: 2020/02/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#ifdef CUBBYFLOW_USE_GL

#include <Framework/GL3/GL3Application.h>
#include <Framework/GL3/GL3Window.h>
#include <Framework/GL3/GL3Renderer.h>
#include <Framework/GL3/GL3Common.h>
#include <Framework/Common.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <vector>

namespace CubbyFlow {
namespace CubbyRender {

    //! Anonymous namespace
    namespace
    {
        GL3Application* gApplicationInstance = nullptr;
    }

    GL3Application::GL3Application()
    {
        gApplicationInstance = this;
    }

    GL3Application::~GL3Application()
    {
        terminate();
        gApplicationInstance = nullptr;
    }

    int GL3Application::initialize()
    {
        glfwSetErrorCallback(onErrorEvent);

        if (!glfwInit()) 
        {
            CUBBYFLOW_ERROR << "GLFW initialization failed";
            return -1;
        }
        
        return 0;
    }

    WindowPtr GL3Application::createWindow(const std::string& title, int width, int height)
    {
        // Use OpenGL 3.3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, CUBBYFLOW_GL_MAJOR_VERSION);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, CUBBYFLOW_GL_MINOR_VERSION);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(CUBBYFLOW_MACOSX)
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        _window = std::make_shared<GL3Window>(title, width, height);

//! In headless server, register window callback function cause segmentation fault.
#if !defined(CUBBYFLOW_HEADLESS)
        GLFWwindow* glfwWindow = _window->getGLFWWindow();
        glfwSetWindowSizeCallback(glfwWindow, onWindowResized);
        glfwSetWindowPosCallback(glfwWindow, onWindowMoved);
        glfwSetKeyCallback(glfwWindow, onKey);
        glfwSetMouseButtonCallback(glfwWindow, onMouseButton);
        glfwSetCursorPosCallback(glfwWindow, onMouseCursorPos);
        glfwSetCursorEnterCallback(glfwWindow, onMouseCursorEnter);
        glfwSetScrollCallback(glfwWindow, onMouseScroll);
        glfwSetCharCallback(glfwWindow, onChar);
        glfwSetCharModsCallback(glfwWindow, onCharMods);
        glfwSetDropCallback(glfwWindow, onDrop);
#endif

        return _window;
    }

    GL3WindowPtr& GL3Application::getApplicationWindow()
    {
        return _window;
    }

    const GL3WindowPtr& GL3Application::getApplicationWindow() const
    {
        return _window;
    }

    int GL3Application::run(int numberOfFrames, double fps, ScreenRecorderPtr recorder)
    {
        UNUSED_VARIABLE(numberOfFrames);
        UNUSED_VARIABLE(fps);
        if (recorder)
        {
            //! Do nothing.
        }

        assert( validateApplication() );
    
        //! Force render first frame
        if (_window != nullptr) 
        {
            // _window->requestRender(1);
        }
        while (validateApplication()) 
        {
            glfwWaitEvents();

            GLFWWindowPtr glfwWindow = _window->getGLFWWindow();

            auto& numRequestedRenderFramesRef = _window->getNumRequestedRenderFrames();
            if (_window->isUpdateEnabled() || numRequestedRenderFramesRef > 0) 
            {
                if (_window->isUpdateEnabled()) 
                {
                    //! update 
                }

                //! Decrease render request count
                numRequestedRenderFramesRef -= 1;

                if (_window->isUpdateEnabled()) 
                {
                    glfwPostEmptyEvent();
                }

                glfwSwapBuffers(glfwWindow);
            }

            if (glfwWindowShouldClose(glfwWindow)) 
            {
                break;
            }
        }

        terminate();

        return 0;
    }

    void GL3Application::terminate()
    {   
        _window.reset();
        glfwTerminate();
    }

    bool GL3Application::validateApplication() 
    {
        return  _window != nullptr;
    }

    void GL3Application::onWindowResized(GLFWwindow* glfwWindow, int width, int height)
    {
        assert(gApplicationInstance);
        auto window = gApplicationInstance->getApplicationWindow();
        assert(gApplicationInstance->validateApplication());

        window->onWindowResized(width, height);
    }

    void GL3Application::onWindowMoved(GLFWwindow* glfwWindow, int width, int height)
    {
        assert(gApplicationInstance);
        auto window = gApplicationInstance->getApplicationWindow();
        assert(gApplicationInstance->validateApplication());

        window->onWindowMoved(width, height);
    }

    void GL3Application::onKey(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods)
    {
        assert(gApplicationInstance);
        auto window = gApplicationInstance->getApplicationWindow();
        assert(gApplicationInstance->validateApplication());

        window->onKey(key, scancode, action, mods);
    }

    void GL3Application::onMouseButton(GLFWwindow* glfwWindow, int button, int action, int mods)
    {   
        UNUSED_VARIABLE(glfwWindow);
        UNUSED_VARIABLE(button);
        UNUSED_VARIABLE(action);
        UNUSED_VARIABLE(mods);
    }

    void GL3Application::onMouseCursorEnter(GLFWwindow* glfwWindow, int entered)
    {
        UNUSED_VARIABLE(glfwWindow);
        UNUSED_VARIABLE(entered);
    }

    void GL3Application::onMouseCursorPos(GLFWwindow* glfwWindow, double x, double y)
    {
        UNUSED_VARIABLE(glfwWindow);
        UNUSED_VARIABLE(x);
        UNUSED_VARIABLE(y);
    }

    void GL3Application::onMouseScroll(GLFWwindow* glfwWindow, double deltaX, double deltaY)
    {
        UNUSED_VARIABLE(glfwWindow);
        UNUSED_VARIABLE(deltaX);
        UNUSED_VARIABLE(deltaY);
    }

    void GL3Application::onChar(GLFWwindow* glfwWindow, unsigned int code)
    {
        UNUSED_VARIABLE(glfwWindow);
        UNUSED_VARIABLE(code);
    }

    void GL3Application::onCharMods(GLFWwindow* glfwWindow, unsigned int code, int mods)
    {
        UNUSED_VARIABLE(glfwWindow);
        UNUSED_VARIABLE(code);
        UNUSED_VARIABLE(mods);
    }

    void GL3Application::onDrop(GLFWwindow* glfwWindow, int numDroppedFiles, const char** pathNames)
    {
        UNUSED_VARIABLE(glfwWindow);
        UNUSED_VARIABLE(numDroppedFiles);
        UNUSED_VARIABLE(pathNames);
    }

    void GL3Application::onErrorEvent(int error, const char* description)
    {
        CUBBYFLOW_ERROR << "Error code(" <<  error << ")  " << description;
    }

} 
}

#endif 