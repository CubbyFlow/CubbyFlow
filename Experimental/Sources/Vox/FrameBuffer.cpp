/*************************************************************************
> File Name: FrameBuffer.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: OpenGL FrameBufferObject abstraction
> Created Time: 2020/07/30
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/FrameBuffer.hpp>
#include <Vox/FileSystem.hpp>
#include <Vox/DebugUtils.hpp>
#include <glad/glad.h>

namespace Vox {

    FrameBuffer::FrameBuffer()
    {
        glGenFramebuffers(1, &_fbo);
    }

    FrameBuffer::~FrameBuffer()
    {
        if (_fbo) glDeleteFramebuffers(1, &_fbo);
    }

    void FrameBuffer::BindFrameBuffer(GLenum target)
    {
        glBindFramebuffer(target, _fbo);
    }

    void FrameBuffer::SetColorAttachment(GLenum index, GLuint texture)
    {
        VoxAssert(index < 32, CURRENT_SRC_PATH_TO_STR ,"ColorAttachment index should be less than 32");
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, texture, 0);
    }

    void FrameBuffer::SetRenderBufferAttachment(GLuint rbo)
    {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    }

    bool FrameBuffer::AssertFramebufferStatus() const
    {
        return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    }
};
