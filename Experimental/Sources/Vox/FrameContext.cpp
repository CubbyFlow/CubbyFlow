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
		_defaultPass = CreateFrameBuffer("DefaultPass", 0);
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
	
	std::shared_ptr<FrameBuffer> FrameContext::CreateFrameBuffer(const std::string& name, GLuint id)
	{
		const unsigned int sid = VoxStringID(name);
		std::lock_guard<std::mutex> guard(_m);

		auto instance = _frameBufferMap[sid].lock();
		if (!instance) _frameBufferMap[sid] = instance = std::make_shared<FrameBuffer>(id);
		return instance;
	}

    std::shared_ptr<Mesh> FrameContext::CreateMesh(const std::string& name, const MeshShape& shape, VertexFormat format, bool bInterleaved)
	{
		const unsigned int sid = VoxStringID(name);
		std::lock_guard<std::mutex> guard(_m);

		auto instance = _meshMap[sid].lock();
		if (!instance) _meshMap[sid] = instance = std::make_shared<Mesh>(shape, format, bInterleaved);
		return instance;
	}

    std::shared_ptr<Texture> FrameContext::CreateTexture(const std::string& name, GLuint target, GLuint id)
	{
		const unsigned int sid = VoxStringID(name);
		std::lock_guard<std::mutex> guard(_m);

		auto instance = _textureMap[sid].lock();
		if (!instance) _textureMap[sid] = instance = std::make_shared<Texture>(target, id);
		return instance;
	}

    std::shared_ptr<Program> FrameContext::CreateProgram(const std::string& name, GLuint program)
	{
		const unsigned int sid = VoxStringID(name);
		std::lock_guard<std::mutex> guard(_m);

		auto instance = _programMap[sid].lock();
		if (!instance) _programMap[sid] = instance = std::make_shared<Program>(program);
		return instance;
	}
	
    std::shared_ptr<FrameBuffer> FrameContext::GetFrameBuffer(const std::string& name)
	{
		const unsigned int sid = VoxStringID(name);
		std::lock_guard<std::mutex> guard(_m);
	
		std::shared_ptr<FrameBuffer> instance = _frameBufferMap[sid].lock();
		VoxAssert(instance, CURRENT_SRC_PATH_TO_STR, "Unregistered Frame Buffer [" + name + "]");
		return instance;
	}

    std::shared_ptr<Mesh> FrameContext::GetMesh(const std::string& name)
	{
		const unsigned int sid = VoxStringID(name);
		std::lock_guard<std::mutex> guard(_m);

		std::shared_ptr<Mesh> instance = _meshMap[sid].lock();
		VoxAssert(instance, CURRENT_SRC_PATH_TO_STR, "Unregistered Mesh [" + name + "]");
		return instance;
	}
    std::shared_ptr<Texture> FrameContext::GetTexture(const std::string& name)
	{
		const unsigned int sid = VoxStringID(name);
		std::lock_guard<std::mutex> guard(_m);

		std::shared_ptr<Texture> instance = _textureMap[sid].lock();
		VoxAssert(instance, CURRENT_SRC_PATH_TO_STR, "Unregistered Texture [" + name + "]");
		return instance;
	}

    std::shared_ptr<Program> FrameContext::GetProgram(const std::string& name)
	{
		const unsigned int sid = VoxStringID(name);
		std::lock_guard<std::mutex> guard(_m);

		std::shared_ptr<Program> instance = _programMap[sid].lock();
		VoxAssert(instance, CURRENT_SRC_PATH_TO_STR, "Unregistered Program [" + name + "]");
		return instance;
	}

	void FrameContext::BindSceneToContext(const std::shared_ptr<VoxScene>& scene)
	{
		_scene = scene;
	}

    std::shared_ptr<VoxScene> FrameContext::GetContextScene()
	{
		std::lock_guard<std::mutex> guard(_m);
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
