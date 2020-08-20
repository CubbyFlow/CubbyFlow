/*************************************************************************
> File Name: S3TextureCompression.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> References: https://github.com/OpenGLInsights/OpenGLInsightsCode
> Purpose: Video capture class using Texture Compression technique
> Created Time: 2020/08/05
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/S3TextureCompression.hpp>
#include <Vox/FrameBuffer.hpp>
#include <Vox/ShaderPreset.hpp>
#include <Vox/Program.hpp>
#include <Vox/Renderer.hpp>
#include <Vox/FrameContext.hpp>

#include <glad/glad.h>

namespace Vox {

    S3TextureCompression::S3TextureCompression(GLsizei width, GLsizei height)
        : _width(width), _height(height)
    {
    }

    S3TextureCompression::~S3TextureCompression()
    {
        if (_vao) glDeleteVertexArrays(1, &_vao);
        if (_vboDXT) glDeleteBuffers(1, &_vboDXT);
    }

    void S3TextureCompression::Initialize(const std::shared_ptr<FrameContext>& ctx)
    {
        GLuint texIm = Renderer::CreateTexture((_width + 3)/4, (_height + 3)/4, PixelFmt::PF_RGBA32UI, nullptr);
        GLuint texDXT = Renderer::CreateTexture(_width, _height, PixelFmt::PF_DXT5, nullptr);
        GLuint texFinal = Renderer::CreateTexture(_width, _height, PixelFmt::PF_RGBA8, nullptr);
		
        glBindTexture(GL_TEXTURE_2D, texFinal);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        ctx->AddTexture("EncodingTexture", texIm);
        ctx->AddTexture("DXTexture5", texDXT);
        ctx->AddTexture("CompressedTexture", texFinal);

        glGenVertexArrays(1, &_vao);

        glGenBuffers(1, &_vboDXT);
        glBindBuffer(GL_ARRAY_BUFFER, _vboDXT);
        glBufferData(GL_ARRAY_BUFFER, _width * _height, nullptr, GL_STREAM_COPY);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        GLuint vs = Renderer::CreateShaderFromSource(kS3TCShaders[0], GL_VERTEX_SHADER);
        GLuint fs = Renderer::CreateShaderFromSource(kS3TCShaders[1], GL_FRAGMENT_SHADER);
        ctx->AddShaderProgram("S3TC", Renderer::CreateProgram(vs, 0, fs));
        glDeleteShader(fs);
        ctx->MakeProgramCurrent("S3TC");
        const auto& s3tc = ctx->GetCurrentProgram();
        if(!s3tc.expired())
            s3tc.lock()->SendUniformVariable("ScreenTexture", 0);

        fs = Renderer::CreateShaderFromSource(kYCoCgDecodingShaders[1], GL_FRAGMENT_SHADER);
        ctx->AddShaderProgram("YCoCgDecoding", Renderer::CreateProgram(vs, 0, fs));
        glDeleteShader(vs);
        glDeleteShader(fs);
        ctx->MakeProgramCurrent("YCoCgDecoding");
        const auto& ycocg = ctx->GetCurrentProgram();
        if(!ycocg.expired())
            ycocg.lock()->SendUniformVariable("ScreenTexture", 0);

        ctx->AddFrameBuffer("S3TCPass");
        ctx->BindFrameBuffer("S3TCPass", GL_FRAMEBUFFER);
        const auto& s3tcFBO = ctx->GetCurrentFrameBuffer().lock();
        s3tcFBO->SetColorAttachment(0, texIm, false);   
        VoxAssert(s3tcFBO->AssertFramebufferStatus(), CURRENT_SRC_PATH_TO_STR, "Frame Buffer Status incomplete");

        ctx->AddFrameBuffer("YCoCgDecodingPass");
        ctx->BindFrameBuffer("YCoCgDecodingPass", GL_FRAMEBUFFER);
        const auto& ycocgFBO = ctx->GetCurrentFrameBuffer().lock();
        ycocgFBO->SetColorAttachment(0, texFinal, false);   
        VoxAssert(ycocgFBO->AssertFramebufferStatus(), CURRENT_SRC_PATH_TO_STR, "Frame Buffer Status incomplete");
    }

    void S3TextureCompression::DXT5Compress(const std::shared_ptr<FrameContext>& ctx, const std::string& textureName)
    {
        ctx->BindFrameBuffer("S3TCPass", GL_FRAMEBUFFER);
        {
            ctx->BindTextureToSlot(textureName, GL_TEXTURE_2D, 0);

            glViewport(0, 0, _width / 4, _height / 4);
            ctx->MakeProgramCurrent("S3TC");
            
            glBindVertexArray(_vao);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glBindVertexArray(0);

            glReadBuffer(GL_COLOR_ATTACHMENT0);
            glBindBuffer(GL_PIXEL_PACK_BUFFER, _vboDXT);
            glReadPixels(0, 0, _width / 4, _height / 4, GL_RGBA_INTEGER, GL_UNSIGNED_INT, nullptr);
            glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
        }

        ctx->BindFrameBuffer("DefaultPass", GL_DRAW_FRAMEBUFFER);
        {
            ctx->BindTextureToSlot("DXTexture5", GL_TEXTURE_2D, 0);
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, _vboDXT);
            
            glCompressedTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, 
                                      GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, 
                                      _width * _height, nullptr);

            glBindTexture(GL_TEXTURE_2D, 0);
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        }

        ctx->BindFrameBuffer("YCoCgDecodingPass", GL_FRAMEBUFFER);
        {
            ctx->BindTextureToSlot("DXTexture5", GL_TEXTURE_2D, 0);
            ctx->MakeProgramCurrent("YCoCgDecoding");
            glViewport(0, 0, _width, _height);

            glBindVertexArray(_vao);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glBindVertexArray(0);
        }
    }

};