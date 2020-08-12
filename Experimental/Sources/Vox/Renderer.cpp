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
#include <Vox/DebugUtils.hpp>
#include <Vox/App.hpp>
#include <Vox/Timer.hpp>
#include <Core/Utils/Logging.h>
#include <Core/Size/Size2.h>
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

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>

using namespace CubbyFlow;

namespace Vox {

	namespace {
		std::weak_ptr<App> gApplication;
	};
	void OnWindowResized(GLFWwindow* window, int width, int height);
    void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods);
    void OnMouseButton(GLFWwindow* window, int button, int action, int mods);
    void OnMouseCursorPos(GLFWwindow* window, double x, double y);
    void OnMouseScroll(GLFWwindow* window, double deltaX, double deltaY);

	void Renderer::Initialize()
	{
		VoxAssert(glfwInit(), CURRENT_SRC_PATH_TO_STR, "GLFW initialization failed");

        int major, minor, revision;
        glfwGetVersion(&major, &minor, &revision);
        CUBBYFLOW_INFO << "GLFW version : " << major << "." << minor << "." << revision;

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

	void Renderer::Terminate()
	{
		glfwTerminate();
	}

    void Renderer::RunApp(const std::shared_ptr<App>& app, const Vox::Path& scenePath)
    {   
		gApplication = app;

        Vector2I wndSize = app->GetWindowSize();
        GLFWwindow* window = glfwCreateWindow(wndSize.x, wndSize.y, app->GetWindowTitle(), nullptr, nullptr);
        glfwMakeContextCurrent(window);

        VoxAssert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), CURRENT_SRC_PATH_TO_STR, "Failed to initialize OpenGL");

		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
        glDebugMessageCallbackARB(GLDebug::DebugLog, nullptr);
        glDebugMessageControlARB(GL_DEBUG_SOURCE_APPLICATION_ARB, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);
	    glDebugMessageControlARB(GL_DEBUG_SOURCE_THIRD_PARTY_ARB, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);
        glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW_ARB, 0, NULL, true);  

		auto extensions = {"GL_ARB_debug_output"};
		VoxAssert(Renderer::CheckExtensionsSupported(extensions), CURRENT_SRC_PATH_TO_STR, "UnSupported OpenGL Extension");                      

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

    bool Renderer::CheckExtensionsSupported(const std::initializer_list<const char*>& exts)
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

	GLuint Renderer::CreateTexture(GLsizei width, GLsizei height, const PixelFmt pf, const void* data, bool multisample)
	{
		const PixelFmtDesc* pfd = GetPixelFmtDesc(pf);

		const GLenum target = multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(target, texture);
		//! https://paroj.github.io/gltut/Texturing/Tut15%20Anisotropy.html
		//! ARB_texture_filter_anisotropic extension.
		//! Apply anisotropic filter to the texture.
		// glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 1.0f);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (multisample)
		{
			GLsizei maxSamples;
			glGetIntegerv(GL_MAX_COLOR_TEXTURE_SAMPLES, &maxSamples);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, std::min(maxSamples, 4), pfd->internal, width, height, GL_TRUE);
		}
			
		else
			glTexImage2D(GL_TEXTURE_2D, 0, pfd->internal, width, height, 0, pfd->format, pfd->type, data);
		
		glBindTexture(target, 0);

		return texture;
	}

    GLuint Renderer::CreateTextureFromFile(const Path& path, bool srgb)
    {
		int width, height, numChannels;
		//! Because of differences on way to interpret image y-axis coordinate between opengl and stb.
		//! OpenGL expect 0.0 at the bottom of the y-axis, but vice-versa in stb 
		stbi_flip_vertically_on_write(true);
		unsigned char* data = stbi_load(path.ToString().c_str(), &width, &height, &numChannels, 0);

		//! If (width != 0 && height != 0 && numChannels != 0 && data != nullptr) is true, it means loading image is failed.
		VoxAssert((width != 0 && height != 0 && numChannels != 0 && data != nullptr), CURRENT_SRC_PATH_TO_STR, "Failed to load image");

		PixelFmt pf;
		switch(numChannels)
		{
		case 1: 
			pf = PixelFmt::PF_R8;
			break;
		case 2:
			pf = PixelFmt::PF_RG8;
			break;
		case 3:
			pf = (srgb) ? PixelFmt::PF_SRGB8 : PixelFmt::PF_RGB8;
			break;
		case 4:
			pf = (srgb) ? PixelFmt::PF_SRGB8_ALPHA8 : PixelFmt::PF_RGBA8;
			break;
		default:
			VoxAssert(false, CURRENT_SRC_PATH_TO_STR, "Unknown Pixel Format");
			break;
		}

		GLuint texture = CreateTexture(static_cast<GLsizei>(width), static_cast<GLsizei>(height), pf, data);
		//! Deallocate data after using it.
		stbi_image_free(data);
		return texture;
    }

    GLuint Renderer::CreateRenderBuffer(GLsizei width, GLsizei height, const PixelFmt pf, bool multisample)
    {
		const PixelFmtDesc* pfd = GetPixelFmtDesc(pf);

		GLuint rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		if (multisample)
		{
			GLsizei maxSamples;
			glGetIntegerv(GL_MAX_INTEGER_SAMPLES, &maxSamples);
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, std::min(maxSamples, 4), pfd->internal, width, height);
		}
		else
			glRenderbufferStorage(GL_RENDERBUFFER, pfd->internal, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		return rbo;
    }

	GLuint Renderer::CreateShaderFromSource(const char* source, GLenum shaderType)
	{
		//! Create shader and pass shader source code and finally compile.
		GLuint shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &source, nullptr);
		glCompileShader(shader);

		//! Check whether if there is compile error or not.
		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		//! If shader compile was failed, print shader compile info log.
		if (!success) 
		{
			GLint length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			std::vector<GLchar> info(length);
			glGetShaderInfoLog(shader, static_cast<GLsizei>(length), nullptr, info.data());
			VoxAssert(false, CURRENT_SRC_PATH_TO_STR, info.data());
		}

		return shader;
	}

    GLuint Renderer::CreateShaderFromFile(const Path& path, GLenum shaderType)
    {
		//! Load shader source file.
		std::ifstream file(path.ToString());
		VoxAssert(file.is_open(), CURRENT_SRC_PATH_TO_STR, "Open Shader source file failed");

		//! Read file contents to string.
		std::istringstream isstr;
		isstr >> file.rdbuf();
		const char* src = isstr.str().c_str();
		VoxAssert(src, CURRENT_SRC_PATH_TO_STR, "Loaded Shader source file is empty");

		return CreateShaderFromSource(src, shaderType);
    }

    GLuint Renderer::CreateProgram(GLuint vs, GLuint gs, GLuint fs)
    {
		//! Create program and attach shaders and finally link.
		GLuint program = glCreateProgram();
		glAttachShader(program, vs);
		if (gs) //! geometry shader is optional stage.
			glAttachShader(program, gs); 
		glAttachShader(program, fs);
		glLinkProgram(program);
		
		//! Check whether if there is link error or not.
		int success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		//! If program linking was failed, print program linking info log.
		if (!success)
		{
			GLint length;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
			std::vector<GLchar> info(length);
			glGetProgramInfoLog(program, static_cast<GLsizei>(length), nullptr, info.data());
			VoxAssert(false, CURRENT_SRC_PATH_TO_STR, info.data());
		}
		
		//! Detach shaders from program because they are already linked.
		glDetachShader(program, vs);
		if (gs) glDetachShader(program, gs); //! geometry shader is optional stage.
		glDetachShader(program, fs);

		return program;
    }

    void Renderer::SaveTextureToRGB(const char* path, int width, int height)
	{
		static unsigned char* cache = nullptr;
		static int prevWidth = width;
		static int prevHeight = height;
		constexpr int numChannels = 3;

		//! There is no need to re-allocate heap memory if width and height are unchanged.
		if (!cache || ((prevWidth != width) || (prevHeight != height)))
		{
			prevWidth = width; prevHeight = height; 
			cache = reinterpret_cast<unsigned char*>(::realloc(cache, width * height * numChannels * sizeof(unsigned char)));
		}
		
		//! Read pixels to client memory(heap-pre-allocated data).
		glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, static_cast<void*>(cache));
		//! Save read image data into rgba png.
		stbi_flip_vertically_on_write(true);
		stbi_write_png(path, width, height, numChannels, static_cast<const void*>(cache), 0);
	}
  
  	void Renderer::RegisterCallbacks(const std::shared_ptr<FrameContext>& ctx)
	{
		GLFWwindow* window = ctx->GetWindowContext();
		glfwSetFramebufferSizeCallback(window, OnWindowResized);
		glfwSetKeyCallback(window, OnKey);
		glfwSetMouseButtonCallback(window, OnMouseButton);
		glfwSetCursorPosCallback(window, OnMouseCursorPos);
		glfwSetScrollCallback(window, OnMouseScroll);
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