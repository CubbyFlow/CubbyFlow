/*************************************************************************
> File Name: SimulationBuffer.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The buffer class which store buffer desc and own raw pointer optionally.
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/RoundRobinAsyncBuffer.hpp>
#include <Vox/FrameContext.hpp>
#include <Vox/ParticleLoader.hpp>
#include <Vox/Scene.hpp>
#include <Core/Utils/Logging.h>
#include <glad/glad.h>

using namespace CubbyFlow;

namespace Vox {

    RoundRobinAsyncBuffer::RoundRobinAsyncBuffer()
        : RoundRobinAsyncBuffer(RoundRobinAsyncBuffer::kDefaultNumBuffer)
    {
    }

    RoundRobinAsyncBuffer::RoundRobinAsyncBuffer(const size_t numBuffer)
    {
        _vaos.resize(numBuffer);
        _buffers.resize(numBuffer);
        _fences.resize(numBuffer);

        glGenVertexArrays(numBuffer, _vaos.data());
        glGenBuffers(numBuffer, _buffers.data());
        for (size_t i = 0; i < numBuffer; ++i)
        {
            glBindVertexArray(_vaos[i]);
            glBindBuffer(GL_ARRAY_BUFFER, _buffers[i]);
            glBufferData(GL_ARRAY_BUFFER, RoundRobinAsyncBuffer::kMaxBufferSize, nullptr, GL_STREAM_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
    }

    RoundRobinAsyncBuffer::~RoundRobinAsyncBuffer()
    {
        glDeleteVertexArrays(static_cast<GLsizei>(_vaos.size()), _vaos.data());
        glDeleteBuffers(static_cast<GLsizei>(_buffers.size()), _buffers.data());
        _fences.clear();
    }

    bool RoundRobinAsyncBuffer::CheckFence(GLuint64 timeout) 
    {
        const size_t bufferNum = _frameIndex % _buffers.size();
        const GLsync& fence = _fences[bufferNum];

        if (fence)
        {
            GLenum result = glClientWaitSync(fence, 0, timeout); //! CPU side synchronization
            return result == GL_ALREADY_SIGNALED || result == GL_CONDITION_SATISFIED;
        }
        return true;
    }

    void RoundRobinAsyncBuffer::AsyncBufferTransfer(const std::shared_ptr<Scene>& scn)
    {
        const size_t bufferNum = _frameIndex % _buffers.size();
        const GLuint buffer = _buffers[bufferNum];
        const GLsync& fence = _fences[bufferNum];
        
        //! If fence exists, delete.
        if (fence) glDeleteSync(fence);
        //! Bind the vertex buffer.
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        //! Take modulo for repeating buffer transfer.
        const size_t index = _frameIndex % scn->GetLoader()->GetNumberOfFrame();
        size_t numBytes = scn->GetLoader()->GetNumberOfBytes(index);
        //! Map gpu pointer to cpu.
        void* ptr = glMapBufferRange(GL_ARRAY_BUFFER, 0, numBytes, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
        //! Pass pointer for the memcpy
        scn->GetLoader()->CopyParticleData(ptr, index);
        //! Unmap the pointer after transfer finished.
        glUnmapBuffer(GL_ARRAY_BUFFER);
        
        _numVertices  = numBytes / (sizeof(float) * 3);
    }

    void RoundRobinAsyncBuffer::DrawFrame(const std::shared_ptr<FrameContext>& ctx)
    {
        const size_t bufferNum = _frameIndex % _buffers.size();
        GLsync& fence = _fences[bufferNum];

        GLenum primitive = ctx->GetRenderMode();
        glBindVertexArray(_vaos[bufferNum]);
        glDrawArrays(primitive, 0, _numVertices);
        glBindVertexArray(0);
        //glDrawArraysInstanced(primitive, 0, _numVertices / (sizeof(float) * 3), 0);
        
        //! Make fence to draw call.
        fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0); 
    }

    void RoundRobinAsyncBuffer::AdvanceFrame()
    {
        ++_frameIndex; //! Advance frame index;
    }

};