/*************************************************************************
> File Name: Renderer.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: OpenGL Renderere class managing whole context and states.
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Core/Renderer.hpp>
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

	namespace Detail
	{
		std::string GetPathDirectory(const std::string& path)
		{
			size_t pos = path.find_last_of('/');
			return path.substr(0, pos + 1);
		}

		std::string PreprocessShaderInclude(const Vox::Path& path, std::string includePrefix = "#include")
		{
			includePrefix += ' ';

			std::cout << GetPathDirectory(path.ToString());
			//! Load shader source file.
			std::ifstream file(path.ToString());
			VoxAssert(file.is_open(), CURRENT_SRC_PATH_TO_STR, "Open Shader source file failed");

			std::string fullSourceCode = "";
			std::string temp;
			while (std::getline(file, temp))
			{
				if (temp.find(includePrefix) != std::string::npos) //! temp string contain "#include "
				{
					temp.erase(0, includePrefix.length());

					std::string filePath = GetPathDirectory(path.ToString());
					filePath += temp;
					std::string includeSrc = PreprocessShaderInclude(Vox::Path(temp));

					fullSourceCode += includeSrc;
					continue;
				}

				fullSourceCode += temp + '\n';
			}

			fullSourceCode += '\0';

			return fullSourceCode;
		}
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
		{
			if (pfd->compressed)
			{
				const int pitch = (width >> 2) * pfd->size;
				const int rows = height >> 2;
				glCompressedTexImage2D(GL_TEXTURE_2D, 0, pfd->internal, width, height, 0, pitch * rows, data);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, pfd->internal, width, height, 0, pfd->format, pfd->type, data);
			}
		}

		glBindTexture(target, 0);

		return texture;
	}

	GLuint Renderer::CreateVolumeTexture(GLsizei width, GLsizei height, GLsizei depth, const PixelFmt pf, const void* data)
	{
		const PixelFmtDesc* pfd = GetPixelFmtDesc(pf);
		
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_3D, texture);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage3D(GL_TEXTURE_3D, 0, pfd->internal, width, height, depth, 0, pfd->format, pfd->type, data);
		glBindTexture(GL_TEXTURE_3D, 0);
		
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
		std::string fullSourceCode = Detail::PreprocessShaderInclude(path);
		return CreateShaderFromSource(fullSourceCode.c_str(), shaderType);
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

	GLuint Renderer::CreateFrameBuffer()
	{
		GLuint fbo;
		glGenFramebuffers(1, &fbo);
		return fbo;
	}
	
	void Renderer::ReadFrameBuffer(int width, int height, int mips, const PixelFmt pf, void* data)
	{
        UNUSED_VARIABLE(mips);
        const PixelFmtDesc* desc = GetPixelFmtDesc(pf);
		glReadPixels(0, 0, width, height, desc->format, desc->type, data);
	}

};