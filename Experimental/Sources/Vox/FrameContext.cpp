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
#include <Vox/PerspectiveCamera.hpp>
#include <Vox/FrameBuffer.hpp>
#include <Vox/Texture.hpp>
#include <Vox/Program.hpp>
#include <Vox/FileSystem.hpp>
#include <Vox/Mesh.hpp>
#include <Vox/VoxScene.hpp>
#include <Vox/Device.hpp>
#include <Vox/StringID.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Vox {

	FrameContext::RenderStatus::RenderStatus()
		: isFrontFaceClockWise(true),
		  isBlendEnabled(true),
		  isDepthTestEnabled(true),
		  cullMode(GL_BACK),
		  sourceBlendFactor(GL_SRC_ALPHA),
		  destinationBlendFactor(GL_ONE_MINUS_SRC_ALPHA),
		  primitive(GL_TRIANGLE_STRIP) {};

	FrameContext::FrameContext(GLFWwindow* windowCtx)
		: _windowCtx(windowCtx)
	{
		//! Do nothing
	}

    FrameContext::~FrameContext()
	{
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
  	
	GLFWwindow* FrameContext::GetWindowContext()
	{
		return _windowCtx;
	}

	void FrameContext::BindSceneToContext(const std::shared_ptr<VoxScene>& scene)
	{
		_scene = scene;
	}

    std::shared_ptr<VoxScene> FrameContext::GetContextScene()
	{
		auto instance = _scene.lock();
		VoxAssert(instance != nullptr, CURRENT_SRC_PATH_TO_STR, "Vox Scene expired");
		return instance;
	}
	
    FrameContext::RenderStatus FrameContext::GetRenderStatus() const
	{
		return _renderStatus;
	}

    void FrameContext::SetRenderStatus(RenderStatus newRenderStatus)
	{	
		Device::ApplyRenderStatus(_renderStatus, newRenderStatus);
		_renderStatus = newRenderStatus;
	}
};
