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

} 
}
