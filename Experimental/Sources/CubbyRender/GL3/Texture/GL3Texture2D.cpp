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

    void GL3Texture2D::updateTexture(RendererPtr renderer, const ConstArrayAccessor2<Vector4F>& data)
    {
        UNUSED_VARIABLE(renderer);
        glBindTexture(GL3Texture::getTextureTarget(), GL3Texture::getGLTextureID());
        glTexSubImage2D(GL3Texture::getTextureTarget(), 0, 0, 0, static_cast<GLsizei>(_textureSize.x),
                    static_cast<GLsizei>(_textureSize.y), GL_RGBA, GL_FLOAT,
                    data.data());
    }

    void GL3Texture2D::updateTexture(RendererPtr renderer, const ConstArrayAccessor2<Vector4UB>& data)
    {
        UNUSED_VARIABLE(renderer);
        glBindTexture(GL3Texture::getTextureTarget(), GL3Texture::getGLTextureID());
        glTexSubImage2D(GL3Texture::getTextureTarget(), 0, 0, 0, static_cast<GLsizei>(_textureSize.x),
                    static_cast<GLsizei>(_textureSize.y), GL_RGBA, GL_UNSIGNED_BYTE,
                    data.data());
    }

    void GL3Texture2D::onSamplingModeChanged(const TextureSamplingMode& mode)
    {
        GL3TextureParameters texParam = GL3Texture::getGLTextureParameters();
        if (mode == TextureSamplingMode::kNearest)
        {
            texParam.minFilter = GL_NEAREST;
            texParam.magFilter = GL_NEAREST;
            GL3Texture::setGLTextureParameters(texParam);
        }
        else if (mode == TextureSamplingMode::kLinear)
        {
            texParam.minFilter = GL_LINEAR;
            texParam.magFilter = GL_LINEAR;
            GL3Texture::setGLTextureParameters(texParam);
        }
        else
        {
            CUBBYFLOW_ERROR << "Unknown TextureSamplingMode enum was given.";
            std::abort();
        }
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

    void GL3Texture2D::onAllocateTexture(RendererPtr renderer, const ConstArrayAccessor2<Vector4F>& data)
    {
        UNUSED_VARIABLE(renderer);
        GL3Texture::createGLTexture();

        GLuint target = GL3Texture::getTextureTarget();
        GL3TextureParameters texParam = GL3Texture::getGLTextureParameters();

        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, texParam.minFilter);
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, texParam.magFilter);
        glTexParameteri(target, GL_TEXTURE_WRAP_S, texParam.wrapS);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, texParam.wrapT);

        glTexImage2D(target, 0, GL_RGBA8, static_cast<GLsizei>(_textureSize.x),
                     static_cast<GLsizei>(_textureSize.y), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                     data.data());
    }

    void GL3Texture2D::onAllocateTexture(RendererPtr renderer, const ConstArrayAccessor2<Vector4UB>& data)
    {
        UNUSED_VARIABLE(renderer);
        GL3Texture::createGLTexture();

        GLuint target = GL3Texture::getTextureTarget();
        GL3TextureParameters texParam = GL3Texture::getGLTextureParameters();

        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, texParam.minFilter);
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, texParam.magFilter);
        glTexParameteri(target, GL_TEXTURE_WRAP_S, texParam.wrapS);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, texParam.wrapT);

        glTexImage2D(target, 0, GL_RGBA8, static_cast<GLsizei>(_textureSize.x),
                     static_cast<GLsizei>(_textureSize.y), 0, GL_RGBA, GL_FLOAT,
                     data.data());
    }

} 
} 

#endif