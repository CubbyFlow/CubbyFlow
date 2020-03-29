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

#include <GL3/Application/GL3Application.h>
#include <GL3/Window/GL3Window.h>
#include <GL3/Renderer/GL3Renderer.h>
#include <GL3/Utils/GL3Debugging.h>
#include <GL3/Utils/GL3Common.h>
#include <Media/ScreenRecorder.h>
#include <Framework/Utils/Common.h>
#include <Core/Vector/Vector4.h>
#include <Core/Array/Array2.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
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
        if (!glfwInit()) 
        {
            CUBBYFLOW_ERROR << "GLFW initialization failed";
            return -1;
        }
        
        int major, minor, revision;
        glfwGetVersion(&major, &minor, &revision);
        CUBBYFLOW_INFO << "GLFW version : " << major << "." << minor << "." << revision;
        
        glfwSetErrorCallback(onErrorEvent);

        return 0;
    }

    WindowPtr GL3Application::createWindow(const std::string& title, int width, int height)
    {
        // Use OpenGL 3.3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, CUBBYFLOW_GL_MAJOR_VERSION);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, CUBBYFLOW_GL_MINOR_VERSION);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	    glfwWindowHint(GLFW_SAMPLES, 4);
	    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
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
        glfwSetDropCallback(glfwWindow, onDrop);
#endif
        CUBBYFLOW_INFO << "Create GLFWwindow with value (" << title << ", " << width << ", " << height << ")";
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

    int GL3Application::run(int numberOfFrames, EncodingCallback callback)
    {
        bool bValidation = validateApplication();
        if (!bValidation)
            abort();

        CUBBYFLOW_INFO << "Application validation before running simulation success";
        //! Frequently used local variables.
        RendererPtr rendererPtr = _window->getRenderer();

        _window->setIsUpdateEnabled(true);
        _window->requestRender(numberOfFrames);

        while (validateApplication()) 
        {
            glfwWaitEvents();
            GLFWWindowPtr glfwWindow = _window->getGLFWWindow();
            auto& numRequestedRenderFramesRef = _window->getNumRequestedRenderFrames();
            if (numRequestedRenderFramesRef > 0) 
            {
                if (_window->isUpdateEnabled()) 
                {
                    _window->update();
                }

                _window->render();
                //! Decrease render request count
                numRequestedRenderFramesRef -= 1;

                if (_window->isUpdateEnabled()) 
                {
                    glfwPostEmptyEvent();
                }

                if (callback)
                {
                    Size2 framebufferSize = _window->getFramebufferSize();
                    const auto& frame = rendererPtr->getCurrentFrame(framebufferSize);
                    
                    callback(framebufferSize, frame);
                }
                
                glfwSwapBuffers(glfwWindow);
            }
            else
            {
                glfwSetWindowShouldClose(glfwWindow, GL_TRUE);
            }
        }
        
        CUBBYFLOW_CHECK_GLERROR();

        return 0;
    }

    void GL3Application::terminate()
    {   
        CUBBYFLOW_INFO << "Application terminated";
        _window.reset();
        glfwTerminate();
    }

    bool GL3Application::validateApplication() 
    {
        return  (_window != nullptr) && (glfwWindowShouldClose(_window->getGLFWWindow()) == GLFW_FALSE);
    }

    void GL3Application::onWindowResized(GLFWwindow* glfwWindow, int width, int height)
    {
        UNUSED_VARIABLE(glfwWindow);
        if(!gApplicationInstance)
            abort();
        auto window = gApplicationInstance->getApplicationWindow();
        bool result = gApplicationInstance->validateApplication();
        if (!result)
            abort();

        window->onWindowResized(width, height);
    }

    void GL3Application::onWindowMoved(GLFWwindow* glfwWindow, int width, int height)
    {
        UNUSED_VARIABLE(glfwWindow);
        if(!gApplicationInstance)
            abort();
        auto window = gApplicationInstance->getApplicationWindow();
        bool result = gApplicationInstance->validateApplication();
        if (!result)
            abort();

        window->onWindowMoved(width, height);
    }

    void GL3Application::onKey(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods)
    {
        UNUSED_VARIABLE(glfwWindow);
        if(!gApplicationInstance)
            abort();
        auto window = gApplicationInstance->getApplicationWindow();
        bool result = gApplicationInstance->validateApplication();
        if (!result)
            abort();

        window->onKey(key, scancode, action, mods);
    }

    void GL3Application::onMouseButton(GLFWwindow* glfwWindow, int button, int action, int mods)
    {   
        UNUSED_VARIABLE(glfwWindow);
        if(!gApplicationInstance)
            abort();
        auto window = gApplicationInstance->getApplicationWindow();
        bool result = gApplicationInstance->validateApplication();
        if (!result)
            abort();

        window->onMouseButton(button, action, mods);
    }

    void GL3Application::onMouseCursorEnter(GLFWwindow* glfwWindow, int entered)
    {
        UNUSED_VARIABLE(glfwWindow);
        if(!gApplicationInstance)
            abort();
        auto window = gApplicationInstance->getApplicationWindow();
        bool result = gApplicationInstance->validateApplication();
        if (!result)
            abort();

        window->onMouseCursorEnter(entered);
    }

    void GL3Application::onMouseCursorPos(GLFWwindow* glfwWindow, double x, double y)
    {
        UNUSED_VARIABLE(glfwWindow);
        if(!gApplicationInstance)
            abort();
        auto window = gApplicationInstance->getApplicationWindow();
        bool result = gApplicationInstance->validateApplication();
        if (!result)
            abort();

        window->onMouseCursorPos(x, y);
    }

    void GL3Application::onMouseScroll(GLFWwindow* glfwWindow, double deltaX, double deltaY)
    {
        UNUSED_VARIABLE(glfwWindow);
        if(!gApplicationInstance)
            abort();
        auto window = gApplicationInstance->getApplicationWindow();
        bool result = gApplicationInstance->validateApplication();
        if (!result)
            abort();

        window->onMouseScroll(deltaX, deltaY);
    }

    void GL3Application::onChar(GLFWwindow* glfwWindow, unsigned int code)
    {
        UNUSED_VARIABLE(glfwWindow);
        if(!gApplicationInstance)
            abort();
        auto window = gApplicationInstance->getApplicationWindow();
        bool result = gApplicationInstance->validateApplication();
        if (!result)
            abort();

        window->onChar(code);
    }

    void GL3Application::onDrop(GLFWwindow* glfwWindow, int numDroppedFiles, const char** pathNames)
    {
        UNUSED_VARIABLE(glfwWindow);
        if(!gApplicationInstance)
            abort();
        auto window = gApplicationInstance->getApplicationWindow();
        bool result = gApplicationInstance->validateApplication();
        if (!result)
            abort();

        window->onDrop(numDroppedFiles, pathNames);
    }

    void GL3Application::onErrorEvent(int error, const char* description)
    {
        CUBBYFLOW_ERROR << "Error code(" <<  error << ")  " << description;
    }

} 
}

#endif 