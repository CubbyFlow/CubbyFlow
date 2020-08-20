/*************************************************************************
> File Name: FluidMeshViewer.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Particle Viewer
> Created Time: 2020/08/11
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include "FluidMeshViewer.hpp"
#include <Vox/FileSystem.hpp>
#include <Vox/FrameContext.hpp>
#include <Vox/VoxScene.hpp>
#include <Vox/Renderer.hpp>
#include <Vox/Program.hpp>
#include <Vox/ShaderPreset.hpp>
#include <Vox/FluidMeshBuffer.hpp>
#include <Vox/PostProcessing.hpp>
#include <Vox/FrameBuffer.hpp>
#include <Vox/S3TextureCompression.hpp>
#include <Vox/GeometryCache.hpp>
#include <Vox/GeometryCacheManager.hpp>
#include <glad/glad.h>

FluidMeshViewer::FluidMeshViewer()
{
    //! Do nothing.
}

FluidMeshViewer::~FluidMeshViewer()
{
    //! Do nothing.
}

bool FluidMeshViewer::Initialize(const Vox::Path& scenePath)
{
    if (!App::Initialize(scenePath))
        return false;
    
    std::shared_ptr<Vox::FrameContext> ctx = Vox::App::PopFrameContextFromQueue();
    _cacheMgr = _scene->GetSceneObject<Vox::GeometryCacheManager>("FluidMeshCache");

    for (size_t i = 0; i < _cacheMgr->GetNumberOfCache(); ++i)
    {
        auto& cache = _cacheMgr->GetGeometryCache(i);
        cache->InterleaveData(Vox::VertexFormat::Position3Normal3);
    }

    _buffer.reset(new Vox::FluidMeshBuffer());
    GLuint vs = Vox::Renderer::CreateShaderFromSource(Vox::kFluidMeshShaders[0], GL_VERTEX_SHADER);
    GLuint fs = Vox::Renderer::CreateShaderFromSource(Vox::kFluidMeshShaders[1], GL_FRAGMENT_SHADER);
    ctx->AddShaderProgram("FluidMesh", Vox::Renderer::CreateProgram(vs, 0, fs));
    glDeleteShader(vs);
    glDeleteShader(fs);

    GLuint mainPassColorTexture = Vox::Renderer::CreateTexture(_windowSize.x, _windowSize.y, Vox::PixelFmt::PF_RGBA8, nullptr, false);
    ctx->AddTexture("MainPassColorTexture", mainPassColorTexture);
    GLuint mainPassRBO = Vox::Renderer::CreateRenderBuffer(_windowSize.x, _windowSize.y, Vox::PixelFmt::PF_DEPTH_COMPONENT24_STENCIL8, false);

    ctx->AddFrameBuffer("MainRenderPass");
    ctx->BindFrameBuffer("MainRenderPass", GL_FRAMEBUFFER);
    const auto& mainFBO = ctx->GetCurrentFrameBuffer().lock();
    mainFBO->SetColorAttachment(0, mainPassColorTexture, false);   
    mainFBO->SetRenderBufferAttachment(mainPassRBO);
    VoxAssert(mainFBO->AssertFramebufferStatus(), CURRENT_SRC_PATH_TO_STR, "Frame Buffer Status incomplete");

    _compressor.reset(new Vox::S3TextureCompression(_windowSize.x, _windowSize.y));
    _compressor->Initialize(ctx);

    _postProcessing.reset(new Vox::PostProcessing());
    _postProcessing->Initialize(ctx);

    ctx->SetRenderMode(GL_TRIANGLES);

    Vox::App::PushFrameContextToQueue(ctx);
    return true;
}

void FluidMeshViewer::DrawFrame() 
{
    static int count = 0;

    std::shared_ptr<Vox::FrameContext> ctx = Vox::App::PopFrameContextFromQueue();
    ctx->MakeContextCurrent();

    //! Main RenderPass
    ctx->BindFrameBuffer("MainRenderPass" ,GL_FRAMEBUFFER);
    {
        Vox::App::BeginFrame(ctx);
        glViewport(0, 0, _windowSize.x, _windowSize.y);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);

        if (_buffer->CheckFence(50000))
        {
            ctx->MakeProgramCurrent("FluidMesh");
            ctx->UpdateProgramCamera(_camera);
            _buffer->AsyncBufferTransfer(_cacheMgr);
            _buffer->DrawFrame(ctx);
            _buffer->AdvanceFrame();
        }
        
        Vox::App::EndFrame(ctx);
    }

    //! DXT5 Compressing Pass
    _compressor->DXT5Compress(ctx, "MainPassColorTexture");

    //! Screen Pass
    ctx->BindFrameBuffer("DefaultPass", GL_FRAMEBUFFER);
    {
        Vox::App::BeginFrame(ctx);
        glViewport(0, 0, _windowSize.x, _windowSize.y);
        _postProcessing->DrawFrame(ctx, "CompressedTexture");
        //! ReadPixels must be implemented with Asynchronous features.
        //! Note this reference : http://http.download.nvidia.com/developer/Papers/2005/Fast_Texture_Transfers/Fast_Texture_Transfers.pdf
        char baseName[256];
        snprintf(baseName, sizeof(baseName), "./FluidMeshViewer_output/result%06d.png", count++);
        Vox::Renderer::SaveTextureToRGB(baseName, _windowSize.x, _windowSize.y);
        Vox::App::EndFrame(ctx);
    }

    if (static_cast<unsigned int>(count) == _cacheMgr->GetNumberOfCache()) ctx->SetWindowContextShouldClose(true);

    Vox::App::PushFrameContextToQueue(ctx);
}

void FluidMeshViewer::OnSetWindowSize()
{
    App::OnSetWindowSize();
}