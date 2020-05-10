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

#include <iostream>
#include <Framework/Renderable/PointsRenderable.h>
#include <Core/Vector/Vector3.h>
#include <Core/Array/Array1.h>

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

    void GL3Application::setMainWindow(WindowPtr window)
    {
        GL3WindowPtr glwindow = std::dynamic_pointer_cast<GL3Window>(window);
        if (glwindow == nullptr)
        {
            CUBBYFLOW_ERROR << "Failed to dynamic cast from WindowPtr to GL3WindowPtr";
            std::abort();
        }

//! In headless server, register window callback function cause segmentation fault.
#if !defined(CUBBYFLOW_HEADLESS)
        GLFWwindow* glfwWindow = glwindow->getGLFWWindow();
        glfwSetWindowSizeCallback(glfwWindow, onWindowResized);
        glfwSetKeyCallback(glfwWindow, onKey);
        glfwSetMouseButtonCallback(glfwWindow, onMouseButton);
        glfwSetCursorPosCallback(glfwWindow, onMouseCursorPos);
        glfwSetScrollCallback(glfwWindow, onMouseScroll);
#endif

        _mainWindow = glwindow;
    }

    WindowPtr GL3Application::getMainWindow()
    {
        return _mainWindow;
    }

    int GL3Application::run(int numberOfFrames, EncodingCallback makeScreenshot)
    {
        //! Application Validation before run main loop
        bool bValidation = validateApplication();
        if (bValidation == false)
            return 1;
        CUBBYFLOW_INFO << "Application validation before running simulation success";
    
        //! Prepare rendering.
        _mainWindow->setIsUpdateEnabled(true);
        _mainWindow->requestRender(numberOfFrames);

        //! Main Loop
        while (validateApplication()) 
        {
            GLFWWindowPtr glfwWindow = _mainWindow->getGLFWWindow();
            glfwMakeContextCurrent(glfwWindow);
            glfwWaitEvents();
            auto& numRequestedRenderFramesRef = _mainWindow->getNumRequestedRenderFrames();
            if (numRequestedRenderFramesRef > 0) 
            {
                if (_mainWindow->isUpdateEnabled()) 
                {
                    _mainWindow->updateScene();
                }

                _mainWindow->renderScene();

                //! Decrease render request count
                numRequestedRenderFramesRef -= 1;

                if (_mainWindow->isUpdateEnabled()) 
                {
                    glfwPostEmptyEvent();
                }

#ifdef CUBBYFLOW_RECORDING
                if (makeScreenshot)
                {
                    Size2 framebufferSize = _mainWindow->getFramebufferSize();
                    const auto& frame = _mainWindow->getCurrentScreen(framebufferSize);
                    makeScreenshot(framebufferSize, frame);
                }
#else  
                UNUSED_VARIABLE(makeScreenshot);
#endif  

                glfwSwapBuffers(glfwWindow);
            }
            else
            {
                glfwSetWindowShouldClose(glfwWindow, GL_TRUE);
            }
        }
        
        //! After simulation, check error occurrence.
        CUBBYFLOW_CHECK_GLERROR();

        return 0;
    }

    void GL3Application::terminate()
    {   
        CUBBYFLOW_INFO << "Application terminated";
        _mainWindow.reset();
        glfwTerminate();
    }

    bool GL3Application::validateApplication() 
    {
        return  (_mainWindow != nullptr) && 
                (glfwWindowShouldClose(_mainWindow->getGLFWWindow()) == GLFW_FALSE);
    }

    void GL3Application::onWindowResized(GLFWwindow* glfwWindow, int width, int height)
    {
        UNUSED_VARIABLE(glfwWindow);
        if(gApplicationInstance == nullptr || gApplicationInstance->validateApplication() == false)
            std::abort();
        auto window = gApplicationInstance->getMainWindow();

        window->onWindowResized(width, height);
    }

    void GL3Application::onKey(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods)
    {
        UNUSED_VARIABLE(glfwWindow);
        if(gApplicationInstance == nullptr || gApplicationInstance->validateApplication() == false)
            std::abort();
        auto window = gApplicationInstance->getMainWindow();

        window->onKey(key, scancode, action, mods);
    }

    void GL3Application::onMouseButton(GLFWwindow* glfwWindow, int button, int action, int mods)
    {   
        UNUSED_VARIABLE(glfwWindow);
        if(gApplicationInstance == nullptr || gApplicationInstance->validateApplication() == false)
            std::abort();
        auto window = gApplicationInstance->getMainWindow();

        window->onMouseButton(button, action, mods);
    }

    void GL3Application::onMouseCursorPos(GLFWwindow* glfwWindow, double x, double y)
    {
        UNUSED_VARIABLE(glfwWindow);
        if(gApplicationInstance == nullptr || gApplicationInstance->validateApplication() == false)
            std::abort();
        auto window = gApplicationInstance->getMainWindow();

        window->onMouseCursorPos(x, y);
    }

    void GL3Application::onMouseScroll(GLFWwindow* glfwWindow, double deltaX, double deltaY)
    {
        UNUSED_VARIABLE(glfwWindow);
        if(gApplicationInstance == nullptr || gApplicationInstance->validateApplication() == false)
            std::abort();
        auto window = gApplicationInstance->getMainWindow();

        window->onMouseScroll(deltaX, deltaY);
    }

    void GL3Application::onErrorEvent(int error, const char* description)
    {
        CUBBYFLOW_ERROR << "Error code(" <<  error << ")  " << description;
    }

} 
}

#endif 