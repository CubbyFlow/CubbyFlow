/*************************************************************************
> File Name: GLTexture2D.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: the abstract base class for texture/
> Created Time: 2020/05/03
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <GL3/Texture/GLTexture2D.h>
#include <cassert>

#ifdef CUBBYFLOW_USE_GL

namespace CubbyFlow {
namespace CubbyRender {

    GLTexture2D::GLTexture2D() 
        : GLTexture(GL_TEXTURE_2D)
    {
        //! Do nothing.
    }

    GLTexture2D::~GLTexture2D()
    {
        //! Do nothing.
    }

    void GLTexture2D::updateTexture(RendererPtr renderer, MaterialPtr material, const ConstArrayAccessor2<Vector4F>& data)
    {
        glBindTexture(GLTexture::getTextureTarget(), GLTexture::getGLTextureID());
        glTexSubImage2D(GLTexture::getTextureTarget(), 0, 0, 0, static_cast<GLsizei>(_textureSize.x),
                    static_cast<GLsizei>(_textureSize.y), GL_RGBA, GL_FLOAT,
                    data.data());
    }

    void GLTexture2D::updateTexture(RendererPtr renderer, MaterialPtr material, const ConstArrayAccessor2<Vector4UB>& data)
    {
        glBindTexture(GLTexture::getTextureTarget(), GLTexture::getGLTextureID());
        glTexSubImage2D(GLTexture::getTextureTarget(), 0, 0, 0, static_cast<GLsizei>(_textureSize.x),
                    static_cast<GLsizei>(_textureSize.y), GL_RGBA, GL_UNSIGNED_BYTE,
                    data.data());
    }

    void GLTexture2D::onSamplingModeChanged(const TextureSamplingMode& mode)
    {
        GLTextureParameters texParam = GLTexture::getGLTextureParameters();
        if (mode == TextureSamplingMode::kNearest)
        {
            texParam.minFilter = GL_NEAREST;
            texParam.magFilter = GL_NEAREST;
            GLTexture::setGLTextureParameters(texParam);
        }
        else if (mode == TextureSamplingMode::kLinear)
        {
            texParam.minFilter = GL_LINEAR;
            texParam.magFilter = GL_LINEAR;
            GLTexture::setGLTextureParameters(texParam);
        }
        else
        {
            CUBBYFLOW_ERROR << "Unknown TextureSamplingMode enum was given.";
            std::abort();
        }
    }

    void GLTexture2D::onBind(RendererPtr renderer, unsigned int slotID)
    {
        UNUSED_VARIABLE(renderer);
        GLTexture::bindTexture(slotID);
    }

    void GLTexture2D::onDestroy()
    {
        GLTexture::destroyTexture();
    }

    void GLTexture2D::onAllocateTexture(RendererPtr renderer, MaterialPtr material, const ConstArrayAccessor2<Vector4F>& data)
    {
        GLTexture::createTexture();

        GLuint target = GLTexture::getTextureTarget();
        GLTextureParameters texParam = GLTexture::getGLTextureParameters();

        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, texParam.minFilter);
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, texParam.magFilter);
        glTexParameteri(target, GL_TEXTURE_WRAP_S, texParam.wrapS);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, texParam.wrapT);

        glTexImage2D(target, 0, GL_RGBA8, static_cast<GLsizei>(_size.x),
                     static_cast<GLsizei>(_size.y), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                     data.data());
    }

    void GLTexture2D::onAllocateTexture(RendererPtr renderer, MaterialPtr material, const ConstArrayAccessor2<Vector4UB>& data)
    {
        //! Do nothing.
    }

} 
} 

#endif