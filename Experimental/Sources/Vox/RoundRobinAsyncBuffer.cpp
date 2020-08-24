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
#include <Vox/GeometryCacheManager.hpp>
#include <Vox/Vertex.hpp>
#include <Vox/VoxScene.hpp>
#include <Core/Utils/Logging.hpp>
#include <glad/glad.h>

using namespace CubbyFlow;

namespace Vox {

    RoundRobinAsyncBuffer::RoundRobinAsyncBuffer()
        : RoundRobinAsyncBuffer(RoundRobinAsyncBuffer::kDefaultNumBuffer)
    {
    }

    RoundRobinAsyncBuffer::RoundRobinAsyncBuffer(const size_t numBuffer)
        : _numBuffer(numBuffer)
    {
        _vaos.resize(numBuffer);
        _fences.resize(numBuffer);

        glGenVertexArrays(numBuffer, _vaos.data());
    }

    RoundRobinAsyncBuffer::~RoundRobinAsyncBuffer()
    {
        glDeleteVertexArrays(static_cast<GLsizei>(_vaos.size()), _vaos.data());
        _fences.clear();
    }

    bool RoundRobinAsyncBuffer::CheckFence(GLuint64 timeout) 
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

    void RoundRobinAsyncBuffer::AsyncBufferTransfer(const std::shared_ptr<GeometryCacheManager>& cacheManager)
    {
        const size_t bufferNum = _frameIndex % _numBuffer;
        const GLsync& fence = _fences[bufferNum];
        
        //! If fence exists, delete.
        if (fence) glDeleteSync(fence);
            
        OnAsyncBufferTransfer(cacheManager);
    }

    void RoundRobinAsyncBuffer::DrawFrame(const std::shared_ptr<FrameContext>& ctx)
    {
        const size_t bufferNum = _frameIndex % _numBuffer;
        GLsync& fence = _fences[bufferNum];

        OnDrawFrame(ctx);
        
        //! Make fence to draw call.
        fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0); 
    }

    void RoundRobinAsyncBuffer::AdvanceFrame()
    {
        ++_frameIndex; //! Advance frame index;
    }

};