/*************************************************************************
> File Name: Prerequisites.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Forward declaration of frequently used classes
> Created Time: 2020/02/15
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_PREREQUISITES_H
#define CUBBYFLOW_PREREQUISITES_H

#include <memory>

namespace CubbyFlow {
namespace CubbyRender {
    class Object;
    class VertexBuffer;
    class Shader;
    class Renderer;

    using ObjectPtr         = std::shared_ptr<Object>;
    using VertexBufferPtr   = std::shared_ptr<VertexBuffer>;
    using ShaderPtr         = std::shared_ptr<Shader>;
    using RendererPtr       = std::shared_ptr<Renderer>;
}
}

#endif