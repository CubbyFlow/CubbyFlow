/*************************************************************************
> File Name: FrameContext.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Frame Context class for containing context information in one frame.
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Core/FrameContext.hpp>
#include <Vox/Utils/DebugUtils.hpp>
#include <Vox/Camera/PerspectiveCamera.hpp>
#include <Vox/Core/FrameBuffer.hpp>
#include <Vox/Core/Texture.hpp>
#include <Vox/Core/Program.hpp>
#include <Vox/Utils/FileSystem.hpp>
#include <Vox/Core/Mesh.hpp>
#include <Vox/Scene/VoxScene.hpp>
#include <Vox/Core/Device.hpp>
#include <Vox/Utils/StringID.hpp>
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
