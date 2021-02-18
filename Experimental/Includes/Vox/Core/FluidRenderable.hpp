/*************************************************************************
> File Name: FluidRenderable.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The buffer class which store buffer desc and own raw pointer optionally.
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_FLUID_RENDERABLE_HPP
#define CUBBYFLOW_VOX_FLUID_RENDERABLE_HPP

#include <Vox/Core/RenderableObject.hpp>
#include <Vox/Utils/GLTypes.hpp>
#include <memory>

typedef struct __GLsync* GLsync;

namespace Vox {
    class FrameContext;
    class GeometryCacheManager;
    /**
     * Buffer for simulating which need huge effort for optimizing data transfer performance.
     * Implemented with multiple buffer technique (round-robin)
     **/
    class FluidRenderable : public RenderableObject
    {
    public:
        //! Constructor with number of the buffers.
        FluidRenderable(const std::shared_ptr<GeometryCacheManager>& manager, const size_t numBuffer = kDefaultNumBuffer);
        //! Default destructor.
        ~FluidRenderable();

        //! Resize the buffer number.
        void Resize(const size_t numBuffer = kDefaultNumBuffer);
        //! Draw one frame of the particles data.
        bool CheckFence(GLuint64 timeout);
        //! Asynchronously transfer scene data to vertex buffer.
        void AsyncBufferTransfer();
        //! Draw the frmae with the transferred vertex buffer.
        void DrawRenderableObject(const std::shared_ptr<FrameContext>& ctx) override;
        //! Advance the frame index.
        void AdvanceFrame();
        //! Attach geometry cache manager
        void SetGeometryCacheManager(const std::shared_ptr<GeometryCacheManager>& manager);
        //! Clean-up the meshes.
        void CleanUp();

        static const size_t kMaxBufferSize = 0x1000000; //! 2097 kB - This is GPU memory
        static const size_t kDefaultNumBuffer = 3;
    protected:
        void ConfigureRenderSettings(const std::shared_ptr<FrameContext>& ctx) override;
    private:
        std::shared_ptr<GeometryCacheManager> _cacheManager;
        CubbyFlow::Array1<GLsync> _fences;
        size_t _numBuffer{ 0 };
        size_t _frameIndex{ 0 };
    };
};

#endif