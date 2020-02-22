/*************************************************************************
> File Name: GL3VertexBuffer.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: VertexBuffer class implemented by modern opengl
> Created Time: 2020/02/22
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#ifdef CUBBYFLOW_USE_GL

#include <Framework/GL3/GL3VertexBuffer.h>
#include <Framework/Renderer.h>
#include <Framework/Common.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    GL3VertexBuffer::GL3VertexBuffer()
    {
        //! Do nothing
    }

    GL3VertexBuffer::~GL3VertexBuffer()
    {
        //! Do nothing
    }

    void GL3VertexBuffer::onBind(RendererPtr renderer)
    {
        UNUSED_VARIABLE(renderer);
    }
        
    void GL3VertexBuffer::onUnbind(RendererPtr renderer) 
    {
        UNUSED_VARIABLE(renderer);
    }

    void GL3VertexBuffer::onDestroy() 
    {
        //! Do nothing
    }

} 
}

#endif