/*************************************************************************
> File Name: GL3Texture.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: the abstract base class for texture/
> Created Time: 2020/02/24
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <GL3/Texture/GL3Texture.h>

#ifdef CUBBYFLOW_USE_GL

namespace CubbyFlow {
namespace CubbyRender {

    GL3Texture::GL3Texture()
    {
        //! Do nothing.
    }
    
    GL3Texture::GL3Texture(GLuint target)
        : _target(target)
    {
        //! Do nothing.
    }

    GL3Texture::~GL3Texture()
    {
        //! Do nothing.
    }

    void GL3Texture::setGLTextureParameters(const GL3TextureParameters& glTexParams)
    {
        _glTexParams = glTexParams;
        glBindTexture(_target, _textureID);
        glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, _glTexParams.minFilter);
        glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, _glTexParams.magFilter);
        glTexParameteri(_target, GL_TEXTURE_WRAP_S, _glTexParams.wrapS);
        glTexParameteri(_target, GL_TEXTURE_WRAP_T, _glTexParams.wrapT);
        glTexParameteri(_target, GL_TEXTURE_WRAP_R, _glTexParams.wrapR);
    }

    GL3TextureParameters GL3Texture::getGLTextureParameters() const
    {
        return _glTexParams;
    }

    GLuint GL3Texture::getGLTextureID() const
    {
        return _textureID;
    }

    void GL3Texture::createGLTexture()
    {
        glGenTextures(1, &_textureID);
    }

    void GL3Texture::destroyGLTexture()
    {
        if (static_cast<int>(_textureID))
        {
            glDeleteTextures(1, &_textureID);
        }
        _textureID = GLuint(0);
    }

    void GL3Texture::bindGLTexture(unsigned int slotID)
    {
        if (slotID >= 32U)
        {
            CUBBYFLOW_ERROR << "OpenGL Texture slot id should be smaller than 32";
            std::abort();
        }
        glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(slotID));
        glBindTexture(_target, _textureID);
    }

    GLuint GL3Texture::getTextureTarget() const
    {
        return _target;
    }

} 
} 

#endif