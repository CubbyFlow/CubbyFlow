/*************************************************************************
> File Name: VertexBuffer.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for VertexBuffer object.
> Created Time: 2020/03/16
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Framework/Buffer/VertexBuffer.h>
#include <Framework/Buffer/InputLayout.h>
#include <Core/Size/Size.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    VertexBuffer::VertexBuffer()
    {
        //! Do nothing
    }

    VertexBuffer::~VertexBuffer()
    {
        //! Do nothing
    }

    void VertexBuffer::allocateBuffer(RendererPtr renderer, const ConstArrayAccessor1<float>& data, size_t numberOfVertices)
    {
        if (numberOfVertices == size_t(0))
        {
            destroy();
        }
        else if (numberOfVertices == _numberOfVertices)
        {
            updateBuffer(renderer, data);
        }
        else
        {
            destroy();
            _numberOfVertices = numberOfVertices;
            onAllocateBuffer(renderer, data);
        }
    }

    size_t VertexBuffer::getNumberOfVertices() const
    {
        return _numberOfVertices;
    }

    void VertexBuffer::setInputVertexFormat(VertexFormat format)
    {
        _vertexFormat = format;
    }

    VertexFormat VertexBuffer::getInputVertexFormat() const
    {
        return _vertexFormat;
    }

    void VertexBuffer::bindState(RendererPtr renderer, MaterialPtr material)
    {
        onBindState(renderer, material);
    }
} 
}