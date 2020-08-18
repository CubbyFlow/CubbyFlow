/*************************************************************************
> File Name: FrameContext.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Frame Context class for containing context information in one frame.
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/FrameContext.hpp>
#include <Vox/DebugUtils.hpp>
#include <Vox/FrameBuffer.hpp>
#include <Vox/PerspectiveCamera.hpp>
#include <Vox/Program.hpp>
#include <Vox/FileSystem.hpp>
#include <Vox/VoxScene.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Vox {

	FrameContext::FrameContext(GLFWwindow* windowCtx)
		: _windowCtx(windowCtx), _renderMode(GL_POINTS)
	{
		//! Do nothing.
	}

    FrameContext::~FrameContext()
	{
		for (auto iter : _textureMap)
			glDeleteTextures(1, &(iter.second));

		if (_windowCtx)
			glfwDestroyWindow(_windowCtx);
	}

    void FrameContext::SetWindowContextShouldClose(bool bShutdown)
	{
		glfwSetWindowShouldClose(_windowCtx, bShutdown);
	}
	
    void FrameContext::MakeContextCurrent() const
	{
		glfwMakeContextCurrent(_windowCtx);
	}

	void FrameContext::SetRenderMode(GLenum mode)
	{
		_renderMode = mode;
	}

    GLenum FrameContext::GetRenderMode() const
	{
		return _renderMode;
	}

	void FrameContext::AddShaderProgram(const std::string& name, GLuint program)
	{
		_programMap.emplace(name, std::make_shared<Program>(program));
	}

	const std::weak_ptr<Program>& FrameContext::GetCurrentProgram() const
	{
		return _currentProgram;
	}

	void FrameContext::MakeProgramCurrent(const std::string& name)
	{
		auto iter = _programMap.find(name);
		//! If given name does not exist, print call stack and error message.
		VoxAssert(iter != _programMap.end(), CURRENT_SRC_PATH_TO_STR, std::string("No Shader Program ") + name);
		_currentProgram = iter->second;
		//! Check whether if dependent shared_ptr expired or not.
		if (!_currentProgram.expired())
		{
			_currentProgram.lock()->UseProgram();
		}
	}

	void FrameContext::AddTexture(const std::string& name, GLuint texture)
	{
		_textureMap.emplace(name, texture);
	}

    void FrameContext::BindTextureToSlot(const std::string& name, GLenum target, GLenum slot)
	{
		//! Find given program name in the program map.
		auto iter = _textureMap.find(name);
		//! If given name does not exist, print call stack and error message.
		VoxAssert(iter != _textureMap.end(), CURRENT_SRC_PATH_TO_STR, std::string("No Shader Program ") + name);
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(target, iter->second);
	}

	void FrameContext::UpdateProgramCamera(const std::shared_ptr<PerspectiveCamera>& camera)
	{
		//! Check whether if dependent shared_ptr expired or not.
		if (!_currentProgram.expired())
			_currentProgram.lock()->SendUniformVariable("ViewProjection", camera->GetViewProjectionMatrix());
	}

    void FrameContext::AddFrameBuffer(const std::string& name)
	{
		_fboMap.emplace(name, std::make_shared<FrameBuffer>());
	}

    void FrameContext::BindFrameBuffer(const std::string& name, GLenum target)
	{
		if (name == "DefaultPass")
		{
			glBindFramebuffer(target, 0);
		}
		else
		{
			//! Find given program name in the program map.
			auto iter = _fboMap.find(name);
			//! If given name does not exist, print call stack and error message.
			VoxAssert(iter != _fboMap.end(), CURRENT_SRC_PATH_TO_STR, std::string("No Frame Buffer Object ") + name);
			_currentFrameBuffer = iter->second;
			if (!_currentFrameBuffer.expired())
				_currentFrameBuffer.lock()->BindFrameBuffer(target);
		}
	}
    
	const std::weak_ptr<FrameBuffer>& FrameContext::GetCurrentFrameBuffer() const
	{
		return _currentFrameBuffer;
	}
  	
	GLFWwindow* FrameContext::GetWindowContext()
	{
		return _windowCtx;
	}
};
