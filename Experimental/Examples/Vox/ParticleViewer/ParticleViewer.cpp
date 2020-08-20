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
#include <Vox/ParticleBuffer.hpp>
#include <Vox/GeometryCacheManager.hpp>
#include <Vox/ShaderPreset.hpp>
#include <Vox/FrameBuffer.hpp>
#include <Vox/PostProcessing.hpp>
#include <Vox/RoundRobinAsyncBuffer.hpp>
#include <Vox/DebugDraw.hpp>
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

    _debugDraw.reset(new Vox::DebugDraw());

    _cacheMgr = _scene->GetSceneObject<Vox::GeometryCacheManager>("ParticleCache");
    const auto& boundingBox = _cacheMgr->GetBoundingBox();
    _debugDraw->AddBoundingBox(boundingBox, {0.0f, 0.0f, 0.0f});
    _debugDraw->AddFloorGrid({10, 10}, 0.1f, { 0.144f, 0.188f, 0.188f });
    _debugDraw->AddCartesianCoordinateAxis(3.0f, 3.0f, 3.0f);
    _debugDraw->Initialize(ctx);

    _buffer.reset(new Vox::ParticleBuffer());
    GLuint vs = Vox::Renderer::CreateShaderFromSource(Vox::kPointsShaders[0], GL_VERTEX_SHADER);
    GLuint fs = Vox::Renderer::CreateShaderFromSource(Vox::kPointsShaders[1], GL_FRAGMENT_SHADER);
    ctx->AddShaderProgram("FluidParticle", Vox::Renderer::CreateProgram(vs, 0, fs));
    glDeleteShader(vs);
    glDeleteShader(fs);

    GLuint multisampleTexture = Vox::Renderer::CreateTexture(_windowSize.x, _windowSize.y, Vox::PixelFmt::PF_RGBA8, nullptr, true);
    ctx->AddTexture("MainPassColorTexture", multisampleTexture);
    GLuint multisampleRBO = Vox::Renderer::CreateRenderBuffer(_windowSize.x, _windowSize.y, Vox::PixelFmt::PF_DEPTH_COMPONENT24_STENCIL8, true);
    
    ctx->AddFrameBuffer("MainRenderPass");
    ctx->BindFrameBuffer("MainRenderPass", GL_FRAMEBUFFER);
    const auto& multisampleFBO = ctx->GetCurrentFrameBuffer().lock();
    multisampleFBO->SetColorAttachment(0, multisampleTexture, true);   
    multisampleFBO->SetRenderBufferAttachment(multisampleRBO);
    VoxAssert(multisampleFBO->AssertFramebufferStatus(), CURRENT_SRC_PATH_TO_STR, "Frame Buffer Status incomplete");
    
    GLuint screenTexture = Vox::Renderer::CreateTexture(_windowSize.x, _windowSize.y, Vox::PixelFmt::PF_RGB8, nullptr, false);
    ctx->AddTexture("ScreenTexture", screenTexture);

    ctx->AddFrameBuffer("IntermediatePass");
    ctx->BindFrameBuffer("IntermediatePass", GL_FRAMEBUFFER);
    const auto& intermediateFBO = ctx->GetCurrentFrameBuffer().lock();
    intermediateFBO->SetColorAttachment(0, screenTexture, false);   
    VoxAssert(intermediateFBO->AssertFramebufferStatus(), CURRENT_SRC_PATH_TO_STR, "Frame Buffer Status incomplete");

    _postProcessing.reset(new Vox::PostProcessing());
    _postProcessing->Initialize(ctx);

    //! Enable spehrical point sprite rendering.
    glEnable(GL_PROGRAM_POINT_SIZE);

    Vox::App::PushFrameContextToQueue(ctx);
    return true;
}

void ParticleViewer::DrawFrame() 
{
    static int count = 0;
    
    std::shared_ptr<Vox::FrameContext> ctx = Vox::App::PopFrameContextFromQueue();
    ctx->MakeContextCurrent();

    //_camera->OrbitRotation(CubbyFlow::Vector3F(0.5f, 0.0f, 0.0f), 0.02f, 0.0f, 3.0f);

    //! Main Render Pass
    ctx->BindFrameBuffer("MainRenderPass", GL_FRAMEBUFFER);
    {
        Vox::App::BeginFrame(ctx);
        glViewport(0, 0, _windowSize.x, _windowSize.y);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glDisable(GL_CULL_FACE);

        if (!_buffer->CheckFence(50000))
            return;

        ctx->MakeProgramCurrent("FluidParticle");
        ctx->UpdateProgramCamera(_camera);
        _buffer->AsyncBufferTransfer(_cacheMgr);
        _buffer->DrawFrame(ctx);
        _buffer->AdvanceFrame();
        
        ctx->MakeProgramCurrent("DebugDraw");
        ctx->UpdateProgramCamera(_camera);
        _debugDraw->DrawFrame(ctx);
        
        Vox::App::EndFrame(ctx);
    }

    //! Post Processing Pass
    ctx->BindFrameBuffer("MainRenderPass", GL_READ_FRAMEBUFFER);
    ctx->BindFrameBuffer("IntermediatePass", GL_DRAW_FRAMEBUFFER);
    glBlitFramebuffer(0, 0, _windowSize.x, _windowSize.y, 0, 0, _windowSize.x, _windowSize.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    //! Screen Pass
    ctx->BindFrameBuffer("DefaultPass", GL_FRAMEBUFFER);
    {
        Vox::App::BeginFrame(ctx);
        glViewport(0, 0, _windowSize.x, _windowSize.y);
        _postProcessing->DrawFrame(ctx, "ScreenTexture");
        //! ReadPixels must be implemented with Asynchronous features.
        //! Note this reference : http://http.download.nvidia.com/developer/Papers/2005/Fast_Texture_Transfers/Fast_Texture_Transfers.pdf
        char baseName[256];
        snprintf(baseName, sizeof(baseName), "./ParticleViewer_output/result%06d.png", count++);
        Vox::Renderer::SaveTextureToRGB(baseName, _windowSize.x, _windowSize.y);
        Vox::App::EndFrame(ctx);
    }

    if (static_cast<unsigned int>(count) == _cacheMgr->GetNumberOfCache()) ctx->SetWindowContextShouldClose(true);

    Vox::App::PushFrameContextToQueue(ctx);
}