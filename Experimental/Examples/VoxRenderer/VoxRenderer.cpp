/*************************************************************************
> File Name: VoxRenderer.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Particle Viewer
> Created Time: 2020/08/11
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include "VoxRenderer.hpp"
#include <Vox/Utils/FileSystem.hpp>
#include <Vox/Core/FrameContext.hpp>
#include <Vox/Core/Renderer.hpp>
#include <Vox/Scene/VoxScene.hpp>
#include <Vox/Core/Program.hpp>
#include <Vox/Core/FrameBuffer.hpp>
#include <Vox/Core/Texture.hpp>
#include <Vox/Core/ShaderPreset.hpp>
#include <Vox/Core/FluidBuffer.hpp>
#include <Vox/Core/PostProcessing.hpp>
#include <Vox/Core/S3TextureCompression.hpp>
#include <Vox/Utils/SequentialFrameCapture.hpp>
#include <Vox/Scene/GeometryCache.hpp>
#include <Vox/Scene/GeometryCacheManager.hpp>
#include <glad/glad.h>

VoxRenderer::VoxRenderer()
{
    //! Do nothing.
}

VoxRenderer::~VoxRenderer()
{
    //! Do nothing.
}

bool VoxRenderer::Initialize(const Vox::Path& scenePath)
{
    if (!App::Initialize(scenePath))
        return false;
    
    std::shared_ptr<Vox::FrameContext> ctx = Vox::App::PopFrameContextFromQueue();
    ctx->BindSceneToContext(_scene);

    _cacheMgr = _scene->GetSceneObject<Vox::GeometryCacheManager>("fluidAnim");
    for (size_t i = 0; i < _cacheMgr->GetNumberOfCache(); ++i)
    {
        auto& cache = _cacheMgr->GetGeometryCache(i);
        cache->InterleaveData(Vox::VertexFormat::Position3Normal3);
    }

    _buffer.reset(new Vox::FluidBuffer(Vox::FluidBuffer::kDefaultNumBuffer));
    GLuint vs = Vox::Renderer::CreateShaderFromSource(Vox::kFluidMeshShaders[0], GL_VERTEX_SHADER);
    GLuint fs = Vox::Renderer::CreateShaderFromSource(Vox::kFluidMeshShaders[1], GL_FRAGMENT_SHADER);
    _meshShader = std::make_shared<Vox::Program>(Vox::Renderer::CreateProgram(vs, 0, fs));
    glDeleteShader(vs);
    glDeleteShader(fs);

    GLuint mainPassColorTexture = Vox::Renderer::CreateTexture(_windowSize.x, _windowSize.y, Vox::PixelFmt::PF_RGBA8, nullptr, false);
    _screenTexture = std::make_shared<Vox::Texture>(GL_TEXTURE_2D, mainPassColorTexture);
    GLuint mainPassRBO = Vox::Renderer::CreateRenderBuffer(_windowSize.x, _windowSize.y, Vox::PixelFmt::PF_DEPTH_COMPONENT24_STENCIL8, false);

    _mainPass = std::make_shared<Vox::FrameBuffer>(Vox::Renderer::CreateFrameBuffer());
    _mainPass->AttachTexture(0, _screenTexture, false);
    _mainPass->AttachRenderBuffer(mainPassRBO);
    VoxAssert(_mainPass->ValidateFrameBufferStatus(), CURRENT_SRC_PATH_TO_STR, "Frame Buffer Status incomplete");

    _postProcessing.reset(new Vox::PostProcessing());
    _postProcessing->Initialize(ctx);

    auto status = ctx->GetRenderStatus();
    status.primitive = GL_TRIANGLES;
    status.cullMode = GL_BACK;
    status.isDepthTestEnabled = true;
    ctx->SetRenderStatus(status);
    
    Vox::App::PushFrameContextToQueue(ctx);
    return true;
}

void VoxRenderer::DrawFrame() 
{
    std::shared_ptr<Vox::FrameContext> ctx = Vox::App::PopFrameContextFromQueue();
    ctx->MakeContextCurrent();
    //! Main RenderPass

    _mainPass->BindFrameBuffer(GL_FRAMEBUFFER);
    {
        Vox::App::BeginFrame(ctx);
        glViewport(0, 0, _windowSize.x, _windowSize.y);

        if (_buffer->CheckFence(50000))
        {
            _meshShader->BindProgram(_scene);
            _buffer->AsyncBufferTransfer(_cacheMgr);
            _buffer->DrawFrame(ctx);
            _buffer->AdvanceFrame();
        }
        
        Vox::App::EndFrame(ctx);
    }

    //! Screen Pass
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    {
        Vox::App::BeginFrame(ctx);
        glViewport(0, 0, _windowSize.x, _windowSize.y);
        _postProcessing->DrawFrame(ctx, _screenTexture);
        _frameCapture->CaptureFrameBuffer(_windowSize.x, _windowSize.y, 1, Vox::PixelFmt::PF_BGRA8);
        _frameCapture->WriteCurrentCaptureToDDS("./VoxRenderer_output/result%06d.dds");
        Vox::App::EndFrame(ctx);
    }

    if (_frameCapture->GetCurrentFrameIndex() == _cacheMgr->GetNumberOfCache()) ctx->SetWindowContextShouldClose(true);

    Vox::App::PushFrameContextToQueue(ctx);
}