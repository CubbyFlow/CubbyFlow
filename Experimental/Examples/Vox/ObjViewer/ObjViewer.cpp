/*************************************************************************
> File Name: ObjViewer.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Particle Viewer
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include "ObjViewer.hpp"
#include <Vox/FileSystem.hpp>
#include <Vox/FrameContext.hpp>
#include <Vox/VoxScene.hpp>
#include <Vox/Renderer.hpp>
#include <Vox/Program.hpp>
#include <Vox/FrameBuffer.hpp>
#include <Vox/Texture.hpp>
#include <Vox/ShaderPreset.hpp>
#include <Vox/GeometryCache.hpp>
#include <Vox/PostProcessing.hpp>
#include <Vox/Mesh.hpp>
#include <Vox/RenderableObject.hpp>
#include <Vox/SequentialFrameCapture.hpp>
#include <glad/glad.h>

ObjViewer::ObjViewer()
{
    //! Do nothing.
}

ObjViewer::~ObjViewer()
{
    //! Do nothing.
}

bool ObjViewer::Initialize(const Vox::Path& scenePath)
{
    if (!App::Initialize(scenePath))
        return false;
    
    std::shared_ptr<Vox::FrameContext> ctx = Vox::App::PopFrameContextFromQueue();
    ctx->BindSceneToContext(_scene);
    
    GLuint mainPassColorTexture = Vox::Renderer::CreateTexture(_windowSize.x, _windowSize.y, Vox::PixelFmt::PF_RGBA8, nullptr, false);
    _screenTexture = ctx->CreateTexture("T_MainPassColor", GL_TEXTURE_2D, mainPassColorTexture);
    GLuint mainPassRBO = Vox::Renderer::CreateRenderBuffer(_windowSize.x, _windowSize.y, Vox::PixelFmt::PF_DEPTH_COMPONENT24_STENCIL8, false);

    _mainPass = ctx->CreateFrameBuffer("FB_MainPass", Vox::Renderer::CreateFrameBuffer());
    _mainPass->AttachTexture(0, mainPassColorTexture, false);
    _mainPass->AttachRenderBuffer(mainPassRBO);
    VoxAssert(_mainPass->ValidateFrameBufferStatus(), CURRENT_SRC_PATH_TO_STR, "Frame Buffer Status incomplete");

    GLuint vs = Vox::Renderer::CreateShaderFromSource(Vox::kFluidMeshShaders[0], GL_VERTEX_SHADER);
    GLuint fs = Vox::Renderer::CreateShaderFromSource(Vox::kFluidMeshShaders[1], GL_FRAGMENT_SHADER);
    _meshShader = ctx->CreateProgram("P_MeshShader", Vox::Renderer::CreateProgram(vs, 0, fs));

    _postProcessing.reset(new Vox::PostProcessing());
    _postProcessing->Initialize(ctx);

    const auto& sphereCache = _scene->GetSceneObject<Vox::GeometryCache>("Sphere");
    sphereCache->InterleaveData(Vox::VertexFormat::Position3Normal3);
    auto sphereMesh = std::make_shared<Vox::Mesh>(sphereCache->GetShape(0), Vox::VertexFormat::Position3Normal3, true);
    _renderable = std::make_shared<Vox::RenderableObject>();
    _renderable->AddGeometryMesh(sphereMesh);

    auto status = ctx->GetRenderStatus();
    status.primitive = GL_TRIANGLES;
    ctx->SetRenderStatus(status);
    Vox::App::PushFrameContextToQueue(ctx);
    return true;
}

void ObjViewer::DrawFrame() 
{
    std::shared_ptr<Vox::FrameContext> ctx = Vox::App::PopFrameContextFromQueue();
    ctx->MakeContextCurrent();

    //! Main Render Pass
    _mainPass->BindFrameBuffer(GL_FRAMEBUFFER);
    {
        Vox::App::BeginFrame(ctx);
        glViewport(0, 0, _windowSize.x, _windowSize.y);
        
        _meshShader->BindProgram(_scene);
        _renderable->DrawRenderableObject(ctx);

        Vox::App::EndFrame(ctx);
    }

    //! Screen Pass
    ctx->GetFrameBuffer("FB_DefaultPass")->BindFrameBuffer(GL_FRAMEBUFFER);
    {
        Vox::App::BeginFrame(ctx);
        glViewport(0, 0, _windowSize.x, _windowSize.y);
        _postProcessing->DrawFrame(ctx, _screenTexture);
        _frameCapture->CaptureFrameBuffer(_windowSize.x, _windowSize.y, 1, Vox::PixelFmt::PF_BGRA8);
        _frameCapture->WriteCurrentCaptureToTGA("./ObjViewer_output/result%06d.tga");
        Vox::App::EndFrame(ctx);
    }

    if (_frameCapture->GetCurrentFrameIndex() == 2) ctx->SetWindowContextShouldClose(true);

    Vox::App::PushFrameContextToQueue(ctx);
}