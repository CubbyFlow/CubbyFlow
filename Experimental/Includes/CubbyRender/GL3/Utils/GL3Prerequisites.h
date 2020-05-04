/*************************************************************************
> File Name: GL3Prerequisites.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Forward declaration of frequently used GL3 classes
> Created Time: 2020/02/15
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_GL3PREREQUISITES_H
#define CUBBYFLOW_GL3PREREQUISITES_H

#ifdef CUBBYFLOW_USE_GL

#include <memory>

namespace CubbyFlow {
namespace CubbyRender {
    class GL3Renderer;
    class GL3Shader;
    class GL3Application;
    class GL3Texture;
    class GL3VertexBuffer;
    class GL3IndexBuffer;
    class GL3Texture;
    class GL3Texture2D;
    class GL3Texture3D;
    class GL3Window;

    using GL3RendererPtr        = std::shared_ptr<GL3Renderer>;
    using GL3ShaderPtr          = std::shared_ptr<GL3Shader>;
    using GL3ApplicationPtr     = std::shared_ptr<GL3Application>;
    using GL3VertexBufferPtr    = std::shared_ptr<GL3VertexBuffer>;
    using GL3TexturePtr         = std::shared_ptr<GL3Texture>;
    using GL3Texture2DPtr       = std::shared_ptr<GL3Texture2D>;
    using GL3Texture3DPtr       = std::shared_ptr<GL3Texture3D>;
    using GL3WindowPtr          = std::shared_ptr<GL3Window>;
}
}

#endif

#endif 