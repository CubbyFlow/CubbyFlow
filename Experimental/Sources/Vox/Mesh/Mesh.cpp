/*************************************************************************
> File Name: Mesh.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: OpenGL Mesh class for sharing resourches which have same vertices data.
> Created Time: 2020/08/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Mesh/Mesh.hpp>
#include <Vox/Core/FrameContext.hpp>
#include <Vox/Core/Material.hpp>
#include <glad/glad.h>

namespace Vox {

    Mesh::Mesh()
        : _usage(GL_STATIC_DRAW), _vao(0), _vbo(0), _ebo(0), _numVertices(0)
    {
        //! Do nothing.
    }

    Mesh::~Mesh()
    {
        ClearMeshObject();
    }

    void Mesh::SetBufferUsage(GLuint usage)
    {
        _usage = usage;
    }

    void Mesh::GenerateMeshObject(const MeshShape& shape, VertexFormat format, bool bInterleaved)
    {
        //! Generate opengl resources.
        glGenVertexArrays(1, &_vao);
        glGenBuffers(1, &_vbo);
        glGenBuffers(1, &_ebo);
        //! Bind input layout vertex array.
        glBindVertexArray(_vao);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        GLuint index = 0;
        size_t offset = 0;
        //! Pass Buffer data and set attributes according to whether if it is interleaved or not. 
        if (bInterleaved)
        {
            glBufferData(GL_ARRAY_BUFFER, shape.interleaved.size() * sizeof(float), shape.interleaved.size() == 0 ? nullptr : shape.interleaved.data(), _usage);
            if (static_cast<int>(format & VertexFormat::Position3))
            {
                glEnableVertexAttribArray(index);
                glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(VertexHelper::GetSizeInBytes(format)), (void*)0);
                offset += 3; index += 1;
            }
            if (static_cast<int>(format & VertexFormat::Normal3))
            {
                glEnableVertexAttribArray(index);
                glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(VertexHelper::GetSizeInBytes(format)), (void*)(offset * sizeof(float)));
                offset += 3; index += 1;
            }
            if (static_cast<int>(format & VertexFormat::TexCoord2))
            {
                glEnableVertexAttribArray(index);
                glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(VertexHelper::GetSizeInBytes(format)), (void*)(offset * sizeof(float)));
            }
        }
        else
        {
            const size_t totalNum = shape.positions.size() * 3 + shape.normals.size() * 3 + shape.texCoords.size() * 2;
            glBufferData(GL_ARRAY_BUFFER, totalNum * sizeof(float), nullptr, GL_STATIC_DRAW);
            if (static_cast<int>(format & VertexFormat::Position3))
            {
                glBufferSubData(GL_ARRAY_BUFFER, 0, shape.positions.size() * sizeof(CubbyFlow::Vector3F), shape.positions.data());
                glEnableVertexAttribArray(index);
                glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(VertexHelper::GetSizeInBytes(VertexFormat::Position3)), (void*)0);
                offset += shape.positions.size() * sizeof(CubbyFlow::Vector3F); index += 1;
            }
            if (static_cast<int>(format & VertexFormat::Normal3))
            {
                glBufferSubData(GL_ARRAY_BUFFER, offset, shape.normals.size() * sizeof(CubbyFlow::Vector3F), shape.normals.data());
                glEnableVertexAttribArray(index);
                glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(VertexHelper::GetSizeInBytes(VertexFormat::Normal3)), (void*)(offset));
                offset += shape.positions.size() * sizeof(CubbyFlow::Vector3F); index += 1;
            }
            if (static_cast<int>(format & VertexFormat::TexCoord2))
            {
                glBufferSubData(GL_ARRAY_BUFFER, offset, shape.texCoords.size() * sizeof(CubbyFlow::Vector3F), shape.texCoords.data());
                glEnableVertexAttribArray(index);
                glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(VertexHelper::GetSizeInBytes(VertexFormat::TexCoord2)), (void*)(offset));
            }
        }
        //! Bind index buffer and pass data.
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indices.size() * sizeof(unsigned int), shape.indices.data(), _usage);
        glBindVertexArray(0);
        //! Set the number of indices of given geometry shape.
        _numVertices = static_cast<unsigned int>(shape.indices.size());
        //! Merge the bounding box.
        _boundingBox.Merge(shape.boundingBox);
    }

    void Mesh::ClearMeshObject()
    {
        if (_vao) glDeleteVertexArrays(1, &_vao);
        if (_vbo) glDeleteBuffers(1, &_vbo);
        if (_ebo) glDeleteBuffers(1, &_ebo);
    }

    void Mesh::DrawMesh(const std::shared_ptr<FrameContext>& ctx)
    {
        glBindVertexArray(_vao);
        glDrawElements(ctx->GetRenderStatus().primitive, _numVertices, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void Mesh::AsyncTransfer(const MeshShape& shape, const size_t numBytes, bool isIndices)
    {
        //! Set the target and the buffer variable.
        const GLenum target = isIndices ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER;
        const GLuint buffer = isIndices ? _ebo : _vbo;

        //! Bind the buffer which will be asynchronously transfered.
        glBindBuffer(target, buffer);
        void* ptr = glMapBufferRange(target, 0, numBytes, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);

        //! Pass pointer for the memcpy
        if (isIndices)
        {
            std::memcpy(ptr, shape.indices.data(), numBytes);
            _numVertices = static_cast<unsigned int>(numBytes / sizeof(unsigned int));
        }
        else
        {
            std::memcpy(ptr, shape.interleaved.data(), numBytes);
        }

        //! Unmap the pointer after transfer finished.
        glUnmapBuffer(target);

        //! Merge the bounding box.
        _boundingBox.Merge(shape.boundingBox);
    }

    void Mesh::GenerateEmptyMesh(VertexFormat format, const size_t verticesBytes, const size_t indicesBytes)
    {
        //! Genetry opengl resources.
        glGenVertexArrays(1, &_vao);
        glGenBuffers(1, &_vbo);
        glGenBuffers(1, &_ebo);
        //! Bind input layout vertex array.
        glBindVertexArray(_vao);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        GLuint index = 0;
        size_t offset = 0;

        //! Pass Buffer data and set attributes according to whether if it is interleaved or not. 
        glBufferData(GL_ARRAY_BUFFER, verticesBytes, nullptr, _usage);
        if (static_cast<int>(format & VertexFormat::Position3))
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(VertexHelper::GetSizeInBytes(format)), (void*)0);
            offset += 3; index += 1;
        }
        if (static_cast<int>(format & VertexFormat::Normal3))
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(VertexHelper::GetSizeInBytes(format)), (void*)(offset * sizeof(float)));
            offset += 3; index += 1;
        }
        if (static_cast<int>(format & VertexFormat::TexCoord2))
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(VertexHelper::GetSizeInBytes(format)), (void*)(offset * sizeof(float)));
        }

        //! Bind index buffer and pass data.
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesBytes, nullptr, _usage);
        glBindVertexArray(0);
    }

    CubbyFlow::BoundingBox3F Mesh::GetBoundingBox() const
    {
        return _boundingBox;
    }
}