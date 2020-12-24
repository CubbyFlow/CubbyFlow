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
#include <Vox/ShaderPreset.hpp>
#include <Vox/Texture.hpp>
#include <Vox/FrameBuffer.hpp>
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
        UNUSED_VARIABLE(ctx);
        _texIm = std::make_shared<Texture>(GL_TEXTURE_2D, Renderer::CreateTexture((_width + 3)/4, (_height + 3)/4, PixelFmt::PF_RGBA32UI, nullptr));
        _texDXT = std::make_shared<Texture>(GL_TEXTURE_2D, Renderer::CreateTexture(_width, _height, PixelFmt::PF_DXT5, nullptr));
        _texFinal = std::make_shared<Texture>(GL_TEXTURE_2D, Renderer::CreateTexture(_width, _height, PixelFmt::PF_RGBA8, nullptr));
		
        auto param = _texFinal->GetParameters();
        param.minFilter = GL_NEAREST;
        param.magFilter = GL_NEAREST;
        _texFinal->SetParameters(param);

        glGenVertexArrays(1, &_vao);
        glGenBuffers(1, &_vboDXT);
        glBindBuffer(GL_ARRAY_BUFFER, _vboDXT);
        glBufferData(GL_ARRAY_BUFFER, _width * _height, nullptr, GL_STREAM_COPY);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        GLuint vs = Renderer::CreateShaderFromSource(kS3TCShaders[0], GL_VERTEX_SHADER);
        GLuint fs = Renderer::CreateShaderFromSource(kS3TCShaders[1], GL_FRAGMENT_SHADER);
        _s3tcProgram = std::make_shared<Program>(Renderer::CreateProgram(vs, 0, fs));
        glDeleteShader(fs);

        auto& params = _s3tcProgram->GetParameters();
        params.SetParameter("ScreenTexture", 0);

        fs = Renderer::CreateShaderFromSource(kYCoCgDecodingShaders[1], GL_FRAGMENT_SHADER);
        _decodingProgram = std::make_shared<Program>(Renderer::CreateProgram(vs, 0, fs));
        glDeleteShader(vs);
        glDeleteShader(fs);

        params = _s3tcProgram->GetParameters();
        params.SetParameter("ScreenTexture", 0);

        _s3tcPass = std::make_shared<FrameBuffer>(Renderer::CreateFrameBuffer());
        _s3tcPass->AttachTexture(0, _texIm, false);
        VoxAssert(_s3tcPass->ValidateFrameBufferStatus(), CURRENT_SRC_PATH_TO_STR, "Frame Buffer Status incomplete");

        _decodingPass = std::make_shared<FrameBuffer>(Renderer::CreateFrameBuffer());
        _decodingPass->AttachTexture(0, _texFinal, false);
        VoxAssert(_decodingPass->ValidateFrameBufferStatus(), CURRENT_SRC_PATH_TO_STR, "Frame Buffer Status incomplete");
    }

    void S3TextureCompression::CompressionPass(const std::shared_ptr<FrameContext>& ctx, const std::shared_ptr<Texture>& compressTarget)
    {
        _s3tcPass->BindFrameBuffer(GL_FRAMEBUFFER);
        {
            compressTarget->BindTexture(0);

            glViewport(0, 0, _width / 4, _height / 4);
            _s3tcProgram->BindProgram(ctx->GetContextScene());
            
            glBindVertexArray(_vao);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glBindVertexArray(0);

            glReadBuffer(GL_COLOR_ATTACHMENT0);
            glBindBuffer(GL_PIXEL_PACK_BUFFER, _vboDXT);
            glReadPixels(0, 0, _width / 4, _height / 4, GL_RGBA_INTEGER, GL_UNSIGNED_INT, nullptr);
            glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
        }
    }

    std::shared_ptr<Texture> S3TextureCompression::DecodingPass(const std::shared_ptr<FrameContext>& ctx)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        {
            _texDXT->BindTexture(0);
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, _vboDXT);
            
            glCompressedTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, 
                                      GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, 
                                      _width * _height, nullptr);

            glBindTexture(GL_TEXTURE_2D, 0);
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        }

        _decodingPass->BindFrameBuffer(GL_FRAMEBUFFER);
        {
            _texDXT->BindTexture(0);
            _decodingProgram->BindProgram(ctx->GetContextScene());
            glViewport(0, 0, _width, _height);

            glBindVertexArray(_vao);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glBindVertexArray(0);
        }

        return _texFinal;
    }
};