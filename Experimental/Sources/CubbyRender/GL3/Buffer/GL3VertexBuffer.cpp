/*************************************************************************
> File Name: GL3VertexBuffer.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: VertexBuffer class implemented by modern opengl
> Created Time: 2020/02/22
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#include <Framework/Renderable/Material.h>
#include <Framework/Buffer/InputLayout.h>
#include <Framework/Renderer/Renderer.h>
#include <Framework/Utils/Common.h>

#ifdef CUBBYFLOW_USE_GL

#include <GL3/Buffer/GL3VertexBuffer.h>
#include <GL3/Renderer/GL3Renderer.h>
#include <GL3/Utils/GL3Common.h>
#include <GL3/Shader/GL3Shader.h>
#include <glad/glad.h>

#include <GL3/Utils/GL3Debugging.h>
#include <iostream>

namespace CubbyFlow {
namespace CubbyRender {
    
    GL3VertexBuffer::GL3VertexBuffer()
    {
        //! Do nothing
    }

    GL3VertexBuffer::~GL3VertexBuffer()
    {
        destroy();
    }

    void GL3VertexBuffer::onBind(RendererPtr renderer)
    {
        UNUSED_VARIABLE(renderer);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
    }
        
    void GL3VertexBuffer::onUnbind(RendererPtr renderer) 
    {
        UNUSED_VARIABLE(renderer);
        glBindBuffer(GL_ARRAY_BUFFER, 0U);
    }

    void GL3VertexBuffer::onDestroy() 
    {
        if (static_cast<int>(_vertexBufferID))
            glDeleteBuffers(1, &_vertexBufferID);
    }

    void GL3VertexBuffer::updateBuffer(RendererPtr renderer, const ConstArrayAccessor1<float>& data)
    {
        GLsizei stride = static_cast<GLsizei>(VertexHelper::getSizeInBytes(_vertexFormat));   
        bind(renderer);
        glBufferSubData(GL_ARRAY_BUFFER, static_cast<GLintptr>(0), _numberOfVertices * stride, data.data());
        unbind(renderer);
    }

    void GL3VertexBuffer::onAllocateBuffer(RendererPtr renderer, const ConstArrayAccessor1<float>& data) 
    {   
        GLsizei stride = static_cast<GLsizei>(VertexHelper::getSizeInBytes(_vertexFormat));   
        glGenBuffers(1, &_vertexBufferID);
        bind(renderer);
        glBufferData(GL_ARRAY_BUFFER, _numberOfVertices * stride, data.data(), GL_DYNAMIC_DRAW);
        unbind(renderer);
    }

    void GL3VertexBuffer::onBindState(RendererPtr renderer, MaterialPtr material) 
    { 
        GL3ShaderPtr shader = std::dynamic_pointer_cast<GL3Shader>(material->getShader());
        if (shader == nullptr)
            abort();

        shader->bind(renderer); // bind shader to the renderer..
        bind(renderer); // bind vertex buffer to the renderer.

        // get the bytes size of the vertex format.
        // ex. in case of position3normal3, there is six floats.
        // therefore stride = 6 * 4 = 24
        GLsizei stride = static_cast<GLsizei>(VertexHelper::getSizeInBytes(_vertexFormat));   
        GLsizeiptr offset = 0;
        
        std::cout << "vertexformat : " << static_cast<int>(_vertexFormat) << std::endl;
        std::cout << "stride : " << stride << std::endl;
        
        if (static_cast<int>(_vertexFormat & VertexFormat::Position3))
        {
            GLint attribLoc = shader->getAttribLocation("position");
            GLint numberOfFloats = static_cast<GLint>(VertexHelper::getNumberOfFloats(VertexFormat::Position3));
            std::cout << numberOfFloats << std::endl;
            glVertexAttribPointer(static_cast<GLuint>(attribLoc), static_cast<GLint>(numberOfFloats), GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offset));
            glEnableVertexAttribArray(attribLoc);
            offset += sizeof(float) * numberOfFloats;
        }
        if (static_cast<int>(_vertexFormat & VertexFormat::Normal3))
        {
            GLint attribLoc = shader->getAttribLocation("normal");
            GLint numberOfFloats = static_cast<GLint>(VertexHelper::getNumberOfFloats(VertexFormat::Normal3));
            glVertexAttribPointer(static_cast<GLuint>(attribLoc), static_cast<GLint>(numberOfFloats), GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offset));
            glEnableVertexAttribArray(attribLoc);
            offset += sizeof(float) * numberOfFloats;
        }       
        
        if (static_cast<int>(_vertexFormat & VertexFormat::TexCoord2))
        {
            GLint attribLoc = shader->getAttribLocation("texCoord");
            GLint numberOfFloats = static_cast<GLint>(VertexHelper::getNumberOfFloats(VertexFormat::TexCoord2));
            glVertexAttribPointer(static_cast<GLuint>(attribLoc), static_cast<GLint>(numberOfFloats), GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offset));
            glEnableVertexAttribArray(attribLoc);
            offset += sizeof(float) * numberOfFloats;
        }
        
        if (static_cast<int>(_vertexFormat & VertexFormat::TexCoord3))
        {
            GLint attribLoc = shader->getAttribLocation("texCoord");
            GLint numberOfFloats = static_cast<GLint>(VertexHelper::getNumberOfFloats(VertexFormat::TexCoord3));
            glVertexAttribPointer(static_cast<GLuint>(attribLoc), static_cast<GLint>(numberOfFloats), GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offset));
            glEnableVertexAttribArray(attribLoc);
            offset += sizeof(float) * numberOfFloats;
        }
        
        std::cout << "offset : " << offset << std::endl;
        if (static_cast<int>(_vertexFormat & VertexFormat::Color4))
        {
            GLint attribLoc = shader->getAttribLocation("color");
        CUBBYFLOW_CHECK_GLERROR();
            GLint numberOfFloats = static_cast<GLint>(VertexHelper::getNumberOfFloats(VertexFormat::Color4));
        CUBBYFLOW_CHECK_GLERROR();
        std::cout << attribLoc << std::endl;
            glVertexAttribPointer(static_cast<GLuint>(attribLoc), static_cast<GLint>(numberOfFloats), GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offset));
        CUBBYFLOW_CHECK_GLERROR();
            glEnableVertexAttribArray(attribLoc);
        CUBBYFLOW_CHECK_GLERROR();
        }

        unbind(renderer); // unbind vertex buffer from the renderer.
        shader->unbind(renderer); // unbind shader from the renderer.
    }
} 
}

#endif
