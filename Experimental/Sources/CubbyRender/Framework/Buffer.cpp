/*************************************************************************
> File Name: Buffer.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for buffer object which need 
            to be allocated by shader program.
> Created Time: 2020/03/17
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Framework/Buffer.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    Buffer::Buffer()
    {
        //! Do nothing.
    }

    Buffer::Buffer(const float* data, size_t numberOfElements, VertexFormat format)
        : _data(data), _numberOfElements(numberOfElements), _format(format)
    {
        //! Do nothing.
    }

    Buffer::~Buffer()
    {
        if (_data) 
            delete[] _data;
    }

    void Buffer::destroy(RendererPtr renderer)
    {
        UNUSED_VARIABLE(renderer);
        if (_data) 
        {
            delete[] _data;
        }
    }

    void Buffer::allocateResource(RendererPtr renderer, MaterialPtr material, const float* data, bool storeData)
    {
        onAllocateResource(renderer, material, data, storeData);
    }

    void Buffer::updateResource(RendererPtr renderer, MaterialPtr material, const float* data, 
                                size_t numberOfElements, VertexFormat format, bool storeData)
    {
        this->_numberOfElements = numberOfElements;
        this->_format           = format;
        onUpdateResource(renderer, material, data, storeData);
    }

} 
}