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
#include <Vox/FileSystem.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Vox {

	FrameContext::FrameContext(GLFWwindow* windowCtx)
		: _windowCtx(windowCtx), _renderMode(GL_POINTS)
	{

	}

    FrameContext::~FrameContext()
	{
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

	void FrameContext::MakeProgramCurrent(GLuint program)
	{
		_currentProgram = program;
		glUseProgram(_currentProgram);
	}

	void FrameContext::UpdateProgramCamera(const PerspectiveCamera& camera)
	{
		GLint loc = glGetUniformLocation(_currentProgram, "ViewProjection");
		VoxAssert(loc != -1, CURRENT_SRC_PATH_TO_STR, "No Uniform Variable with name 'ViewProjection'");

		glUniformMatrix4fv(loc, 1, false, camera.GetViewProjectionMatrix().data());
	}

};