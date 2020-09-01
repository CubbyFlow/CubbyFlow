/*************************************************************************
> File Name: FluidMeshBuffer.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The buffer class which store buffer desc and own raw pointer optionally.
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/FluidMeshBuffer.hpp>
#include <Vox/FrameContext.hpp>
#include <Vox/GeometryCache.hpp>
#include <Vox/GeometryCacheManager.hpp>
#include <glad/glad.h>
#include <cstring>

namespace Vox {

    FluidMeshBuffer::FluidMeshBuffer()
        : FluidMeshBuffer(RoundRobinAsyncBuffer::kDefaultNumBuffer)
    {
        //! Do nothing.
    }

    FluidMeshBuffer::FluidMeshBuffer(const size_t numBuffer)
        : RoundRobinAsyncBuffer(numBuffer)
    {
        _vaos.resize(numBuffer);
        _vbos.resize(numBuffer);
        _ebos.resize(numBuffer);
        glGenVertexArrays(numBuffer, _vaos.data());
        glGenBuffers(numBuffer, _vbos.data());
        glGenBuffers(numBuffer, _ebos.data());
        for (size_t i = 0; i < numBuffer; ++i)
        {
            glBindVertexArray(_vaos[i]);
            glBindBuffer(GL_ARRAY_BUFFER, _vbos[i]);
            glBufferData(GL_ARRAY_BUFFER, RoundRobinAsyncBuffer::kMaxBufferSize, nullptr, GL_STREAM_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebos[i]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, RoundRobinAsyncBuffer::kMaxBufferSize, nullptr, GL_STREAM_DRAW);
        }
        glBindVertexArray(0);
    }

    FluidMeshBuffer::~FluidMeshBuffer()
    {
        glDeleteVertexArrays(static_cast<GLsizei>(_numBuffer), _vaos.data());
        glDeleteBuffers(static_cast<GLsizei>(_numBuffer), _vbos.data());
        glDeleteBuffers(static_cast<GLsizei>(_numBuffer), _ebos.data());
    }

    void FluidMeshBuffer::OnAsyncBufferTransfer(const std::shared_ptr<GeometryCacheManager>& cacheManager)
    {
        const size_t bufferNum = _frameIndex % _numBuffer;
        const GLuint vbo = _vbos[bufferNum];
        const GLuint ebo = _ebos[bufferNum];

        //! Take Modulo with geometry cache size for repeating transfer.
        const size_t index = _frameIndex % cacheManager->GetNumberOfCache();
        const auto& shape = cacheManager->GetGeometryCache(index)->GetShape(0);
        size_t numBytes = 0;

        //! Bind the vertex buffer.
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        //! Map gpu pointer to cpu.
        numBytes = shape.interleaved.size() * sizeof(float);
        void* verticesPtr = glMapBufferRange(GL_ARRAY_BUFFER, 0, numBytes, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
        //! Pass pointer for the memcpy
        std::memcpy(verticesPtr, static_cast<const void*>(shape.interleaved.data()), numBytes);
        //! Unmap the pointer after transfer finished.
        glUnmapBuffer(GL_ARRAY_BUFFER);

        //! Bind the element array buffer.
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        //! Map gpu pointer to cpu.
        numBytes = shape.indices.size() * sizeof(unsigned int);
        void* elementsPtr = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, numBytes, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
        //! Pass pointer for the memcpy
        std::memcpy(elementsPtr, static_cast<const void*>(shape.indices.data()), numBytes);
        //! Unmap the pointer after transfer finished.
        glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

        _numIndices = shape.indices.size();
    }

    void FluidMeshBuffer::OnDrawFrame(const std::shared_ptr<FrameContext>& ctx)
    {
        const size_t bufferNum = _frameIndex % _numBuffer;
        
        glBindVertexArray(_vaos[bufferNum]);
        glDrawElements(ctx->GetRenderStatus().primitive, _numIndices, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

};