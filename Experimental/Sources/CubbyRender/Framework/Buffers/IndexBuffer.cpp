/*************************************************************************
> File Name: IndexBuffer.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for IndexBuffer object.
> Created Time: 2020/03/16
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Framework/Buffer/IndexBuffer.h>
#include <Framework/Buffer/InputLayout.h>
#include <Framework/Renderer/Renderer.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    IndexBuffer::IndexBuffer()
    {
        //! Do nothing
    }

    IndexBuffer::~IndexBuffer()
    {
        //! Do nothing
    }

    void IndexBuffer::allocateBuffer(RendererPtr renderer, const ConstArrayAccessor1<unsigned int>& data, size_t numberOfIndices)
    {
        if (numberOfIndices == size_t(0))
        {
            destroy();
        }
        else if (numberOfIndices == _numberOfIndices)
        {
            updateBuffer(renderer, data);
        }
        else
        {
            destroy();
            _numberOfIndices = numberOfIndices;
            onAllocateBuffer(renderer, data);
        }
    }

    size_t IndexBuffer::getNumberOfIndices() const
    {
        return _numberOfIndices;
    }

    void IndexBuffer::bindState(RendererPtr renderer)
    {
        onBindState(renderer);
    }

} 
}