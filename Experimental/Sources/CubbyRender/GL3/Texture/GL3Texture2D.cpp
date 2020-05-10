/*************************************************************************
> File Name: GL3Texture2D.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: the abstract base class for texture/
> Created Time: 2020/05/03
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <GL3/Texture/GL3Texture2D.h>
#include <cassert>

#ifdef CUBBYFLOW_USE_GL

#include <GL3/Enum/GL3EnumHelper.h>

namespace CubbyFlow {
namespace CubbyRender {

    GL3Texture2D::GL3Texture2D() 
        : GL3Texture(GL_TEXTURE_2D)
    {
        //! Do nothing.
    }

    GL3Texture2D::~GL3Texture2D()
    {
        //! Do nothing.
    }

    void GL3Texture2D::updateTexture(RendererPtr renderer, void* data)
    {
        UNUSED_VARIABLE(renderer);
        GLuint target = GL3Texture::getTextureTarget();
        GL3TextureParameters texParam = GL3Texture::getGLTextureParameters();
        glBindTexture(target, GL3Texture::getGLTextureID());
        glTexSubImage2D(target, 0, 0, 0, static_cast<GLsizei>(_textureSize.x),
                    static_cast<GLsizei>(_textureSize.y), texParam.format, texParam.type,
                    data);
    }

    void GL3Texture2D::onSetTextureParams(const TextureParams& param)
    {
        GL3TextureParameters glParam = GL3Texture::getGLTextureParameters();
        
        glParam.minFilter = glParam.magFilter = GL3EnumHelper::ImageSamplingModeConverter(param.samplingMode);
        glParam.format = GL3EnumHelper::ImageFormatConverter(param.format);
        glParam.internalFormat = GL3EnumHelper::ImageInternalFormatConverter(param.internalFormat);
        glParam.wrapR = glParam.wrapS = glParam.wrapT = GL3EnumHelper::ImageWrapMethodConverter(param.wrapMethod);

        GL3Texture::setGLTextureParameters(glParam);
    }

    void GL3Texture2D::onBind(RendererPtr renderer, unsigned int slotID)
    {
        UNUSED_VARIABLE(renderer);
        GL3Texture::bindGLTexture(slotID);
    }

    void GL3Texture2D::onDestroy()
    {
        GL3Texture::destroyGLTexture();
    }

    void GL3Texture2D::onAllocateTexture(RendererPtr renderer, void* data)
    {
        UNUSED_VARIABLE(renderer);
        GL3Texture::createGLTexture();

        GLuint target = GL3Texture::getTextureTarget();
        GL3TextureParameters texParam = GL3Texture::getGLTextureParameters();

        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, texParam.minFilter);
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, texParam.magFilter);
        glTexParameteri(target, GL_TEXTURE_WRAP_S, texParam.wrapS);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, texParam.wrapT);

        glTexImage2D(target, 0, texParam.internalFormat, static_cast<GLsizei>(_textureSize.x),
                     static_cast<GLsizei>(_textureSize.y), 0, texParam.format, texParam.type,
                     data);
    }

} 
} 

#endif