/*************************************************************************
> File Name: Renderable.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base rendererable for Renderer object.
> Created Time: 2020/03/16
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_RENDERERABLE_H
#define CUBBYFLOW_RENDERERABLE_H

#include <Framework/Renderer/RenderOptions.h>
#include <Framework/Utils/Prerequisites.h>
#include <string>
#include <memory>

namespace CubbyFlow {
namespace CubbyRender {
    
    //!
    //! \brief Abstract base class for Rendererable object.
    //!
    //! This class contains basic interface for renderable object 
    //! and also resources which are essential for rendering.
    //! 
    //! The renderable instance does not represent the *unique instance* of specific species,
    //! but represents *whole instance* of specific species.
    //!
    class Renderable 
    {
    public:
        //! Default constructor.
        Renderable();

        //! Default destructor.
        virtual ~Renderable();

        void render(RendererPtr renderer);

        void release();

        void setPrimitiveType(PrimitiveType type);

    protected:
        virtual void onRender(RendererPtr renderer) = 0;

        virtual void onInitializeResource(RendererPtr renderer) = 0;

        virtual void onRelease() = 0;
        
        void invalidateResources();
        
        InputLayoutPtr _inputLayout { nullptr };
        MaterialPtr _material { nullptr };
        PrimitiveType _primitiveType { PrimitiveType::TriangleStrip };
    private:
        bool _bResourceInitialized { false };

    };

    using RenderablePtr = std::shared_ptr<Renderable>;
} 
}

#endif