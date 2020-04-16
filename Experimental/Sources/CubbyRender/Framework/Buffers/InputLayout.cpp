/*************************************************************************
> File Name: InputLayout.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Input layout which store vertex buffers and index buffers.  
            Same as VAO(Vertex Array Object) in OpenGL.
> Created Time: 2020/04/01
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Framework/Buffer/InputLayout.h>
#include <Framework/Buffer/VertexBuffer.h>
#include <Framework/Buffer/IndexBuffer.h>
#include <Framework/Renderer/Renderer.h>
#include <Framework/Renderable/Material.h>
#include <Framework/Utils/Common.h>
#include <cassert>

namespace CubbyFlow {
namespace CubbyRender {

    InputLayout::InputLayout() 
    {
        //! Do nothings.
    }
    
    InputLayout::~InputLayout() 
    {
        //! Do nothings.
    }

    VertexFormat InputLayout::getDrawFormat() const
    {
        return _drawFormat;
    }

    void InputLayout::attachVertexBuffer(RendererPtr renderer, MaterialPtr material, VertexBufferPtr vertexBuffer)
    {
        for (const auto& buffer : _vertexBuffers)
        {
            if (static_cast<int>(vertexBuffer->getInputVertexFormat() & buffer->getInputVertexFormat()))
            {
                CUBBYFLOW_ERROR << "Input vertex buffer's format is overlapped with already exist ones.";
                abort();
            }
            if (buffer->getNumberOfVertices() != vertexBuffer->getNumberOfVertices())
            {
                CUBBYFLOW_ERROR << "The number of vertices of given buffer and already exist one must matched.";
                abort();
            }
        }

        this->bind(renderer);
        vertexBuffer->bindState(renderer, material);
        _drawFormat |= vertexBuffer->getInputVertexFormat();
        _vertexBuffers.push_back(vertexBuffer);
        this->unbind(renderer);
    }

    void InputLayout::attachIndexBuffer(RendererPtr renderer, IndexBufferPtr indexBuffer)
    {
        this->bind(renderer);
        indexBuffer->bindState(renderer);
        _indexBuffer = indexBuffer;
        this->unbind(renderer);
    }

    void InputLayout::updateVertexBuffer(RendererPtr renderer, const ConstArrayAccessor1<float>& data, VertexFormat format)
    {
        VertexBufferPtr matchedBuffer = nullptr;
        for (auto buffer : _vertexBuffers)
        {
            if (static_cast<int>(format & buffer->getInputVertexFormat()))
            {
                matchedBuffer = buffer;
                break;
            }
        }

        if (matchedBuffer)
        {
            matchedBuffer->allocateBuffer(renderer, data, static_cast<size_t>(data.size() / VertexHelper::getNumberOfFloats(format)));
        }
        else
        {
            CUBBYFLOW_ERROR << "Could not find vertex buffer matched with given vertex format";
            abort();
        }
    }

    void InputLayout::updateIndexBuffer(RendererPtr renderer, const ConstArrayAccessor1<unsigned int>& data)
    {
        if (_indexBuffer)
        {
            _indexBuffer->updateBuffer(renderer, data);
        }
        else
        {
            CUBBYFLOW_ERROR << "Could not find any attached index buffer";
            abort();
        }
    }
    
    bool InputLayout::isIndexBufferAttached() const
    {
        return _indexBuffer != nullptr;
    }

    void InputLayout::bind(RendererPtr renderer)
    {
        onBind(renderer);
    }

    void InputLayout::unbind(RendererPtr renderer)
    {
        onUnbind(renderer);
    }

    void InputLayout::release()
    {
        onRelease();
    }

    size_t InputLayout::getNumberOfVertices() const
    {
        return _vertexBuffers.size() == size_t(0) ? size_t(0) : _vertexBuffers.front()->getNumberOfVertices();
    }

    size_t InputLayout::getNumberOfIndices() const
    {
        return _indexBuffer == nullptr ? size_t(0) : _indexBuffer->getNumberOfIndices();
    }


} 
}
