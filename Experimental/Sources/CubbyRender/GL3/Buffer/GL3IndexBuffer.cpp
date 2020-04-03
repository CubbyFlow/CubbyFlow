/*************************************************************************
> File Name: GL3IndexBuffer.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: VertexBuffer class implemented by modern opengl
> Created Time: 2020/02/22
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#ifdef CUBBYFLOW_USE_GL

#include <GL3/Buffer/GL3IndexBuffer.h>
#include <GL3/Renderer/GL3Renderer.h>
#include <GL3/Utils/GL3Common.h>
#include <GL3/Shader/GL3Shader.h>
#include <Framework/Renderable/Material.h>
#include <Framework/Renderer/Renderer.h>
#include <Framework/Utils/Common.h>
#include <glad/glad.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    GL3IndexBuffer::GL3IndexBuffer()
    {
        //! Do nothing
    }

    GL3IndexBuffer::~GL3IndexBuffer()
    {
        destroy();
    }

    void GL3IndexBuffer::onBind(RendererPtr renderer)
    {
        UNUSED_VARIABLE(renderer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferID);
    }
        
    void GL3IndexBuffer::onUnbind(RendererPtr renderer) 
    {
        UNUSED_VARIABLE(renderer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLuint>(0));
    }

    void GL3IndexBuffer::onDestroy() 
    {
        //!if (static_cast<int>(_indexBufferID))
        //!    glDeleteBuffers(1, &_indexBufferID);
    }

    void GL3IndexBuffer::updateBuffer(RendererPtr renderer, const ConstArrayAccessor1<unsigned int>& data)
    {
        bind(renderer);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLintptr>(0), _numberOfIndices, data.data());
        unbind(renderer);
    }

    void GL3IndexBuffer::onAllocateBuffer(RendererPtr renderer, const ConstArrayAccessor1<unsigned int>& data) 
    {   
        glGenBuffers(1, &_indexBufferID);
        bind(renderer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(_numberOfIndices), data.data(), GL_DYNAMIC_DRAW);
        unbind(renderer);
    }
    
    void GL3IndexBuffer::onBindState(RendererPtr renderer)
    {
        bind(renderer);
    }
} 
}

#endif