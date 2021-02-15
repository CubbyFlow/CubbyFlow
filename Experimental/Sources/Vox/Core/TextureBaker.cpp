/*************************************************************************
> File Name: TextureBaker.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Texture baker class for baking texture in advance rendering loop
> Created Time: 2020/12/30
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Core/TextureBaker.hpp>
#include <Vox/Core/Texture.hpp>
#include <Vox/Core/Renderer.hpp>
#include <Vox/Core/Program.hpp>
#include <Vox/Core/FrameBuffer.hpp>
#include <Vox/Mesh/CubeMesh.hpp>
#include <Vox/Utils/MatrixUtils.hpp>
#include <glad/glad.h>

namespace Vox
{
    std::shared_ptr<Texture> TextureBaker::BakingCubeMap(const std::shared_ptr<FrameContext>& ctx, const std::shared_ptr<Texture>& equirect,
                                                         GLsizei resolutionX, GLsizei resolutionY)
    {
        GLuint vs, fs;
        vs = Renderer::CreateShaderFromFile(FileSystem::FindPath("baking_cubemap_vs.glsl"), GL_VERTEX_SHADER);
        fs = Renderer::CreateShaderFromFile(FileSystem::FindPath("baking_cubemap_fs.glsl"), GL_FRAGMENT_SHADER);
        auto equirectToCubeMapProgram = std::make_shared<Program>(Renderer::CreateProgram(vs, 0, fs));
        glDeleteShader(vs);
        glDeleteShader(fs);

        auto captureFramebuffer = std::make_shared<FrameBuffer>(Renderer::CreateFrameBuffer());
        captureFramebuffer->BindFrameBuffer(GL_FRAMEBUFFER);
        captureFramebuffer->AttachRenderBuffer(Renderer::CreateRenderBuffer(resolutionX, resolutionY, PixelFmt::PF_DEPTH_COMPONENT24));

        //! Empty brackets mean we will not pass any data for generating cube map.
        auto cubeMap = std::make_shared<Texture>(GL_TEXTURE_CUBE_MAP, Renderer::CreateCubeMap(resolutionX, resolutionY, PixelFmt::PF_RGB16F, {}));
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        //! Generate cube mesh for baking
        auto cubeMesh = std::make_shared<CubeMesh>(2.0f, 2.0f, 2.0f, VertexFormat::Position3);

        //! Create view and projection matrices for capturing
        CubbyFlow::Matrix4x4F projection = Perspective(1.0f, 0.1f, 10.0f, 90.0f);
        CubbyFlow::Matrix4x4F views[] = {
            LookAt(CubbyFlow::Vector3F(0.0f, 0.0f, 0.0f), CubbyFlow::Vector3F( 1.0f,  0.0f,  0.0f), CubbyFlow::Vector3F(0.0f, -1.0f,  0.0f)),
            LookAt(CubbyFlow::Vector3F(0.0f, 0.0f, 0.0f), CubbyFlow::Vector3F(-1.0f,  0.0f,  0.0f), CubbyFlow::Vector3F(0.0f, -1.0f,  0.0f)),
            LookAt(CubbyFlow::Vector3F(0.0f, 0.0f, 0.0f), CubbyFlow::Vector3F( 0.0f,  1.0f,  0.0f), CubbyFlow::Vector3F(0.0f,  0.0f,  1.0f)),
            LookAt(CubbyFlow::Vector3F(0.0f, 0.0f, 0.0f), CubbyFlow::Vector3F( 0.0f, -1.0f,  0.0f), CubbyFlow::Vector3F(0.0f,  0.0f, -1.0f)),
            LookAt(CubbyFlow::Vector3F(0.0f, 0.0f, 0.0f), CubbyFlow::Vector3F( 0.0f,  0.0f,  1.0f), CubbyFlow::Vector3F(0.0f, -1.0f,  0.0f)),
            LookAt(CubbyFlow::Vector3F(0.0f, 0.0f, 0.0f), CubbyFlow::Vector3F( 0.0f,  0.0f, -1.0f), CubbyFlow::Vector3F(0.0f, -1.0f,  0.0f))
        };

        auto& params = equirectToCubeMapProgram->GetParameters();
        params.SetParameter("equirectangularMap", 0);
        params.SetParameter("camera.projection", projection);
        equirect->BindTexture(0);

        captureFramebuffer->BindFrameBuffer(GL_FRAMEBUFFER);
        glViewport(0, 0, resolutionX, resolutionY);
        for (int i = 0; i < 6; ++i)
        {
            params.SetParameter("camera.view", views[i]);
            equirectToCubeMapProgram->BindProgram(ctx);
            captureFramebuffer->AttachTexture(0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubeMap);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            cubeMesh->DrawMesh(ctx);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return cubeMap;
    }
    
};  // namespace Vox