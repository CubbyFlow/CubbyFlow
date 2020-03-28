/*************************************************************************
> File Name: VertexBuffer.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for VertexBuffer object.
> Created Time: 2020/03/16
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Framework/VertexBuffer.h>
#include <Core/Size/Size.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    VertexBuffer::VertexBuffer()
    {
        //! Do nothing
    }

    VertexBuffer::VertexBuffer(VertexFormat format)
        : _format(format)
    {
        //! Do nothing
    }

    VertexBuffer::~VertexBuffer()
    {
        //! Do nothing
    }

    void VertexBuffer::allocateBuffer(RendererPtr renderer, MaterialPtr material, const ConstArrayAccessor1<float>& data, size_t numberOfVertices, bool storeData)
    {
        if (storeData)
        {
            if (numberOfVertices != _numberOfVertices)
            {
                _data.Resize(numberOfVertices);
            }
            
            data.ParallelForEachIndex([&](size_t i){
                _data[i] = data[i];
            });
        }

        if (numberOfVertices == size_t(0))
        {
            destroy();
        }
        else if (numberOfVertices == _numberOfVertices)
        {
            updateBuffer(renderer, material, data, false);
        }
        else
        {
            destroy();
            _numberOfVertices = numberOfVertices;
            onAllocateBuffer(renderer, material, data);
        }
    }

    size_t VertexBuffer::getNumberOfVertices() const
    {
        return _numberOfVertices;
    }

    VertexFormat VertexBuffer::getFormat() const
    {
        return _format;
    }

} 
}