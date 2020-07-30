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

    void Renderer::RunApp(const std::shared_ptr<App>& app)
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
#ifdef CUBBYFLOW_MACOSX
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        Size2 wndSize = app->GetWindowSize();
        GLFWwindow* window = glfwCreateWindow(int(wndSize.x), int(wndSize.y), app->GetWindowTitle(), nullptr, nullptr);
        glfwMakeContextCurrent(window);


        VoxAssert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), CURRENT_SRC_PATH_TO_STR, "Failed to initialize OpenGL");

        glDebugMessageCallbackARB(FrameContext::DebugLog, nullptr);
        glDebugMessageControlARB(GL_DEBUG_SOURCE_APPLICATION_ARB, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);
	    glDebugMessageControlARB(GL_DEBUG_SOURCE_THIRD_PARTY_ARB, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);
        glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW_ARB, 0, NULL, true);  

		auto extensions = {"GL_ARB_debug_output"};
		VoxAssert(Renderer::CheckExtensionsSupported(extensions), CURRENT_SRC_PATH_TO_STR, "UnSupported OpenGL Extension");                      

		VoxAssert(app->Initialize(), CURRENT_SRC_PATH_TO_STR, "Application initialize failed");                      

        HostTimer hostTimer;
        DeviceTimer deviceTimer;

        double hostStartTime = hostTimer.DurationInSeconds(), deviceStartTime = deviceTimer.DurationInSeconds();
        size_t hostFrameCnt = 0, deviceFrameCnt = 0;
        for(;;) 
        {
            //! main loop
            //! Process Input
            app->DrawFrame();
            ++hostFrameCnt; ++deviceFrameCnt;

            glfwPollEvents();
            glfwSwapBuffers(window);

            double hostNowTime = hostTimer.DurationInSeconds();
            if (hostNowTime - hostStartTime > 1.0)
            {
                printf("HostTimer : %.6f ms/frame, %.3f fps\n", (hostNowTime - hostStartTime) * 1000.0 / hostFrameCnt, hostFrameCnt / (hostNowTime - hostStartTime));
                hostStartTime = hostNowTime;
                hostFrameCnt = 0;
            }

            double deviceNowTime = deviceTimer.DurationInSeconds();
            if (deviceNowTime - deviceStartTime > 1.0)
            {
                printf("DeviceTimer : %.6f ms/frame, %.3f fps\n", (deviceNowTime - deviceStartTime) * 1000.0 / deviceFrameCnt, deviceFrameCnt / (deviceNowTime - deviceStartTime));
                deviceStartTime = deviceNowTime;
                deviceFrameCnt = 0;
            }
        }
    }

    bool Renderer::CheckExtensionsSupported(const std::initializer_list<const char*>& exts)
    {
        int max;
        glGetIntegerv(GL_NUM_EXTENSIONS, &max);
        std::list<const char*> extensionList (exts);

        for (int i = 0; i < max; ++i)
        {
            const GLchar* temp = reinterpret_cast<const GLchar*>(glGetStringi(GL_EXTENSIONS, i));
            for (auto iter = extensionList.begin(); iter != extensionList.end();)
            {
                if (strcmp(temp, *iter) == 0)
                {
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
				fprintf(stderr, "%s is not supported\n", ext);
            }
            return false;
        }

        return true;
    }

    GLuint Renderer::CreateTextureFromFile(const Path& path, bool srgb)
    {
        GLuint texture;
		int width, height, numChannels;
		//! Because of differences on way to interpret image y-axis coordinate between opengl and stb.
		//! OpenGL expect 0.0 at the bottom of the y-axis, but vice-versa in stb 
		stbi_flip_vertically_on_write(true);
		unsigned char* data = stbi_load(path.ToString().c_str(), &width, &height, &numChannels, 0);

		//! If (width != 0 && height != 0 && numChannels != 0 && data != nullptr) is true, it means loading image is failed.
		VoxAssert((width != 0 && height != 0 && numChannels != 0 && data != nullptr), CURRENT_SRC_PATH_TO_STR, "Failed to load image");

		//! Create texture as GL_TEXTURE_2D
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		//! Set Anisotropy filter on texture (https://paroj.github.io/gltut/Texturing/Tut15%20Anisotropy.html)
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 1.0f);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLenum internalformat = 0, format = 0;
		switch(numChannels)
		{
		case 1: 
			internalformat = GL_R8;
			format = GL_RED;
			break;
		case 2:
			internalformat = GL_RG8;
			format = GL_RG;
			break;
		case 3:
			internalformat = (srgb) ? GL_SRGB8 : GL_RGB8;
			format = GL_RGB;
			break;
		case 4:
			internalformat = (srgb) ? GL_SRGB8_ALPHA8 : GL_RGBA8;
			format = GL_RGBA;
			break;
		default:
			break;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, GL_UNSIGNED_BYTE, static_cast<const void*>(data));

		//! Deallocate data after using it.
		stbi_image_free(data);
		return texture;
    }

    GLuint Renderer::CreateRenderBuffer(GLenum internalFormat, GLsizei width, GLsizei height)
    {
		GLuint rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
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
			VoxAssert(false, CURRENT_SRC_PATH_TO_STR, "Compile shader failed");
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
			VoxAssert(false, CURRENT_SRC_PATH_TO_STR, "Link program failed");
		}
		
		//! Detach shaders from program because they are already linked.
		glDetachShader(program, vs);
		if (gs) glDetachShader(program, gs); //! geometry shader is optional stage.
		glDetachShader(program, fs);

		return program;
    }

};