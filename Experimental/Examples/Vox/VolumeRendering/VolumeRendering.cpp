/*************************************************************************
> File Name: VolumeRendering.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Particle Viewer
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include "VolumeRendering.hpp"
#include <Vox/FileSystem.hpp>
#include <Vox/FrameContext.hpp>
#include <Vox/VoxScene.hpp>
#include <Vox/Renderer.hpp>
#include <Vox/Program.hpp>
#include <Vox/ShaderPreset.hpp>
#include <Vox/GeometryCache.hpp>
#include <Vox/Mesh.hpp>
#include <Vox/RenderableObject.hpp>
#include <Vox/PostProcessing.hpp>
#include <Vox/SequentialFrameCapture.hpp>
#include <glad/glad.h>

VolumeRendering::VolumeRendering()
{
    //! Do nothing.
}

VolumeRendering::~VolumeRendering()
{
    //! Do nothing.
}

bool VolumeRendering::Initialize(const Vox::Path& scenePath)
{
    if (!App::Initialize(scenePath))
        return false;
    
    std::shared_ptr<Vox::FrameContext> ctx = Vox::App::PopFrameContextFromQueue();

    GLuint mainPassColorTexture = Vox::Renderer::CreateTexture(_windowSize.x, _windowSize.y, Vox::PixelFmt::PF_RGBA8, nullptr, false);
    ctx->AddTexture("MainPassColorTexture", mainPassColorTexture);
    GLuint mainPassRBO = Vox::Renderer::CreateRenderBuffer(_windowSize.x, _windowSize.y, Vox::PixelFmt::PF_DEPTH_COMPONENT24_STENCIL8, false);

    GLuint mainPass = Vox::Renderer::CreateFrameBuffer();
    ctx->AddFrameBuffer("MainRenderPass", mainPass);
    Vox::Renderer::AttachTextureToFrameBuffer(mainPass, 0, mainPassColorTexture, false);
    Vox::Renderer::AttachRenderBufferToFrameBuffer(mainPass, mainPassRBO);
    VoxAssert(Vox::Renderer::ValidateFrameBufferStatus(mainPass), CURRENT_SRC_PATH_TO_STR, "Frame Buffer Status incomplete");

    GLuint volumeRayEntryPointPass = Vox::Renderer::CreateFrameBuffer();
    ctx->AddFrameBuffer("VolumeRayEntryPointPass", volumeRayEntryPointPass);

    _frontface = Vox::Renderer::CreateTexture(_windowSize.x, _windowSize.y, Vox::PixelFmt::PF_RGBA16F, nullptr);
    ctx->AddTexture("VolumeFrontFace", _frontface);
    _backface = Vox::Renderer::CreateTexture(_windowSize.x, _windowSize.y, Vox::PixelFmt::PF_RGBA16F, nullptr);
    ctx->AddTexture("VolumeBackFace", _backface);

    GLuint vs = Vox::Renderer::CreateShaderFromSource(Vox::kRayEntryPointShaders[0], GL_VERTEX_SHADER);
    GLuint fs = Vox::Renderer::CreateShaderFromSource(Vox::kRayEntryPointShaders[1], GL_FRAGMENT_SHADER);
    ctx->AddShaderProgram("RayEntryPointShader", Vox::Renderer::CreateProgram(vs, 0, fs));

    _postProcessing.reset(new Vox::PostProcessing());
    _postProcessing->Initialize(ctx);

    const auto& cubeCache = _scene->GetSceneObject<Vox::GeometryCache>("VolumeCube");
    cubeCache->InterleaveData(Vox::VertexFormat::Position3);
    auto cubeMesh = std::make_shared<Vox::Mesh>(cubeCache->GetShape(0), Vox::VertexFormat::Position3, true);
    _cubeRenderable = std::make_shared<Vox::RenderableObject>();
    _cubeRenderable->AddGeometryMesh(cubeMesh);

    Vox::App::PushFrameContextToQueue(ctx);
    return true;
}

void VolumeRendering::DrawFrame() 
{
    std::shared_ptr<Vox::FrameContext> ctx = Vox::App::PopFrameContextFromQueue();
    ctx->MakeContextCurrent();

    //! Volume Entry Point calculation pass.
    ctx->BindFrameBuffer("VolumeRayEntryPointPass", GL_FRAMEBUFFER);
    {
        ctx->MakeProgramCurrent("RayEntryPointShader");
        ctx->UpdateProgramCamera(_camera);
        glEnable(GL_CULL_FACE);
        GLuint fbo = ctx->GetCurrentFrameBuffer();
        Vox::Renderer::AttachTextureToFrameBuffer(fbo, 0, _backface, false);
        Vox::App::BeginFrame(ctx);
        glCullFace(GL_FRONT);
        _cubeRenderable->DrawRenderableObject(ctx);
        Vox::App::EndFrame(ctx);
        Vox::Renderer::AttachTextureToFrameBuffer(fbo, 0, _frontface, false);
        Vox::App::BeginFrame(ctx);
        glCullFace(GL_BACK);
        _cubeRenderable->DrawRenderableObject(ctx);
        Vox::App::EndFrame(ctx);
    }

    //! Main Render Pass
    ctx->BindFrameBuffer("MainRenderPass", GL_FRAMEBUFFER);
    {
        Vox::App::BeginFrame(ctx);
        glViewport(0, 0, _windowSize.x, _windowSize.y);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        ctx->MakeProgramCurrent("VolumeRayCastingShader");
        ctx->UpdateProgramCamera(_camera);
        ctx->BindTextureToSlot("VolumeFrontFace", GL_TEXTURE_2D, 2);
        ctx->BindTextureToSlot("VolumeBackFace", GL_TEXTURE_2D, 3);

        Vox::App::EndFrame(ctx);
    }

    //! Screen Pass
    ctx->BindFrameBuffer("DefaultPass", GL_FRAMEBUFFER);
    {
        Vox::App::BeginFrame(ctx);
        glViewport(0, 0, _windowSize.x, _windowSize.y);
        _postProcessing->DrawFrame(ctx, "ScreenTexture");
        _frameCapture->CaptureFrameBuffer(_windowSize.x, _windowSize.y, 1, Vox::PixelFmt::PF_BGRA8);
        _frameCapture->WriteCurrentCaptureToTGA("./VolumeRendering_output/result%06d.tga");
        Vox::App::EndFrame(ctx);
    }

    if (_frameCapture->GetCurrentFrameIndex() == 2) ctx->SetWindowContextShouldClose(true);

    Vox::App::PushFrameContextToQueue(ctx);
}