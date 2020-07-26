/*************************************************************************
> File Name: Renderer.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: OpenGL Renderere class managing whole context and states.
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Renderer.hpp>
#include <Vox/FrameContext.hpp>
#include <Vox/App.hpp>
#include <Core/Utils/Timer.h>
#include <Core/Utils/Logging.h>
#include <Core/Size/Size2.h>
#include <cassert>
#include <list>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstring>

using namespace CubbyFlow;

namespace Vox {

    bool Renderer::RunApp(const std::shared_ptr<App>& app)
    {   
        if (!glfwInit())
        {
            CUBBYFLOW_ERROR << "GLFW initialization failed";
            return false;
        }

        int major, minor, revision;
        glfwGetVersion(&major, &minor, &revision);
        CUBBYFLOW_INFO << "GLFW version : " << major << "." << minor << "." << revision;


        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	    glfwWindowHint(GLFW_SAMPLES, 4);
	    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#ifdef CUBBYFLOW_MACOSX
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        Size2 wndSize = app->GetWindowSize();
        GLFWwindow* window = glfwCreateWindow(int(wndSize.x), int(wndSize.y), app->GetWindowTitle(), nullptr, nullptr);
        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
        {
            CUBBYFLOW_ERROR << "Failed to initialize OpenGL";
            return false;
        }

        glDebugMessageCallbackARB(FrameContext::DebugLog, nullptr);
        glDebugMessageControlARB(GL_DEBUG_SOURCE_APPLICATION_ARB, GL_DONT_CARE,
						GL_DONT_CARE, 0, NULL, true);
	    glDebugMessageControlARB(GL_DEBUG_SOURCE_THIRD_PARTY_ARB, GL_DONT_CARE,
						GL_DONT_CARE, 0, NULL, true);
        if(!Renderer::CheckExtensionsSupported({"GL_ARB_debug_output"}))
            return false;

        Timer timer;
        double startTime = timer.DurationInSeconds();
        size_t frameCnt = 0;
        for(;;) 
        {
            //! main loop
            //! Process Input
            app->DrawFrame();
            ++frameCnt;

            glfwPollEvents();
            glfwSwapBuffers(window);

            double nowTime = timer.DurationInSeconds();
            if (nowTime - startTime > 1.0)
            {
                printf("%.6f ms/frame, %.3f fps\n", (nowTime - startTime) * 1000.0 / frameCnt, frameCnt / (nowTime - startTime));
                startTime = nowTime;
                frameCnt = 0;
            }
        }

        return true;
    }

    bool Renderer::CheckExtensionsSupported(const std::initializer_list<std::string>& exts)
    {
        int max;
        glGetIntegerv(GL_NUM_EXTENSIONS, &max);
        std::list<std::string> extensionList (exts);

        for (int i = 0; i < max; ++i)
        {
            const GLchar* temp = reinterpret_cast<const GLchar*>(glGetStringi(GL_EXTENSIONS, i));
            for (auto iter = extensionList.begin(); iter != extensionList.end();)
            {
                if (strcmp(temp, (*iter).c_str()) == 0)
                {
                    CUBBYFLOW_INFO << temp << " is Supported";
                    iter = extensionList.erase(iter);
                }
                else
                {
                    ++iter;
                }
            }
        }

        if (!extensionList.empty())
        {
            for (const auto& ext : extensionList)
            {
                CUBBYFLOW_ERROR << ext << " is not Supported";
            }
            return false;
        }

        return true;
    }
};