/*************************************************************************
> File Name: GL3Texture.CPP
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: texture class opengl implementation
> Created Time: 2020/02/24
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#ifdef CUBBYFLOW_USE_GL

#include <Framework/GL3/GL3Texture.h>
#include <Core/Utils/Macros.h>
#include <glad/glad.h>

namespace CubbyFlow {
namespace CubbyRender {

    GL3Texture::GL3Texture()
    {
        //! Do nothing.
    }

    GL3Texture::~GL3Texture()
    {
        //! Do nothing.
    }

    void GL3Texture::update(const ConstArrayAccessor2<Vector4F>& data)
    {
        UNUSED_VARIABLE(data);
    }

    void GL3Texture::update(const ConstArrayAccessor2<Vector4B>& data)
    {
        UNUSED_VARIABLE(data);
    }

    void GL3Texture::onSetTexture(const ConstArrayAccessor2<Vector4F>& data)
    {
        UNUSED_VARIABLE(data);
    }

    void GL3Texture::onSetTexture(const ConstArrayAccessor2<Vector4B>& data)
    {
        UNUSED_VARIABLE(data);
    }

    void GL3Texture::onSamplingModeChanged(const TextureSamplingMode& mode)
    {
        UNUSED_VARIABLE(mode);
    }

    void GL3Texture::onBind(RendererPtr renderer)
    {
        UNUSED_VARIABLE(renderer);
    }

    void GL3Texture::onUnbind(RendererPtr renderer)
    {
        UNUSED_VARIABLE(renderer);
    }

    void GL3Texture::onDestroy(RendererPtr renderer)
    {
        UNUSED_VARIABLE(renderer);
    }
}
}

#endif