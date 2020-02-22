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
    cla

    using GL3RendererPtr = std::shared_ptr<GL3Renderer>;
}
}

#endif

#endif 