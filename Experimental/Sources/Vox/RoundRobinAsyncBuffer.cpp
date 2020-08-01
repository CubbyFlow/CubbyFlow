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
        _buffers.resize(numBuffer);
        _fences.resize(numBuffer);

        glGenBuffers(numBuffer, _buffers.data());
        for (GLuint id : _buffers)
        {
            glBindBuffer(GL_ARRAY_BUFFER, id);
            glBufferData(GL_ARRAY_BUFFER, RoundRobinAsyncBuffer::kMaxBufferSize, nullptr, GL_STREAM_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }

    RoundRobinAsyncBuffer::~RoundRobinAsyncBuffer()
    {
        for (GLuint& id : _buffers)
            glDeleteBuffers(1, &id);
        _buffers.clear();
        _fences.clear();
    }

    bool RoundRobinAsyncBuffer::CheckFence(GLuint64 timeout) 
    {
        const size_t bufferNum = _frameIndex % _buffers.size();
        const GLsync& fence = _fences[bufferNum];

        if (fence)
        {
            GLenum result = glClientWaitSync(fence, 0, timeout); //! CPU side synchronization
            if (result == GL_ALREADY_SIGNALED || result == GL_CONDITION_SATISFIED)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        return true;
    }

    void RoundRobinAsyncBuffer::AsyncBufferTransfer(const std::shared_ptr<Scene>& scn)
    {
        const size_t bufferNum = _frameIndex % _buffers.size();
        const GLuint buffer = _buffers[bufferNum];

        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        const size_t index = _frameIndex % scn->GetLoader()->GetNumberOfFrame();
        _numVertices = scn->GetLoader()->GetNumberOfBytes(index);
        void* ptr = glMapBufferRange(GL_ARRAY_BUFFER, 0, _numVertices, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
        scn->GetLoader()->CopyParticleData(ptr, index);
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    void RoundRobinAsyncBuffer::DrawFrame(const std::shared_ptr<FrameContext>& ctx)
    {
        const size_t bufferNum = _frameIndex % _buffers.size();
        GLsync& fence = _fences[bufferNum];

        GLenum primitive = ctx->GetRenderMode();
        glDrawArraysInstanced(primitive, 0, _numVertices / (sizeof(float) * 3), 0);
        
        if (fence) glDeleteSync(fence);
        fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0); 
    }

    void RoundRobinAsyncBuffer::AdvanceFrame()
    {
        ++_frameIndex; //! Advance frame index;
    }

};