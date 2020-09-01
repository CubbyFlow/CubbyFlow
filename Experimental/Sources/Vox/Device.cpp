/*************************************************************************
> File Name: Device.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: OpenGL Device class related to graphics device
> Created Time: 2020/08/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Device.hpp>
#include <Vox/FrameContext.hpp>
#include <Vox/DebugUtils.hpp>
#include <Vox/App.hpp>
#include <Vox/Timer.hpp>
#include <Core/Utils/Logging.hpp>
#include <Core/Point/Point2.hpp>
#include <Core/Size/Size2.hpp>
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
#include <Vox/TextureIO.hpp>

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

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
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

        CubbyFlow::Point2I wndSize = app->GetWindowSize();
        GLFWwindow* window = glfwCreateWindow(wndSize.x, wndSize.y, app->GetWindowTitle(), nullptr, nullptr);
        glfwMakeContextCurrent(window);

        VoxAssert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), CURRENT_SRC_PATH_TO_STR, "Failed to initialize OpenGL");

		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
        glDebugMessageCallbackARB(GLDebug::DebugLog, nullptr);
        glDebugMessageControlARB(GL_DEBUG_SOURCE_APPLICATION_ARB, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);
	    glDebugMessageControlARB(GL_DEBUG_SOURCE_THIRD_PARTY_ARB, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);
        glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW_ARB, 0, NULL, true);  

		auto extensions = {"GL_ARB_debug_output", "GL_EXT_texture_compression_s3tc", "GL_EXT_texture_sRGB"};
		VoxAssert(Device::CheckExtensionsSupported(extensions), CURRENT_SRC_PATH_TO_STR, "UnSupported OpenGL Extension");                      

		auto ctx = std::make_shared<FrameContext>(window);        
		RegisterCallbacks(ctx); 
		app->PushFrameContextToQueue(ctx);

		VoxAssert(app->Initialize(scenePath), CURRENT_SRC_PATH_TO_STR, "Application initialize failed");             

        HostTimer hostTimer;
        double startTime = hostTimer.DurationInSeconds();
        size_t frameCnt = 0;
        //! main loop
        for(;;) 
        {
			if (glfwWindowShouldClose(window))
				break;

            //! app->ProcessInput();
			app->UpdateFrame();
            app->DrawFrame();
            ++frameCnt;

            glfwSwapBuffers(window);
            glfwPollEvents();

            double nowTime = hostTimer.DurationInSeconds();
            if (nowTime - startTime > 1.0)
            {
                printf("HostTimer : %.6f ms/frame, %.3f fps\n", (nowTime - startTime) * 1000.0 / frameCnt, frameCnt / (nowTime - startTime));
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
        if (prevStat.isBlendEnabled != newStat.isBlendEnabled)
        {
            if (newStat.isBlendEnabled)
                glEnable(GL_BLEND);
            else
                glDisable(GL_BLEND);
        }

        if (prevStat.isDepthTestEnabled != newStat.isDepthTestEnabled)
        {
            if (newStat.isDepthTestEnabled)
                glEnable(GL_DEPTH_TEST);
            else
                glDisable(GL_DEPTH_TEST);
        }

        if (prevStat.isFrontFaceClockWise != newStat.isFrontFaceClockWise)
        {
            if (newStat.isFrontFaceClockWise)
                glFrontFace(GL_CW);
            else
                glFrontFace(GL_CCW);
        }

        if (prevStat.cullMode != newStat.cullMode)
        {
            glCullFace(newStat.cullMode);
        }

        if ((prevStat.sourceBlendFactor != newStat.sourceBlendFactor) || (prevStat.destinationBlendFactor != newStat.isBlendEnabled))
        {
            glBlendFunc(newStat.sourceBlendFactor, newStat.destinationBlendFactor);
        }
    }

    void OnWindowResized(GLFWwindow* window, int width, int height)
	{
		VoxAssert(!gApplication.expired(), CURRENT_SRC_PATH_TO_STR, "Global Application is Expired");
		(void)width;(void)window;(void)height;
	}

    void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		VoxAssert(!gApplication.expired(), CURRENT_SRC_PATH_TO_STR, "Global Application is Expired");
		(void)window;(void)key;(void)scancode;(void)action;(void)mods;
	}

    void OnMouseButton(GLFWwindow* window, int button, int action, int mods)
	{
		VoxAssert(!gApplication.expired(), CURRENT_SRC_PATH_TO_STR, "Global Application is Expired");
		(void)window;(void)button;(void)action;(void)mods;
	}

    void OnMouseCursorPos(GLFWwindow* window, double x, double y)
	{
		VoxAssert(!gApplication.expired(), CURRENT_SRC_PATH_TO_STR, "Global Application is Expired");
		(void)window;(void)x;(void)y;
	}

    void OnMouseScroll(GLFWwindow* window, double deltaX, double deltaY)
	{
		VoxAssert(!gApplication.expired(), CURRENT_SRC_PATH_TO_STR, "Global Application is Expired");
		(void)window;(void)deltaX;(void)deltaY;
	}

};