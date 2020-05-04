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

#ifdef CUBBYFLOW_USE_GL
#include <GL3/Buffer/GL3Framebuffer.h>
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
    
    void GL3Framebuffer::configure(RendererPtr renderer)
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
            std::abort();
        }

        this->unbind(renderer);
    }
    
    void GL3Framebuffer::onBind(RendererPtr renderer)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, _fboID);
        unsigned int clearFlag = 0;
        if (_colorTextures.empty() == false)
            clearFlag |= GL_COLOR_BUFFER_BIT;
        if (_depthTexture)
            clearFlag |= GL_DEPTH_BUFFER_BIT;
        glClear(clearFlag);
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
    
    void GL3Framebuffer::onAllocateBuffer(RendererPtr renderer)
    {
        glGenFramebuffers(1, &_fboID);

        this->bind(renderer);

        //! Attach color textures.
        for (unsigned int i = 0; i < _colorTextures.size(); ++i)
        {
            _colorTextures[i]->bind(renderer, i);
            //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorTexture, 0);
        }

        //! Attach depth texture
        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
        //! Attach depth buffer with depth texture size.
	    _depthTexture->bind(renderer, 0);
	    glGenRenderbuffers(1, &_rboID);
	    glBindRenderbuffer(GL_RENDERBUFFER, _rboID);
        Size2 size = _depthTexture->getTextureSize();
	    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, static_cast<GLsizei>(size.x), static_cast<GLsizei>(size.y));
	
	    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _rboID);
        this->unbind(renderer);
    }
} 
}

#endif