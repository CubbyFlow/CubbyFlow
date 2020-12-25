/*************************************************************************
> File Name: FluidBuffer.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The buffer class which store buffer desc and own raw pointer optionally.
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_ROUND_ROBIN_ASYNC_BUFFER_HPP
#define CUBBYFLOW_VOX_ROUND_ROBIN_ASYNC_BUFFER_HPP

#include <Vox/Utils/GLTypes.hpp>
#include <memory>
#include <vector>

typedef struct __GLsync *GLsync;

namespace Vox {
    class FrameContext;
    class GeometryCacheManager;
    /**
     * Buffer for simulating which need huge effort for optimizing data transfer performance.
     * Implemented with multiple buffer technique (round-robin)
     **/
    class FluidBuffer
    {
    public:
        //! Constructor with number of the buffers.
        FluidBuffer(const size_t numBuffer);
        //! Default destructor.
        ~FluidBuffer();
    
        //! Resize the buffer number.
        void Resize(const size_t numBuffer);
        //! Draw one frame of the particles data.
        bool CheckFence(GLuint64 timeout);
        //! Asynchronously transfer scene data to vertex buffer.
        void AsyncBufferTransfer(const std::shared_ptr<GeometryCacheManager>& cacheManager);
        //! Draw the frmae with the transferred vertex buffer.
        void DrawFrame(const std::shared_ptr<FrameContext>& ctx);
        //! Advance the frame index.
        void AdvanceFrame();

        static const size_t kMaxBufferSize = 0x1000000; //! 2097 kB - This is GPU memory
        static const size_t kDefaultNumBuffer = 3;

    private:
        std::vector<GLsync> _fences; 
        std::vector<GLuint> _vaos;
        std::vector<GLuint> _vbos;
        std::vector<GLuint> _ebos;
        size_t _numElements { 0 };
        size_t _numBuffer { 0 };
        size_t _frameIndex { 0 } ;
    };
};

#endif