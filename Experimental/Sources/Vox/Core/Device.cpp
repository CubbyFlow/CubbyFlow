/*************************************************************************
> File Name: Device.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: OpenGL Device class related to graphics device
> Created Time: 2020/08/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Core/Device.hpp>
#include <Vox/Core/FrameContext.hpp>
#include <Vox/Utils/DebugUtils.hpp>
#include <Vox/Core/App.hpp>
#include <Vox/Utils/Timer.hpp>
#include <Core/Utils/Logging.hpp>
#include <Core/Geometry/Point2.hpp>
#include <Core/Geometry/Size2.hpp>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Vox {

	namespace {
		std::weak_ptr<App> gApplication;
	};

	void OnWindowResized(GLFWwindow* window, int width, int height);
    void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods);
    void OnMouseButton(GLFWwindow* window, int button, int action, int mods);
    void OnMouseCursorPos(GLFWwindow* window, double x, double y);
    void OnMouseScroll(GLFWwindow* window, double deltaX, double deltaY);

	void Device::Initialize()
	{
		VoxAssert(glfwInit(), CURRENT_SRC_PATH_TO_STR, "GLFW initialization failed");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	    glfwWindowHint(GLFW_SAMPLES, 4);
	    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
		glfwSwapInterval(1); //! Turn on the VSync.
#ifdef CUBBYFLOW_MACOSX
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	}

	void Device::Terminate()
	{
		glfwTerminate();
	}

    void Device::RunApp(const std::shared_ptr<App>& app, const Vox::Path& scenePath)
    {   
		gApplication = app;

        //! Create Window Context.
        CubbyFlow::Point2I wndSize = app->GetWindowSize();
        GLFWwindow* window = glfwCreateWindow(wndSize.x, wndSize.y, app->GetWindowTitle(), nullptr, nullptr);
        glfwMakeContextCurrent(window);

        //! Execute OpenGL API Loader.
        VoxAssert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), CURRENT_SRC_PATH_TO_STR, "Failed to initialize OpenGL");

        //! Check Essential OpenGL Extensions Support.
		auto extensions = {"GL_ARB_debug_output", "GL_EXT_texture_compression_s3tc", "GL_EXT_texture_sRGB"};
		VoxAssert(Device::CheckExtensionsSupported(extensions), CURRENT_SRC_PATH_TO_STR, "UnSupported OpenGL Extension");                      

        //! Enable OpenGL API Debugging Extension.
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
        glDebugMessageCallbackARB(GLDebug::DebugLog, nullptr);
        glDebugMessageControlARB(GL_DEBUG_SOURCE_APPLICATION_ARB, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);
	    glDebugMessageControlARB(GL_DEBUG_SOURCE_THIRD_PARTY_ARB, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);
        glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW_ARB, 0, NULL, false);  
        glDebugMessageControlARB(GL_DONT_CARE, GL_DEBUG_TYPE_OTHER_ARB, GL_DONT_CARE, 0, NULL, false);

        //! Add Created Context to the Application.
		auto ctx = std::make_shared<FrameContext>(window);        
		RegisterCallbacks(ctx); 
		app->PushFrameContextToQueue(ctx);

        //! Initialize Fluid Simulation Application.
		VoxAssert(app->Initialize(scenePath), CURRENT_SRC_PATH_TO_STR, "Application initialize failed");             

        //! Get seconds per frame from the application FPS limit.
        const double secondsPerFrame = 1.0 / app->GetFPSLimit();

        HostTimer hostTimer;
        double startTime = hostTimer.DurationInSeconds();
        double dt = 0.0;
        size_t frameCnt = 0;
        
        while (glfwWindowShouldClose(window) == GLFW_FALSE)
        {
            ++frameCnt;
            double nowTime = hostTimer.DurationInSeconds();
            dt = nowTime - startTime;

			app->UpdateFrame(dt);
            glfwPollEvents();

            if (nowTime - startTime > secondsPerFrame)
            {
                app->DrawFrame();
                glfwSwapBuffers(window);
                startTime = nowTime;
                frameCnt = 0;
            }
        }
    }

    bool Device::CheckExtensionsSupported(const std::initializer_list<const char*>& exts)
    {
		//! Get the number of extensions currently running physical device.
        int max;
        glGetIntegerv(GL_NUM_EXTENSIONS, &max);
        std::list<const char*> extensionList (exts);

		//! Get extension name and compare with given arguments.
        for (int i = 0; i < max; ++i)
        {
            const GLchar* temp = reinterpret_cast<const GLchar*>(glGetStringi(GL_EXTENSIONS, i));
            for (auto iter = begin(extensionList); iter != end(extensionList);)
            {
				//! If current queried extension name mathced with extension argument, pop.
                if (strcmp(temp, *iter) == 0) 
                    iter = extensionList.erase(iter);
                else 
                    ++iter;
            }
        }
		//! If list is still not empty after looping over extension name list, exit the program.
        if (!extensionList.empty())
        {
            for (const auto& ext : extensionList)
            {
				fprintf(stderr, "%s is not supported\n", ext);
            }
            return false;
        }

        return true;
    }

  	void Device::RegisterCallbacks(const std::shared_ptr<FrameContext>& ctx)
	{
		GLFWwindow* window = ctx->GetWindowContext();
		glfwSetFramebufferSizeCallback(window, OnWindowResized);
		glfwSetKeyCallback(window, OnKey);
		glfwSetMouseButtonCallback(window, OnMouseButton);
		glfwSetCursorPosCallback(window, OnMouseCursorPos);
		glfwSetScrollCallback(window, OnMouseScroll);
	}

    void Device::ApplyRenderStatus(const FrameContext::RenderStatus& prevStat, const FrameContext::RenderStatus& newStat)
    {
        static bool isFirstApplying = true;

        if (isFirstApplying || prevStat.isBlendEnabled != newStat.isBlendEnabled)
        {
            if (newStat.isBlendEnabled)
                glEnable(GL_BLEND);
            else
                glDisable(GL_BLEND);
        }

        if (isFirstApplying || prevStat.isDepthTestEnabled != newStat.isDepthTestEnabled)
        {
            if (newStat.isDepthTestEnabled)
                glEnable(GL_DEPTH_TEST);
            else
                glDisable(GL_DEPTH_TEST);
        }

        if (isFirstApplying || prevStat.depthFunc != newStat.depthFunc)
        {
            glDepthFunc(newStat.depthFunc);
        }

        if (isFirstApplying || prevStat.isCullingEnabled != newStat.isCullingEnabled)
        {
            if (newStat.isCullingEnabled)
                glEnable(GL_CULL_FACE);
            else
                glDisable(GL_CULL_FACE);
        }

        if (isFirstApplying || prevStat.isFrontFaceClockWise != newStat.isFrontFaceClockWise)
        {
            if (newStat.isFrontFaceClockWise)
                glFrontFace(GL_CW);
            else
                glFrontFace(GL_CCW);
        }


        if (isFirstApplying || prevStat.cullMode != newStat.cullMode)
        {
            glCullFace(newStat.cullMode);
        }

        if (isFirstApplying || (prevStat.sourceBlendFactor != newStat.sourceBlendFactor) || (prevStat.destinationBlendFactor != newStat.destinationBlendFactor))
        {
            glBlendFunc(newStat.sourceBlendFactor, newStat.destinationBlendFactor);
        }

        isFirstApplying = false;
    }

    void OnWindowResized(GLFWwindow* window, int width, int height)
	{
        UNUSED_VARIABLE(window);
		VoxAssert(!gApplication.expired(), CURRENT_SRC_PATH_TO_STR, "Global Application is Expired");
        std::shared_ptr<App> app = gApplication.lock();
        if (app)
        {
            app->SetWindowSize(width, height);
        }
	}

    void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
        UNUSED_VARIABLE(window);
        VoxAssert(!gApplication.expired(), CURRENT_SRC_PATH_TO_STR, "Global Application is Expired");
        std::shared_ptr<App> app = gApplication.lock();
        if (app)
        {
            app->SetKey(key, scancode, action, mods);
        }
	}

    void OnMouseButton(GLFWwindow* window, int button, int action, int mods)
	{
        UNUSED_VARIABLE(window);
        VoxAssert(!gApplication.expired(), CURRENT_SRC_PATH_TO_STR, "Global Application is Expired");
        std::shared_ptr<App> app = gApplication.lock();
        if (app)
        {
            app->SetMouseButton(button, action, mods);
        }
	}

    void OnMouseCursorPos(GLFWwindow* window, double x, double y)
	{
        UNUSED_VARIABLE(window);
        VoxAssert(!gApplication.expired(), CURRENT_SRC_PATH_TO_STR, "Global Application is Expired");
        std::shared_ptr<App> app = gApplication.lock();
        if (app)
        {
            app->SetMouseCursorPos(x, y);
        }
	}

    void OnMouseScroll(GLFWwindow* window, double deltaX, double deltaY)
	{
        UNUSED_VARIABLE(window); 
        VoxAssert(!gApplication.expired(), CURRENT_SRC_PATH_TO_STR, "Global Application is Expired");
        std::shared_ptr<App> app = gApplication.lock();
        if (app)
        {
            app->SetMouseScroll(deltaX, deltaY);
        }
	}
};