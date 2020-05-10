/*************************************************************************
> File Name: GL3Framebuffer.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Modern opengl framebuffer object wrapper class.
> Created Time: 2020/05/02
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#include <Framework/Texture/Texture2D.h>
#include <Framework/Renderer/Renderer.h>

#ifdef CUBBYFLOW_USE_GL
#include <GL3/Buffer/GL3Framebuffer.h>
#include <GL3/Texture/GL3Texture2D.h>
#include <cassert>

namespace CubbyFlow {
namespace CubbyRender {
    
    GL3Framebuffer::GL3Framebuffer()
    {
        //! Do nothing
    }

    GL3Framebuffer::~GL3Framebuffer()
    {
        destroy();
    }
    
    bool GL3Framebuffer::configure(RendererPtr renderer)
    {
        this->bind(renderer);
	
	    if (_colorTextures.empty())
	    {
	    	glDrawBuffer(GL_NONE);
	    	glReadBuffer(GL_NONE);
	    }

	    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            CUBBYFLOW_ERROR << "OpenGL Framebuffer Object ID [" << _fboID << "] Configure Failed.";
            return false;
        }

        this->unbind(renderer);
        return true;
    }
    
    void GL3Framebuffer::onBind(RendererPtr renderer)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, _fboID);
        renderer->setViewport(0, 0, _viewportSize.x, _viewportSize.y);
    }
    
    void GL3Framebuffer::onUnbind(RendererPtr renderer)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    void GL3Framebuffer::onDestroy()
    {
        if (static_cast<int>(_rboID))
        {
            glDeleteRenderbuffers(1, &_rboID);
        }
        if (static_cast<int>(_fboID))
        {
            glDeleteRenderbuffers(1, &_fboID);
        }
    }
    
    void GL3Framebuffer::allocateBuffer(RendererPtr renderer)
    {
        UNUSED_VARIABLE(renderer);
        glGenFramebuffers(1, &_fboID);
    }

    void GL3Framebuffer::onAttachColorTexture(RendererPtr renderer, Texture2DPtr texture)
    {
        bind(renderer);
        auto glTexture = std::dynamic_pointer_cast<GL3Texture2D>(texture);
        if (glTexture == nullptr)
        {
            CUBBYFLOW_ERROR << "Failed to dynamic casting from Texture2DPtr to GLTexture2DPtr";
            std::abort();
        }
        glTexture->bind(renderer, static_cast<unsigned int>(_colorTextures.size()));
        //! Attach color texture to framebuffer.
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + _colorTextures.size(), GL_TEXTURE_2D, glTexture->getGLTextureID(), 0);
        _colorTextures.push_back(texture);
        unbind(renderer);
    }

    void GL3Framebuffer::onAttachDepthTexture(RendererPtr renderer, Texture2DPtr texture)
    {
        bind(renderer);
        auto glTexture = std::dynamic_pointer_cast<GL3Texture2D>(texture);
        if (glTexture == nullptr)
        {
            CUBBYFLOW_ERROR << "Failed to dynamic casting from Texture2DPtr to GLTexture2DPtr";
            std::abort();
        }
        //! Attach depth texture to framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, glTexture->getGLTextureID(), 0);
	    glTexture->bind(renderer, 0U);
        //! Create render buffer and attach.
	    glGenRenderbuffers(1, &_rboID);
	    glBindRenderbuffer(GL_RENDERBUFFER, _rboID);
        Size2 size = glTexture->getTextureSize();
	    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, static_cast<GLsizei>(size.x), static_cast<GLsizei>(size.y));
	    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _rboID);
        _depthTexture = texture;
        unbind(renderer);
    }

} 
}

#endif