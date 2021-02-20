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
#include <Vox/Mesh/CubeMesh.hpp>
#include <Vox/Core/Material.hpp>
#include <Vox/Core/Program.hpp>
#include <Vox/Core/FrameBuffer.hpp>
#include <Vox/Core/TextureBaker.hpp>
#include <Vox/Core/Texture.hpp>
#include <Vox/Core/StaticRenderable.hpp>
#include <Vox/Core/FluidRenderable.hpp>
#include <Vox/Core/PointLight.hpp>
#include <Vox/Core/PostProcessing.hpp>
#include <Vox/Utils/SequentialFrameCapture.hpp>
#include <Vox/Scene/GeometryCache.hpp>
#include <Vox/Scene/GeometryCacheManager.hpp>
#include <glad/glad.h>
#include <glfw/glfw3.h>

VoxRenderer::VoxRenderer()
    : App(), _polygonMode(GL_FILL)
{
    //! Do nothing.
}

VoxRenderer::VoxRenderer(const int width, const int height)
    : App(width, height), _polygonMode(GL_FILL)
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

    //! Create main framebuffer
    _mainPass = std::make_shared<Vox::FrameBuffer>(Vox::Renderer::CreateFrameBuffer());
    _mainPass->BindFrameBuffer(GL_FRAMEBUFFER);

    //! Create main render target object with HDR color texture.
    GLuint mainPassColorTexture = Vox::Renderer::CreateTexture(_windowSize.x, _windowSize.y, Vox::PixelFmt::PF_RGBA16F, nullptr, false);
    _screenTexture = std::make_shared<Vox::Texture>(GL_TEXTURE_2D, mainPassColorTexture);
    GLuint mainPassRBO = Vox::Renderer::CreateRenderBuffer(_windowSize.x, _windowSize.y, Vox::PixelFmt::PF_DEPTH_COMPONENT24_STENCIL8, false);

    //! Attach color attachment to the main framebuffer
    _mainPass->AttachTexture(0, GL_TEXTURE_2D, _screenTexture);
    //! Attach render target object to the main framebuffer
    _mainPass->AttachRenderBuffer(mainPassRBO);
    //! Check the framebuffer status
    VoxAssert(_mainPass->ValidateFrameBufferStatus(), CURRENT_SRC_PATH_TO_STR, "Frame Buffer Status incomplete");

    //! Get CubeMap equirectangular texture from the scene.
    auto cubeMapEquirect = _scene->GetSceneObject<Vox::Texture>("cubeMap");
    //! Baking cubemap from the hdr equirectangular texture     
    auto bakedCubeMap = Vox::TextureBaker::BakingCubeMap(ctx, cubeMapEquirect, 512, 512);
    //! deallocate cubemap texture because no need to store cube map equirectangular texture as already we have baked texture cube map.
    _scene->DeallocateObject("cubeMap");

    //! Get IrradianceMap equirectangular texture from the scene.
    auto irradianceMapEquirect = _scene->GetSceneObject<Vox::Texture>("irradianceMap");
    //! Baking irradianceMap from the hdr equirectangular texture     
    auto bakedIrradianceMap = Vox::TextureBaker::BakingCubeMap(ctx, irradianceMapEquirect, 512, 512);
    //! deallocate irradiance map texture because no need to store irradiance map equirectangular texture as already we have baked texture irradiance map.
    _scene->DeallocateObject("irradianceMap");

    //! Get point light instance which manage whole point light datum
    _pointLight = _scene->GetSceneObject<Vox::PointLight>("point_lights");

    //! Get loaded fluid animation buffers which implemented with round-robin async technique.
    _fluidAnims = _scene->GetSceneObjects<Vox::FluidRenderable>();
    for (auto& anim : _fluidAnims)
    {
        //! upload uniform variables to fluid shader
        //! And add irradiance map texture to fluid anim material.
        auto animMaterial = anim->GetMaterial();
        auto& animShader = animMaterial->GetProgram();
        if (animShader->HasUniformVariable("cubeMap"))
        {
            animMaterial->AttachTextureToSlot(bakedCubeMap, 0);
            animShader->GetParameters().SetParameter("cubeMap", 0);
        }
        else
        {
            _pointLight->UploadToProgram(animShader);
        }
    }

    //! Get all loaded static objects.
    _obstacles = _scene->GetSceneObjects<Vox::StaticRenderable>();
    for (auto& obstacle : _obstacles)
    {
        auto& material = obstacle->GetMaterial();
        auto& program = material->GetProgram();
        program->GetParameters().SetParameter("irradianceMap", 0);
        material->AttachTextureToSlot(bakedIrradianceMap, 0);
        _pointLight->UploadToProgram(program);
    }

    //! Create Material for cube map rendering.
    auto cubeMapMaterial = std::make_shared<Vox::Material>();
    GLuint vs, fs;
    vs = Vox::Renderer::CreateShaderFromFile(Vox::FileSystem::FindPath("cubemap_vs.glsl"), GL_VERTEX_SHADER);
    fs = Vox::Renderer::CreateShaderFromFile(Vox::FileSystem::FindPath("cubemap_fs.glsl"), GL_FRAGMENT_SHADER);
    cubeMapMaterial->AttachProgramShader(std::make_shared<Vox::Program>(Vox::Renderer::CreateProgram(vs, 0, fs)));
    glDeleteShader(vs);
    glDeleteShader(fs);
    cubeMapMaterial->AttachTextureToSlot(bakedCubeMap, 0);
    cubeMapMaterial->GetProgram()->GetParameters().SetParameter("cubeMap", 0);

    //! Set render status for the skybox material.
    auto defaultRenderStatus = cubeMapMaterial->GetRenderStatus();
    defaultRenderStatus.depthFunc = GL_LEQUAL;
    cubeMapMaterial->SetRenderStatus(defaultRenderStatus);

    //! Create cube mesh and attach skybox material.
    _skybox = std::make_shared<Vox::StaticRenderable>();
    _skybox->AddGeometryMesh(std::make_shared<Vox::CubeMesh>(2.0f, 2.0f, 2.0f, Vox::VertexFormat::Position3));
    _skybox->AttachMaterial(cubeMapMaterial);

    //! Create post-processing shader which do gamma-correnction and tone mapping for hdr.
    vs = Vox::Renderer::CreateShaderFromFile(Vox::FileSystem::FindPath("quad.glsl"), GL_VERTEX_SHADER);
    fs = Vox::Renderer::CreateShaderFromFile(Vox::FileSystem::FindPath("output.glsl"), GL_FRAGMENT_SHADER);
    _postProcessing.reset(new Vox::PostProcessing());
    _postProcessing->Initialize(Vox::Renderer::CreateProgram(vs, 0, fs));
    glDeleteShader(vs);
    glDeleteShader(fs);

    Vox::App::PushFrameContextToQueue(ctx);
    return true;
}

void VoxRenderer::UpdateFrame(double dt)
{
    //! Upload view projection and cam position to mesh shader.
    _camera->UploadToProgram(_skybox->GetMaterial()->GetProgram());
    for (auto& anim : _fluidAnims)
        _camera->UploadToProgram(anim->GetMaterial()->GetProgram());
    for (auto& obstacle : _obstacles)
        _camera->UploadToProgram(obstacle->GetMaterial()->GetProgram());

    //! Advance fluid animation buffer frame.
    UNUSED_VARIABLE(dt);
}

void VoxRenderer::OnSetKey(int key, int scancode, int action, int mods)
{
    UNUSED_VARIABLE(scancode);
    UNUSED_VARIABLE(mods);

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        _animStopped = !_animStopped;
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
        _polygonMode = _polygonMode == GL_FILL ? GL_LINE : GL_FILL;
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

        _skybox->DrawRenderableObject(ctx);

        glPolygonMode(GL_FRONT_AND_BACK, _polygonMode);

        //! Render fluid animation buffer with brdf mesh shader.
        for (auto& anim : _fluidAnims)
        {
            if (anim->CheckFence(50000))
            {
                anim->AsyncBufferTransfer();
                anim->DrawRenderableObject(ctx);

                if (!_animStopped)
                    anim->AdvanceFrame();
            }
        }

        for (auto& obstacle : _obstacles)
            obstacle->DrawRenderableObject(ctx);
    }

    //! Screen Pass
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    {
        //! Clear the screen.
        Vox::App::BeginFrame(ctx);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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