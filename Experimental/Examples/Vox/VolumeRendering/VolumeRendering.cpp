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
#include <Vox/FrameBuffer.hpp>
#include <Vox/Texture.hpp>
#include <Vox/ShaderPreset.hpp>
#include <Vox/GeometryCache.hpp>
#include <Vox/Mesh.hpp>
#include <Vox/RenderableObject.hpp>
#include <Vox/PostProcessing.hpp>
#include <Vox/SequentialFrameCapture.hpp>
#include <glad/glad.h>
#include <fstream>
#include <Core/Grid/VertexCenteredScalarGrid3.hpp>
#include <Vox/ArrayUtils.hpp>
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
    ctx->BindSceneToContext(_scene);

    GLuint mainPassColorTexture = Vox::Renderer::CreateTexture(_windowSize.x, _windowSize.y, Vox::PixelFmt::PF_RGBA8, nullptr, false);
    GLuint mainPassRBO = Vox::Renderer::CreateRenderBuffer(_windowSize.x, _windowSize.y, Vox::PixelFmt::PF_DEPTH_COMPONENT24_STENCIL8, false);
    ctx->CreateTexture("MainPassColorTexture", GL_TEXTURE_2D, mainPassColorTexture);

    _mainPass = ctx->CreateFrameBuffer("MainRenderPass", Vox::Renderer::CreateFrameBuffer());
    _mainPass->AttachTexture(0, mainPassColorTexture, false);
    _mainPass->AttachRenderBuffer(mainPassRBO);
    VoxAssert(_mainPass->ValidateFrameBufferStatus(), CURRENT_SRC_PATH_TO_STR, "Frame Buffer Status incomplete");

    _rayDataPass = ctx->CreateFrameBuffer("RayDataPass", Vox::Renderer::CreateFrameBuffer());

    GLuint frontface = Vox::Renderer::CreateTexture(_windowSize.x, _windowSize.y, Vox::PixelFmt::PF_RGBA16F, nullptr);
    GLuint backface  = Vox::Renderer::CreateTexture(_windowSize.x, _windowSize.y, Vox::PixelFmt::PF_RGBA16F, nullptr);
    _volumeFrontFace = ctx->CreateTexture("VolumeFrontFace", GL_TEXTURE_2D, frontface);
    _volumeBackFace  = ctx->CreateTexture("VolumeBackFace", GL_TEXTURE_2D, backface);

    GLuint vs = Vox::Renderer::CreateShaderFromSource(Vox::kRayDataShaders[0], GL_VERTEX_SHADER);
    GLuint fs = Vox::Renderer::CreateShaderFromSource(Vox::kRayDataShaders[1], GL_FRAGMENT_SHADER);
    _rayDataShader = ctx->CreateProgram("RayDataShader", Vox::Renderer::CreateProgram(vs, 0, fs));

    vs = Vox::Renderer::CreateShaderFromSource(Vox::kVolumeRayCastingShaders[0], GL_VERTEX_SHADER);
    fs = Vox::Renderer::CreateShaderFromSource(Vox::kVolumeRayCastingShaders[1], GL_FRAGMENT_SHADER);
    _rayCastingShader = ctx->CreateProgram("VolumeRayCastingShader", Vox::Renderer::CreateProgram(vs, 0, fs));

    auto& volumeParams = _rayCastingShader->GetParameters();
    volumeParams.SetParameter("VolumeTexture", 0);
    volumeParams.SetParameter("VolumeFrontFace", 1);
    volumeParams.SetParameter("VolumeBackFace", 2);
    volumeParams.SetParameter("StepInc", 0.7f);

    _postProcessing.reset(new Vox::PostProcessing());
    _postProcessing->Initialize(ctx);

    const auto& cubeCache = _scene->GetSceneObject<Vox::GeometryCache>("VolumeCube");
    cubeCache->InterleaveData(Vox::VertexFormat::Position3);
    auto cubeMesh = std::make_shared<Vox::Mesh>(cubeCache->GetShape(0), Vox::VertexFormat::Position3, true);
    _cubeRenderable = std::make_shared<Vox::RenderableObject>();
    _cubeRenderable->AddGeometryMesh(cubeMesh);

    std::ifstream file("./out.sdf");
    const std::vector<uint8_t> buffer(
        (std::istreambuf_iterator<char>(file)),
        (std::istreambuf_iterator<char>()));
    CubbyFlow::VertexCenteredScalarGrid3 grid;
    grid.Deserialize(buffer);

    CubbyFlow::Array3<float> sdf;
    Vox::ArrayCast(sdf, grid.GetDataAccessor());

    file.close();

    GLuint volume = Vox::Renderer::CreateVolumeTexture(sdf.size().x, sdf.size().y, sdf.size().z, Vox::PixelFmt::PF_R8, sdf.data());
    _volumeSDF = ctx->CreateTexture("VolumeTexture", GL_TEXTURE_3D, volume);

    auto status = ctx->GetRenderStatus();
    status.primitive = GL_TRIANGLES;
    ctx->SetRenderStatus(status);
    Vox::App::PushFrameContextToQueue(ctx);

    return true;
}

void VolumeRendering::DrawFrame() 
{
    std::shared_ptr<Vox::FrameContext> ctx = Vox::App::PopFrameContextFromQueue();
    ctx->MakeContextCurrent();

    //! Volume Entry Point calculation pass.
    _rayDataPass->BindFrameBuffer(GL_FRAMEBUFFER);
    {
        _rayDataShader->BindProgram(_scene);
        _rayDataPass->AttachTexture(0, _backface, false);
        Vox::App::BeginFrame(ctx);
        glFrontFace(GL_CCW);
        _cubeRenderable->DrawRenderableObject(ctx);
        Vox::App::EndFrame(ctx);
        _frameCapture->CaptureFrameBuffer(_windowSize.x, _windowSize.y, 1, Vox::PixelFmt::PF_BGRA8);
        _frameCapture->WriteCurrentCaptureToDDS("./VolumeRendering_output/result%06d.dds");
        _rayDataPass->AttachTexture(0, _frontface, false);
        Vox::App::BeginFrame(ctx);
        glFrontFace(GL_CW);
        _cubeRenderable->DrawRenderableObject(ctx);
        Vox::App::EndFrame(ctx);
        _frameCapture->CaptureFrameBuffer(_windowSize.x, _windowSize.y, 1, Vox::PixelFmt::PF_BGRA8);
        _frameCapture->WriteCurrentCaptureToDDS("./VolumeRendering_output/result%06d.dds");
        glFrontFace(GL_CCW);
    }

    //! Main Render Pass
    _mainPass->BindFrameBuffer(GL_FRAMEBUFFER);
    {
        Vox::App::BeginFrame(ctx);
        glViewport(0, 0, _windowSize.x, _windowSize.y);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        _rayCastingShader->BindProgram(_scene);
        _volumeSDF->BindTexture(0);
        _volumeFrontFace->BindTexture(1);
        _volumeBackFace->BindTexture(2);

        Vox::App::EndFrame(ctx);
    }

    //! Screen Pass
    ctx->GetFrameBuffer("DefaultPass")->BindFrameBuffer(GL_FRAMEBUFFER);
    {
        Vox::App::BeginFrame(ctx);
        glViewport(0, 0, _windowSize.x, _windowSize.y);
        _postProcessing->DrawFrame(ctx, _volumeBackFace);
        _frameCapture->CaptureFrameBuffer(_windowSize.x, _windowSize.y, 1, Vox::PixelFmt::PF_BGRA8);
        _frameCapture->WriteCurrentCaptureToDDS("./VolumeRendering_output/result%06d.dds");
        Vox::App::EndFrame(ctx);
    }

    if (_frameCapture->GetCurrentFrameIndex() >= 2) ctx->SetWindowContextShouldClose(true);

    Vox::App::PushFrameContextToQueue(ctx);
}