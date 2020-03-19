/*************************************************************************
> File Name: GL3VertexBuffer.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: VertexBuffer class implemented by modern opengl
> Created Time: 2020/02/22
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#ifdef CUBBYFLOW_USE_GL

#include <Framework/GL3/GL3VertexBuffer.h>
#include <Framework/GL3/GL3Renderer.h>
#include <Framework/GL3/GL3Common.h>
#include <Framework/GL3/GL3Shader.h>
#include <Framework/Material.h>
#include <Framework/Renderer.h>
#include <Framework/Common.h>
#include <glad/glad.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    GL3VertexBuffer::GL3VertexBuffer()
    {
        //! Do nothing
    }

    GL3VertexBuffer::GL3VertexBuffer(size_t numberOfElements, VertexFormat format)
        : VertexBuffer(numberOfElements, format)
    {
        //! Do nothing
    }

    GL3VertexBuffer::~GL3VertexBuffer()
    {
        if (static_cast<int>(_vertexArrayID))
            glDeleteVertexArrays(1, &_vertexArrayID);
    }

    void GL3VertexBuffer::onBind(RendererPtr renderer)
    {
        UNUSED_VARIABLE(renderer);
        glBindVertexArray(_vertexArrayID);
    }
        
    void GL3VertexBuffer::onUnbind(RendererPtr renderer) 
    {
        UNUSED_VARIABLE(renderer);
        glBindVertexArray(static_cast<GLuint>(0));
    }

    void GL3VertexBuffer::onDestroy(RendererPtr renderer) 
    {
        UNUSED_VARIABLE(renderer);
        if (static_cast<int>(_vertexArrayID))
            glDeleteVertexArrays(1, &_vertexArrayID);
        if (_data) 
            delete[] _data;
    }

    void GL3VertexBuffer::onAllocateResource(RendererPtr renderer, MaterialPtr material, const float* data, bool storeData) 
    {   
        if (storeData) 
            _data = data;

        GL3Shader* shader = dynamic_cast<GL3Shader*>(material->getShader().get());
        assert(shader != nullptr);

        VertexFormat vertexFormat = shader->getInputVertexFormat();

        shader->bind(renderer);
        glGenVertexArrays(1, &_vertexArrayID);
        
        if (_vertexArrayID)
        {
            bind(renderer);

            GLsizei stride = static_cast<GLsizei>(VertexHelper::getSizeInBytes(vertexFormat));   

            glGenBuffers(1, &_vertexBufferID);
            glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
            glBufferData(GL_ARRAY_BUFFER, _numberOfElements * stride, data, GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            GLsizeiptr offset = 0;

            if (static_cast<int>(_format & VertexFormat::Position3))
            {
                GLuint attribLoc = shader->getAttribLocation("position");
                GLint numberOfFloats = static_cast<GLint>(VertexHelper::getNumberOfFloats(VertexFormat::Position3));
                glVertexAttribPointer(attribLoc, static_cast<GLint>(numberOfFloats), GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offset));
                glEnableVertexAttribArray(attribLoc);
                offset += sizeof(float) * numberOfFloats;
            }

            if (static_cast<int>(_format & VertexFormat::Normal3))
            {
                GLuint attribLoc = shader->getAttribLocation("normal");
                GLint numberOfFloats = static_cast<GLint>(VertexHelper::getNumberOfFloats(VertexFormat::Normal3));
                glVertexAttribPointer(attribLoc, static_cast<GLint>(numberOfFloats), GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offset));
                glEnableVertexAttribArray(attribLoc);
                offset += sizeof(float) * numberOfFloats;
            }

            if (static_cast<int>(_format & VertexFormat::TexCoord2))
            {
                GLuint attribLoc = shader->getAttribLocation("texCoord");
                GLint numberOfFloats = static_cast<GLint>(VertexHelper::getNumberOfFloats(VertexFormat::TexCoord2));
                glVertexAttribPointer(attribLoc, static_cast<GLint>(numberOfFloats), GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offset));
                glEnableVertexAttribArray(attribLoc);
                offset += sizeof(float) * numberOfFloats;
            }

            if (static_cast<int>(_format & VertexFormat::TexCoord3))
            {
                GLuint attribLoc = shader->getAttribLocation("texCoord");
                GLint numberOfFloats = static_cast<GLint>(VertexHelper::getNumberOfFloats(VertexFormat::TexCoord3));
                glVertexAttribPointer(attribLoc, static_cast<GLint>(numberOfFloats), GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offset));
                glEnableVertexAttribArray(attribLoc);
                offset += sizeof(float) * numberOfFloats;
            }

            if (static_cast<int>(_format & VertexFormat::Color4))
            {
                GLuint attribLoc = shader->getAttribLocation("color");
                GLint numberOfFloats = static_cast<GLint>(VertexHelper::getNumberOfFloats(VertexFormat::Color4));
                glVertexAttribPointer(attribLoc, static_cast<GLint>(numberOfFloats), GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offset));
                glEnableVertexAttribArray(attribLoc);
            }
        }

        this->unbind(renderer);
        shader->unbind(renderer);
    }
    void GL3VertexBuffer::onUpdateResource(RendererPtr renderer, MaterialPtr material, const float* data, bool storeData)
    {
        //! Do nothing
        UNUSED_VARIABLE(renderer);
        UNUSED_VARIABLE(material);
        UNUSED_VARIABLE(data);
        UNUSED_VARIABLE(storeData);
    }

} 
}

#endif