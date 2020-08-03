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
#include <Vox/Program.hpp>
#include <Vox/FileSystem.hpp>
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
		for (auto& fbo : _fbos)
			fbo.reset();
		_fbos.clear();

		for (auto iter : _textureMap)
			glDeleteTextures(1, &(iter.second));
		_textureMap.clear();

		for (auto iter : _programMap)
			iter.second.reset();
		_programMap.clear();

		//! glfw destroy
		if (_windowCtx)
		{
			glfwDestroyWindow(_windowCtx);
		}
		glfwTerminate();
	}

    void FrameContext::MakeContextCurrent() const
	{
		glfwMakeContextCurrent(_windowCtx);
	}

    GLenum FrameContext::GetRenderMode() const
	{
		return _renderMode;
	}

	void FrameContext::AddShaderProgram(const std::string& name, GLuint program)
	{
		_programMap.emplace(name, std::make_shared<Program>(program));
	}

	void FrameContext::MakeProgramCurrent(const std::string& name)
	{
		//! Find given program name in the program map.
		auto iter = _programMap.find(name);
		//! If given name does not exist, print call stack and error message.
		VoxAssert(iter != _programMap.end(), CURRENT_SRC_PATH_TO_STR, std::string("No Shader Program ") + name);
		_currentProgram = iter->second;
		//! Check whether if dependent shared_ptr expired or not.
		if (!_currentProgram.expired())
			_currentProgram.lock()->UseProgram();
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

	void FrameContext::UpdateProgramCamera(const PerspectiveCamera& camera)
	{
		//! Check whether if dependent shared_ptr expired or not.
		if (!_currentProgram.expired())
			_currentProgram.lock()->SendUniformVariable("ViewProjection", camera.GetViewProjectionMatrix());
	}

	void FrameContext::AddFrameBuffer(std::shared_ptr<FrameBuffer> fbo)
	{
		_fbos.push_back(fbo);
        _fboIterator = _fbos.begin();
	}

	void FrameContext::BindNextFrameBuffer(GLenum target)
	{
		//! If there is no frame buffer objects registered, log debug message.
		VoxAssert(_fbos.size() != 0, CURRENT_SRC_PATH_TO_STR, "At least one frame buffer object must exist");

		auto& fbo = *_fboIterator;
		fbo->BindFrameBuffer(target);

		if (_fboIterator + 1 == _fbos.end())
		{
			_fboIterator = _fbos.begin();
		}
	}
};