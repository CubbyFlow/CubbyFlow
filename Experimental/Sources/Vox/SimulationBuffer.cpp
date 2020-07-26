/*************************************************************************
> File Name: SimulationBuffer.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The buffer class which store buffer desc and own raw pointer optionally.
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/SimulationBuffer.hpp>
#include <Vox/ParticleLoader.hpp>
#include <Vox/FrameContext.hpp>
#include <Core/Utils/Logging.h>
#include <glad/glad.h>

using namespace CubbyFlow;

namespace Vox {

    SimulationBuffer::SimulationBuffer()
    {

    }

    SimulationBuffer::SimulationBuffer(const size_t numBuffer)
    {
        _buffers.resize(numBuffer);
        _fences.resize(numBuffer);

        glGenBuffers(numBuffer, _buffers.data());
        for (GLuint id : _buffers)
        {
            glBindBuffer(GL_ARRAY_BUFFER, id);
            glBufferData(GL_ARRAY_BUFFER, SimulationBuffer::kMaxBufferSize, nullptr, GL_STREAM_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }

    SimulationBuffer::~SimulationBuffer()
    {
        for (GLuint& id : _buffers)
            glDeleteBuffers(1, &id);
        _buffers.clear();
        _fences.clear();
    }

    void SimulationBuffer::DrawFrame(const std::shared_ptr<FrameContext>& ctx)
    {
        const size_t bufferNum = _frameIndex % _buffers.size();
        const GLuint buffer = _buffers[bufferNum];
        GLsync& fence = _fences[bufferNum];

        if (fence)
        {
            GLenum result = glClientWaitSync(fence, 0, 50000); //! CPU side synchronization
            if (result != GL_ALREADY_SIGNALED && result != GL_CONDITION_SATISFIED)
            {
                CUBBYFLOW_ERROR << "Simulation Buffer Synchronize with Fence(" << bufferNum << ") Failed";
                return;
            }
        }

        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        const size_t size = ctx->GetLoader()->GetNumberOfBytes(_frameIndex);
        void* ptr = glMapBufferRange(GL_ARRAY_BUFFER, 0, size, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
        ctx->GetLoader()->CopyParticleData(ptr, _frameIndex);
        glUnmapBuffer(GL_ARRAY_BUFFER);

        glDrawArraysInstanced(GL_POINTS, 0, size / (sizeof(float) * 3), 0);
        
        if (fence) glDeleteSync(fence);
        fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0); 
    }

    void SimulationBuffer::AdvanceFrame()
    {
        ++_frameIndex;
    }
};