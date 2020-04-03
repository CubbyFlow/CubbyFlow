/*************************************************************************
> File Name: Buffer.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for Renderer Object
> Created Time: 2020/02/19
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Framework/Buffer/Buffer.h>
#include <Framework/Buffer/InputLayout.h>

namespace CubbyFlow {
namespace CubbyRender {

    Buffer::Buffer()
    {
        //! Do nothing
    }   
    
    Buffer::~Buffer()
    {
        //! Do nothing
    }

    void Buffer::bind(RendererPtr renderer)
    {
        onBind(renderer);
    }

    void Buffer::unbind(RendererPtr renderer)
    {
        onUnbind(renderer);
    }

    void Buffer::destroy()
    {
        onDestroy();
    }
} 
}