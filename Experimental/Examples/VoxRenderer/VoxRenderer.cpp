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
#include <Vox/Camera/PerspectiveCamera.hpp>
#include <Vox/Core/Material.hpp>
#include <Vox/Core/Program.hpp>
#include <Vox/Core/FrameBuffer.hpp>
#include <Vox/Core/Texture.hpp>
#include <Vox/Core/ShaderPreset.hpp>
#include <Vox/Core/FluidBuffer.hpp>
#include <Vox/Core/PointLight.hpp>
#include <Vox/Core/PostProcessing.hpp>
#include <Vox/Utils/SequentialFrameCapture.hpp>
#include <Vox/Scene/GeometryCache.hpp>
#include <Vox/Scene/GeometryCacheManager.hpp>
#include <glad/glad.h>

VoxRenderer::VoxRenderer()
{
    //! Do nothing.
}

VoxRenderer::VoxRenderer(const int width, const int height)
    : App(width, height)
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

    //! Initialize fluid animation buffer which implemented with round-robin async technique.
    _buffer = _scene->GetSceneObject<Vox::FluidBuffer>("fluidAnim");

    //! Add uniform variables to mesh shader.
    const auto& waterMaterial = _scene->GetSceneObject<Vox::Material>("watermat");
    _meshShader = waterMaterial->GetProgram();
    _light = std::make_shared<Vox::PointLight>();
    _light->SetPosition(CubbyFlow::Vector3F(3.0f, 3.0f, 10.0f));
    _light->SetColor(CubbyFlow::Vector3F(150.0f, 150.0f, 150.0f));
    _light->UploadToProgram(_meshShader);

    //! Create main render target object with HDR color texture.
    GLuint mainPassColorTexture = Vox::Renderer::CreateTexture(_windowSize.x, _windowSize.y, Vox::PixelFmt::PF_RGBA16F, nullptr, false);
    _screenTexture = std::make_shared<Vox::Texture>(GL_TEXTURE_2D, mainPassColorTexture);
    GLuint mainPassRBO = Vox::Renderer::CreateRenderBuffer(_windowSize.x, _windowSize.y, Vox::PixelFmt::PF_DEPTH_COMPONENT24_STENCIL8, false);

    _mainPass = std::make_shared<Vox::FrameBuffer>(Vox::Renderer::CreateFrameBuffer());
    _mainPass->AttachTexture(0, _screenTexture, false);
    _mainPass->AttachRenderBuffer(mainPassRBO);
    VoxAssert(_mainPass->ValidateFrameBufferStatus(), CURRENT_SRC_PATH_TO_STR, "Frame Buffer Status incomplete");

    //! Create post-processing shader which do gamma-correnction and tone mapping for hdr.
    GLuint vs, fs;
    vs = Vox::Renderer::CreateShaderFromFile(Vox::FileSystem::FindPath("quad.glsl"), GL_VERTEX_SHADER);
    fs = Vox::Renderer::CreateShaderFromFile(Vox::FileSystem::FindPath("output.glsl"), GL_FRAGMENT_SHADER);
    _postProcessing.reset(new Vox::PostProcessing());
    _postProcessing->Initialize(ctx, Vox::Renderer::CreateProgram(vs, 0, fs));
    glDeleteShader(vs);
    glDeleteShader(fs);

    Vox::App::PushFrameContextToQueue(ctx);
    return true;
}

void VoxRenderer::UpdateFrame(double dt)
{
    //! Upload view projection and cam position to mesh shader.
    _camera->UploadToProgram(_meshShader);

    //! Advance fluid animation buffer frame.
    UNUSED_VARIABLE(dt);
}

void VoxRenderer::DrawFrame() 
{
    std::shared_ptr<Vox::FrameContext> ctx = Vox::App::PopFrameContextFromQueue();
    ctx->MakeContextCurrent();

    //! Main RenderPass
    _mainPass->BindFrameBuffer(GL_FRAMEBUFFER);
    {
        //! Clear the screen.
        Vox::App::BeginFrame(ctx);
        glViewport(0, 0, _windowSize.x, _windowSize.y);

        //! Render fluid animation buffer with brdf mesh shader.
        if (_buffer->CheckFence(50000))
        {
            _buffer->AsyncBufferTransfer();
            _buffer->DrawFrame(ctx);
            _buffer->AdvanceFrame();
        }
    }

    //! Screen Pass
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    {
        //! Clear the screen.
        Vox::App::BeginFrame(ctx);
        glViewport(0, 0, _windowSize.x, _windowSize.y);

        //! Post-processing the screen.
        _postProcessing->DrawFrame(ctx, _screenTexture);
        
        //! Record current screen and save to image.
        //! _frameCapture->CaptureFrameBuffer(_windowSize.x, _windowSize.y, 1, Vox::PixelFmt::PF_RGBA8);
        //! _frameCapture->WriteCurrentCapture("./VoxRenderer_output/result%06d.jpg");

        //! Draw GUI stuffs.
        Vox::App::EndFrame(ctx);
    }

    Vox::App::PushFrameContextToQueue(ctx);
}