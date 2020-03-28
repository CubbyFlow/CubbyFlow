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

#include <Framework/GL3/GL3IndexBuffer.h>
#include <Framework/GL3/GL3Renderer.h>
#include <Framework/GL3/GL3Common.h>
#include <Framework/GL3/GL3Shader.h>
#include <Framework/Material.h>
#include <Framework/Renderer.h>
#include <Framework/Common.h>
#include <GL/glew.h>

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
        if (static_cast<int>(_indexBufferID))
            glDeleteBuffers(1, &_indexBufferID);
    }

    void GL3IndexBuffer::updateBuffer(RendererPtr renderer, MaterialPtr material, const ConstArrayAccessor1<unsigned int>& data, bool storeData)
    {
        UNUSED_VARIABLE(material);
        if (storeData)
        {
            data.ParallelForEachIndex([&](size_t i){
                _data[i] = data[i];
            });
        }

        bind(renderer);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLintptr>(0), _numberOfIndices, data.data());
        unbind(renderer);
    }

    void GL3IndexBuffer::onAllocateBuffer(RendererPtr renderer, MaterialPtr material, const ConstArrayAccessor1<unsigned int>& data) 
    {   
        UNUSED_VARIABLE(material),
        glGenBuffers(1, &_indexBufferID);
        bind(renderer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(_numberOfIndices), data.data(), GL_DYNAMIC_DRAW);
        unbind(renderer);
    }
} 
}

#endif