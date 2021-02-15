/*************************************************************************
> File Name: StaticRenderable.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The static object class
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_STATIC_RENDERABLE_HPP
#define CUBBYFLOW_VOX_STATIC_RENDERABLE_HPP

#include <Vox/Core/RenderableObject.hpp>
#include <memory>

typedef struct __GLsync* GLsync;

namespace Vox {
    class FrameContext;
    /**
     * Buffer for simulating which need huge effort for optimizing data transfer performance.
     * Implemented with multiple buffer technique (round-robin)
     **/
    class StaticRenderable : public RenderableObject
    {
    public:
        //! Default constructor
        StaticRenderable();
        //! Default destructor.
        ~StaticRenderable();

        //! Draw the frmae with the transferred vertex buffer.
        void DrawRenderableObject(const std::shared_ptr<FrameContext>& ctx) override;

    protected:
        void ConfigureRenderSettings(const std::shared_ptr<FrameContext>& ctx) override;
    private:
    };
};

#endif