/*************************************************************************
> File Name: FrameBuffer.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: OpenGL FrameBuffer wrapper class
> Created Time: 2020/08/03
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Core/FrameBuffer.hpp>
#include <Vox/Core/Texture.hpp>
#include <glad/glad.h>

namespace Vox {

    FrameBuffer::FrameBuffer(GLuint framebufferID)
        : _id(framebufferID)
    {
        //! Do nothing.
    }

    FrameBuffer::~FrameBuffer()
    {
        if (_id) glDeleteFramebuffers(1, &_id);
        if (_rbo) glDeleteRenderbuffers(1, &_rbo);
    }

    void FrameBuffer::BindFrameBuffer(GLuint target)
    {
        glBindFramebuffer(target, _id);
    }

    void FrameBuffer::AttachTexture(GLsizei index, GLuint target, const std::shared_ptr<Texture>& texture)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, target, texture->GetTextureID(), 0);
        _textures[index] = texture;
	}

    void FrameBuffer::AttachRenderBuffer(GLuint rbo)
    {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
        _rbo = rbo;
    }

    bool FrameBuffer::ValidateFrameBufferStatus()
    {
        return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    }

};