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

};