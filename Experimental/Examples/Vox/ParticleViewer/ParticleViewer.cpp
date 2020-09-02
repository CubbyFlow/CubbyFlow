/*************************************************************************
> File Name: ParticleViewer.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Particle Viewer
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include "ParticleViewer.hpp"
#include <Vox/FileSystem.hpp>
#include <Vox/FrameContext.hpp>
#include <Vox/VoxScene.hpp>
#include <Vox/Renderer.hpp>
#include <Vox/Program.hpp>
#include <Vox/FrameBuffer.hpp>
#include <Vox/Texture.hpp>
#include <Vox/ParticleBuffer.hpp>
#include <Vox/GeometryCacheManager.hpp>
#include <Vox/ShaderPreset.hpp>
#include <Vox/PostProcessing.hpp>
#include <Vox/SequentialFrameCapture.hpp>
#include <Vox/RoundRobinAsyncBuffer.hpp>
#include <Vox/DebugDraw.hpp>
#include <Vox/S3TextureCompression.hpp>
#include <glad/glad.h>

ParticleViewer::ParticleViewer()
{
    //! Do nothing.
}

ParticleViewer::~ParticleViewer()
{
    //! Do nothing.
}

bool ParticleViewer::Initialize(const Vox::Path& scenePath)
{
    if (!App::Initialize(scenePath))
        return false;
    
    std::shared_ptr<Vox::FrameContext> ctx = Vox::App::PopFrameContextFromQueue();
    ctx->BindSceneToContext(_scene);

    _cacheMgr = _scene->GetSceneObject<Vox::GeometryCacheManager>("ParticleCache");
    const auto& boundingBox = _cacheMgr->GetBoundingBox();
    
    _debugDraw.reset(new Vox::DebugDraw());
    _debugDraw->Initialize(ctx);
    _debugDraw->AddBoundingBox(boundingBox, {0.0f, 0.0f, 0.0f});
    _debugDraw->AddFloorGrid({10, 10}, 0.1f, { 0.144f, 0.188f, 0.188f });
    _debugDraw->AddCartesianCoordinateAxis(3.0f, 3.0f, 3.0f);

    _buffer.reset(new Vox::ParticleBuffer());
    GLuint vs = Vox::Renderer::CreateShaderFromSource(Vox::kPointsShaders[0], GL_VERTEX_SHADER);
    GLuint fs = Vox::Renderer::CreateShaderFromSource(Vox::kPointsShaders[1], GL_FRAGMENT_SHADER);
    _particleShader = ctx->CreateProgram("P_FluidParticle", Vox::Renderer::CreateProgram(vs, 0, fs));
    glDeleteShader(vs);
    glDeleteShader(fs);

    GLuint multisampleTexture = Vox::Renderer::CreateTexture(_windowSize.x, _windowSize.y, Vox::PixelFmt::PF_RGBA8, nullptr, true);
    _mainPassTexture = ctx->CreateTexture("T_MainPassColor", GL_TEXTURE_2D, multisampleTexture);
    GLuint multisampleRBO = Vox::Renderer::CreateRenderBuffer(_windowSize.x, _windowSize.y, Vox::PixelFmt::PF_DEPTH_COMPONENT24_STENCIL8, true);
    
    _mainPass = ctx->CreateFrameBuffer("FB_MainPass", Vox::Renderer::CreateFrameBuffer());
    _mainPass->AttachTexture(0, _mainPassTexture, true);
    _mainPass->AttachRenderBuffer(multisampleRBO);
    VoxAssert(_mainPass->ValidateFrameBufferStatus(), CURRENT_SRC_PATH_TO_STR, "Frame Buffer Status incomplete");
    
    GLuint screenTexture = Vox::Renderer::CreateTexture(_windowSize.x, _windowSize.y, Vox::PixelFmt::PF_RGB8, nullptr, false);
    _screenTexture = ctx->CreateTexture("T_Screen", GL_TEXTURE_2D, screenTexture);

    _intermediatePass = ctx->CreateFrameBuffer("FB_IntermediatePass", Vox::Renderer::CreateFrameBuffer());
    _intermediatePass->AttachTexture(0, _screenTexture, false);
    VoxAssert(_intermediatePass->ValidateFrameBufferStatus(), CURRENT_SRC_PATH_TO_STR, "Frame Buffer Status incomplete");

    _postProcessing.reset(new Vox::PostProcessing());
    _postProcessing->Initialize(ctx);

    //! Enable spehrical point sprite rendering.
    glEnable(GL_PROGRAM_POINT_SIZE);

    _compressor.reset(new Vox::S3TextureCompression(_windowSize.x, _windowSize.y));
    _compressor->Initialize(ctx);

    auto status = ctx->GetRenderStatus();
    status.primitive = GL_TRIANGLES;
    ctx->SetRenderStatus(status);

    Vox::App::PushFrameContextToQueue(ctx);
    return true;
}

void ParticleViewer::DrawFrame() 
{
    std::shared_ptr<Vox::FrameContext> ctx = Vox::App::PopFrameContextFromQueue();
    ctx->MakeContextCurrent();

    //! Main Render Pass
    _mainPass->BindFrameBuffer(GL_FRAMEBUFFER);
    {
        Vox::App::BeginFrame(ctx);
        glViewport(0, 0, _windowSize.x, _windowSize.y);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glDisable(GL_CULL_FACE);

        if (!_buffer->CheckFence(50000))
            return;

        _particleShader->BindProgram(_scene);
        _buffer->AsyncBufferTransfer(_cacheMgr);
        _buffer->DrawFrame(ctx);
        _buffer->AdvanceFrame();
        
        _debugDraw->DrawFrame(ctx);
        
        Vox::App::EndFrame(ctx);
    }

    //! Post Processing Pass
    ctx->GetFrameBuffer("FB_MainPass")->BindFrameBuffer(GL_READ_FRAMEBUFFER);
    ctx->GetFrameBuffer("FB_IntermediatePass")->BindFrameBuffer(GL_DRAW_FRAMEBUFFER);
    glBlitFramebuffer(0, 0, _windowSize.x, _windowSize.y, 0, 0, _windowSize.x, _windowSize.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    //! DXT5 Compressing Pass
    _compressor->CompressionPass(ctx, _screenTexture);
    //! DXT5 Decoding Pass
    auto compressedTexture = _compressor->DecodingPass(ctx);

    //! Screen Pass
    ctx->GetFrameBuffer("FB_DefaultPass")->BindFrameBuffer(GL_FRAMEBUFFER);
    {
        Vox::App::BeginFrame(ctx);
        glViewport(0, 0, _windowSize.x, _windowSize.y);
        _postProcessing->DrawFrame(ctx, compressedTexture);
        _frameCapture->CaptureFrameBuffer(_windowSize.x, _windowSize.y, 1, Vox::PixelFmt::PF_BGRA8);
        _frameCapture->WriteCurrentCaptureToDDS("./ParticleViewer_output/result%06d.dds");
        Vox::App::EndFrame(ctx);
    }

    if (_frameCapture->GetCurrentFrameIndex() == _cacheMgr->GetNumberOfCache()) ctx->SetWindowContextShouldClose(true);

    Vox::App::PushFrameContextToQueue(ctx);
}