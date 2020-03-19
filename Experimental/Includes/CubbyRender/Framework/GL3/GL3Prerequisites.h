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
    class GL3Window;

    using GL3Renderer = std::shared_ptr<GL3Renderer>;
    using GL3Shader = std::shared_ptr<GL3Shader>;
    using GL3Application = std::shared_ptr<GL3Application>;
    using GL3Texture = std::shared_ptr<GL3Texture>;
    using GL3VertexBuffer = std::shared_ptr<GL3VertexBuffer>;
    using GL3IndexBuffer = std::shared_ptr<GL3IndexBuffer>;
    using GL3Window = std::shared_ptr<GL3Window>;
}
}

#endif

#endif 