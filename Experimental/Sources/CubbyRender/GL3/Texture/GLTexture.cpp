/*************************************************************************
> File Name: GLTexture.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: the abstract base class for texture/
> Created Time: 2020/02/24
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <GL3/Texture/GLTexture.h>

#ifdef CUBBYFLOW_USE_GL

namespace CubbyFlow {
namespace CubbyRender {

        GLTexture::GLTexture()
        {
            //! Do nothing.
        }

        GLTexture::GLTexture(GLuint target)
            : _target(target)
        {
            //! Do nothing.
        }

        GLTexture::~GLTexture()
        {
            //! Do nothing.
        }

        void GLTexture::setGLTextureParameters(const GLTextureParameters& glTexParams)
        {
            _glTexParams = glTexParams;

            glBindTexture(_target, _textureID);
            glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, _glTexParams.minFilter);
            glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, _glTexParams.magFilter);
            glTexParameteri(_target, GL_TEXTURE_WRAP_S, _glTexParams.wrapS);
            glTexParameteri(_target, GL_TEXTURE_WRAP_T, _glTexParams.wrapT);
            glTexParameteri(_target, GL_TEXTURE_WRAP_R, _glTexParams.wrapR);
        }

        GLTextureParameters GLTexture::getGLTextureParameters() const
        {
            return _glTexParams
        }

        GLuint GLTexture::getGLTextureID() const
        {
            return _textureID;
        }

        void GLTexture::createTexture()
        {
            glGenTextures(1, &_textureID);
        }

        void GLTexture::destroyTexture()
        {
            if (static_cast<int>(_textureID))
            {
                glDeleteTextures(1, &_textureID);
            }
            _textureID = GLuint(0);
        }

        void GLTexture::bindTexture(unsigned int slotID)
        {
            if (slotID >= 32U)
            {
                CUBBYFLOW_ERROR << "OpenGL Texture slot id should be smaller than 32";
                std::abort();
            }
            glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(slotID));
            glBindTexture(_target, _textureID);
        }

        GLuint GLTexture::getTextureTarget() const
        {
            return _target;
        }

} 
} 

#endif