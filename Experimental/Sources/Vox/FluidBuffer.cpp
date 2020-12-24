/*************************************************************************
> File Name: FluidBuffer.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The buffer class which store buffer desc and own raw pointer optionally.
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/FluidBuffer.hpp>
#include <Vox/FrameContext.hpp>
#include <Vox/GeometryCacheManager.hpp>
#include <Vox/GeometryCache.hpp>
#include <Vox/Vertex.hpp>
#include <Vox/VoxScene.hpp>
#include <Core/Utils/Logging.hpp>
#include <glad/glad.h>

using namespace CubbyFlow;

namespace Vox {

    FluidBuffer::FluidBuffer(const size_t numBuffer)
    {
        Resize(numBuffer);
    }   

    FluidBuffer::~FluidBuffer()
    {
        _fences.clear();
    }

    void FluidBuffer::Resize(const size_t numBuffer)
    {
        if (_numBuffer <= numBuffer) //! In case new number of the buffer is larger than original.
        {
            _vaos.resize(numBuffer);
            _vbos.resize(numBuffer);
            _ebos.resize(numBuffer);
            const GLsizei numCreate = static_cast<GLsizei>(numBuffer - _numBuffer);
            glGenVertexArrays(numCreate, &_vaos[_numBuffer]);
            glGenBuffers(numCreate, &_vbos[_numBuffer]);
            glGenBuffers(numCreate, &_ebos[_numBuffer]);
            for (size_t i = _numBuffer; i < numBuffer; ++i)
            {
                glBindVertexArray(_vaos[i]);
                glBindBuffer(GL_ARRAY_BUFFER, _vbos[i]);
                glBufferData(GL_ARRAY_BUFFER, FluidBuffer::kMaxBufferSize, nullptr, GL_STREAM_DRAW);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebos[i]);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, FluidBuffer::kMaxBufferSize, nullptr, GL_STREAM_DRAW);
            }
            glBindVertexArray(0);
        }
        else //! In case, new number of the buffer is less than original. we need to destroy
        {
            const GLsizei numDelete = static_cast<GLsizei>(_numBuffer - numBuffer);
            glDeleteVertexArrays(numDelete, &_vaos[numBuffer]);
            glDeleteBuffers(numDelete, &_vbos[numBuffer]);
            glDeleteBuffers(numDelete, &_ebos[numBuffer]);
            _vaos.resize(numBuffer);
            _vbos.resize(numBuffer);
            _ebos.resize(numBuffer);
        }
        _numBuffer = numBuffer;
        _fences.resize(_numBuffer);
    }

    bool FluidBuffer::CheckFence(GLuint64 timeout) 
    {
        const size_t bufferNum = _frameIndex % _numBuffer;
        const GLsync& fence = _fences[bufferNum];

        if (fence)
        {
            GLenum result = glClientWaitSync(fence, 0, timeout); //! CPU side synchronization
            return result == GL_ALREADY_SIGNALED || result == GL_CONDITION_SATISFIED;
        }
        return true;
    }

    void FluidBuffer::AsyncBufferTransfer(const std::shared_ptr<GeometryCacheManager>& cacheManager)
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

        _numElements = shape.indices.size();
    }

    void FluidBuffer::DrawFrame(const std::shared_ptr<FrameContext>& ctx)
    {
        const size_t bufferNum = _frameIndex % _numBuffer;

        glBindVertexArray(_vaos[bufferNum]);
        glDrawElements(ctx->GetRenderStatus().primitive, static_cast<GLsizei>(_numElements), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        //! Make fence to draw call.
        _fences[bufferNum] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0); 
    }

    void FluidBuffer::AdvanceFrame()
    {
        ++_frameIndex; //! Advance frame index;
    }

};