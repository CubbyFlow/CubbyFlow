/*************************************************************************
> File Name: ParticleBuffer.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The buffer class which store buffer desc and own raw pointer optionally.
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/ParticleBuffer.hpp>
#include <Vox/FrameContext.hpp>
#include <Vox/GeometryCache.hpp>
#include <Vox/GeometryCacheManager.hpp>
#include <glad/glad.h>
#include <cstring>

namespace Vox {

    ParticleBuffer::ParticleBuffer()
        : ParticleBuffer(RoundRobinAsyncBuffer::kDefaultNumBuffer)
    {

    }

    ParticleBuffer::ParticleBuffer(const size_t numBuffer)
        : RoundRobinAsyncBuffer(numBuffer)
    {
        _vbos.resize(numBuffer);
        glGenBuffers(numBuffer, _vbos.data());
        for (size_t i = 0; i < numBuffer; ++i)
        {
            glBindVertexArray(_vaos[i]);
            glBindBuffer(GL_ARRAY_BUFFER, _vbos[i]);
            glBufferData(GL_ARRAY_BUFFER, RoundRobinAsyncBuffer::kMaxBufferSize, nullptr, GL_STREAM_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
            glBindVertexArray(0);
        }
    }

    ParticleBuffer::~ParticleBuffer()
    {
        glDeleteBuffers(static_cast<GLsizei>(_numBuffer), _vbos.data());
    }

    void ParticleBuffer::OnAsyncBufferTransfer(const std::shared_ptr<GeometryCacheManager>& cacheManager)
    {
        const size_t bufferNum = _frameIndex % _numBuffer;
        const GLuint buffer = _vbos[bufferNum];
        //! Bind the vertex buffer.
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        //! Take Modulo with geometry cache size for repeating transfer.
        const size_t index = _frameIndex % cacheManager->GetNumberOfCache();
        const auto& shape = cacheManager->GetGeometryCache(index)->GetShape(0);
        const size_t numBytes = shape.vertices.size() * sizeof(float);
        //! Map gpu pointer to cpu.
        void* ptr = glMapBufferRange(GL_ARRAY_BUFFER, 0, numBytes, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
        //! Pass pointer for the memcpy
        std::memcpy(ptr, static_cast<const void*>(shape.vertices.data()), numBytes);
        //! Unmap the pointer after transfer finished.
        glUnmapBuffer(GL_ARRAY_BUFFER);
        _numVertices  = numBytes / (sizeof(float) * 3);
    }

    void ParticleBuffer::OnDrawFrame(const std::shared_ptr<FrameContext>& ctx)
    {
        const size_t bufferNum = _frameIndex % _numBuffer;

        GLenum primitive = ctx->GetRenderMode();
        glBindVertexArray(_vaos[bufferNum]);
        glDrawArrays(primitive, 0, _numVertices);
        glBindVertexArray(0);
    }

};