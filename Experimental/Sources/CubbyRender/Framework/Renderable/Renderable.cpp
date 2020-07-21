/*************************************************************************
> File Name: Rendererable.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base rendererable for Renderer object.
> Created Time: 2020/03/16
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Framework/Renderable/Renderable.h>
#include <Framework/Utils/DeviceTimer.h>
#include <Framework/Utils/Common.h>
#include <Core/Utils/Timer.h>

namespace CubbyFlow {
namespace CubbyRender {

    Renderable::Renderable()
    {
        //! Do nothing
    }

    Renderable::~Renderable()
    {
        //! Do nothing
    }

    void Renderable::render(RendererPtr renderer)
    {
        if (_bResourceInitialized == false)
        {
            DeviceTimer timer;
            onInitializeResource(renderer);
            CUBBYFLOW_INFO << "Update Renderable took " << timer.DurationInSeconds() << " seconds";
            _bResourceInitialized = true;
        }
        onRender(renderer);
    }

    void Renderable::release()
    {
        _inputLayout.reset();
        _material.reset();
        onRelease();
    }

    void Renderable::setPrimitiveType(PrimitiveType type)
    {
        _primitiveType = type;
    }

    void Renderable::setMaterial(MaterialPtr material)
    {
        _material = material;
    }

    void Renderable::invalidateResources()
    {
        _bResourceInitialized = false;
    }
} 
}