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
#include <Vox/Renderer.hpp>
#include <Vox/FrameContext.hpp>

#include <glad/glad.h>

namespace Vox {

    S3TextureCompression::S3TextureCompression(GLsizei width, GLsizei height)
        : _width(width), _height(height)
    {
        _texIm = Renderer::CreateTexture((width + 3)/4, (height + 3)/4, PixelFmt::PF_RGBA32UI, nullptr);
        _texDXT = Renderer::CreateTexture(width, height, PixelFmt::PF_DXT5, nullptr);
        _texFinal = Renderer::CreateTexture(width, height, PixelFmt::PF_RGBA8, nullptr);
        
        glGenVertexArrays(1, &_vao);

        glGenBuffers(1, &_vboDXT);
        glBindBuffer(GL_ARRAY_BUFFER, _vboDXT);
        glBufferData(GL_ARRAY_BUFFER, width * height, nullptr, GL_STREAM_COPY);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        GLuint vs = Renderer::CreateShaderFromSource(kS3TCShaders[0], GL_VERTEX_SHADER);
        GLuint fs = Renderer::CreateShaderFromSource(kS3TCShaders[1], GL_FRAGMENT_SHADER);
        _s3tcProgram = Renderer::CreateProgram(vs, 0, fs);
        glDeleteShader(vs);
        glDeleteShader(fs);

        _s3tcFramebuffer.reset(new FrameBuffer());
        _s3tcFramebuffer->BindFrameBuffer(GL_FRAMEBUFFER);
        _s3tcFramebuffer->SetColorAttachment(0, _texIm);
    }

    S3TextureCompression::~S3TextureCompression()
    {
        if (_vao) glDeleteVertexArrays(1, &_vao);
        if (_texIm) glDeleteTextures(1, &_texIm);
        if (_texDXT) glDeleteTextures(1, &_texDXT);
        if (_texFinal) glDeleteTextures(1, &_texFinal);
        if (_vboDXT) glDeleteBuffers(1, &_vboDXT);
        if (_s3tcProgram) glDeleteProgram(_s3tcProgram);
        if (_s3tcFramebuffer) _s3tcFramebuffer.reset();
    }

    GLuint S3TextureCompression::DXT5Compress(const std::shared_ptr<FrameContext>& ctx, const std::string& textureName)
    {
        _s3tcFramebuffer->BindFrameBuffer(GL_FRAMEBUFFER);
        {
            ctx->BindTextureToSlot(textureName, GL_TEXTURE_2D, 0);

            glViewport(0, 0, _width / 4, _height / 4);
            glUseProgram(_s3tcProgram);
            
            glBindVertexArray(_vao);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glBindVertexArray(0);

            glReadBuffer(GL_COLOR_ATTACHMENT0);
            glBindBuffer(GL_PIXEL_PACK_BUFFER, _vboDXT);
            glReadPixels(0, 0, _width / 4, _height / 4, GL_RGBA_INTEGER, GL_UNSIGNED_INT, nullptr);
            glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            glBindTexture(GL_TEXTURE_2D, _texDXT);
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, _vboDXT);
            
            glCompressedTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, 
                                      GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, 
                                      _width * _height, nullptr);

            glBindTexture(GL_TEXTURE_2D, 0);
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        }

        return _texDXT;
    }

};