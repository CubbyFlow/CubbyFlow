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

    WindowPtr GL3Application::createMainWindow(const std::string& title, int width, int height)
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

        _mainWindow = std::make_shared<GL3Window>(title, width, height);

//! In headless server, register window callback function cause segmentation fault.
#if !defined(CUBBYFLOW_HEADLESS)
        GLFWwindow* glfwWindow = std::dynamic_pointer_cast<GL3Window>(_mainWindow)->getGLFWWindow();
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

        return _mainWindow;
    }

    int GL3Application::run(int numberOfFrames, EncodingCallback makeScreenshot)
    {
        bool bValidation = validateApplication();
        if (bValidation == false)
            return 1;

        //! dynamic_cast Window to GL3Window.
        GL3WindowPtr glWindow = std::dynamic_pointer_cast<GL3Window>(_mainWindow);
        if (glWindow == nullptr)
            return 1;

        CUBBYFLOW_INFO << "Application validation before running simulation success";
    
        glWindow->setIsUpdateEnabled(true);
        glWindow->requestRender(numberOfFrames);
        CUBBYFLOW_CHECK_GLERROR();

        while (validateApplication()) 
        {
            GLFWWindowPtr glfwWindow = glWindow->getGLFWWindow();
            glfwMakeContextCurrent(glfwWindow);
            glfwWaitEvents();
            auto& numRequestedRenderFramesRef = glWindow->getNumRequestedRenderFrames();
            if (numRequestedRenderFramesRef > 0) 
            {
                if (glWindow->isUpdateEnabled()) 
                {
                    glWindow->update();
                }

                glWindow->render();

                //! Decrease render request count
                numRequestedRenderFramesRef -= 1;

                if (glWindow->isUpdateEnabled()) 
                {
                    glfwPostEmptyEvent();
                }

#ifdef CUBBYFLOW_RECORDING
                if (makeScreenshot)
                {
                    Size2 framebufferSize = glWindow->getFramebufferSize();
                    const auto& frame = glWindow->getCurrentScreen(framebufferSize);
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
                (glfwWindowShouldClose(std::dynamic_pointer_cast<GL3Window>(_mainWindow)->getGLFWWindow()) == GLFW_FALSE);
    }

    void GL3Application::onWindowResized(GLFWwindow* glfwWindow, int width, int height)
    {
        if(gApplicationInstance == nullptr || gApplicationInstance->validateApplication() == false)
            abort();
        auto window = gApplicationInstance->getMainWindow();

        window->onWindowResized(width, height);
    }

    void GL3Application::onWindowMoved(GLFWwindow* glfwWindow, int width, int height)
    {
        if(gApplicationInstance == nullptr || gApplicationInstance->validateApplication() == false)
            abort();
        auto window = gApplicationInstance->getMainWindow();

        window->onWindowMoved(width, height);
    }

    void GL3Application::onKey(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods)
    {
        if(gApplicationInstance == nullptr || gApplicationInstance->validateApplication() == false)
            abort();
        auto window = gApplicationInstance->getMainWindow();

        window->onKey(key, scancode, action, mods);
    }

    void GL3Application::onMouseButton(GLFWwindow* glfwWindow, int button, int action, int mods)
    {   
        if(gApplicationInstance == nullptr || gApplicationInstance->validateApplication() == false)
            abort();
        auto window = gApplicationInstance->getMainWindow();

        window->onMouseButton(button, action, mods);
    }

    void GL3Application::onMouseCursorEnter(GLFWwindow* glfwWindow, int entered)
    {
        if(gApplicationInstance == nullptr || gApplicationInstance->validateApplication() == false)
            abort();
        auto window = gApplicationInstance->getMainWindow();

        window->onMouseCursorEnter(entered);
    }

    void GL3Application::onMouseCursorPos(GLFWwindow* glfwWindow, double x, double y)
    {
        if(gApplicationInstance == nullptr || gApplicationInstance->validateApplication() == false)
            abort();
        auto window = gApplicationInstance->getMainWindow();

        window->onMouseCursorPos(x, y);
    }

    void GL3Application::onMouseScroll(GLFWwindow* glfwWindow, double deltaX, double deltaY)
    {
        if(gApplicationInstance == nullptr || gApplicationInstance->validateApplication() == false)
            abort();
        auto window = gApplicationInstance->getMainWindow();

        window->onMouseScroll(deltaX, deltaY);
    }

    void GL3Application::onChar(GLFWwindow* glfwWindow, unsigned int code)
    {
        if(gApplicationInstance == nullptr || gApplicationInstance->validateApplication() == false)
            abort();
        auto window = gApplicationInstance->getMainWindow();

        window->onChar(code);
    }

    void GL3Application::onDrop(GLFWwindow* glfwWindow, int numDroppedFiles, const char** pathNames)
    {
        if(gApplicationInstance == nullptr || gApplicationInstance->validateApplication() == false)
            abort();
        auto window = gApplicationInstance->getMainWindow();

        window->onDrop(numDroppedFiles, pathNames);
    }

    void GL3Application::onErrorEvent(int error, const char* description)
    {
        CUBBYFLOW_ERROR << "Error code(" <<  error << ")  " << description;
    }

} 
}

#endif 