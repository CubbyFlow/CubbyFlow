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

namespace CubbyFlow {
namespace CubbyRender {

    Renderable::Renderable()
    {
        //! Do nothing
    }

    Renderable::Renderable(InputLayoutPtr inputLayout, MaterialPtr material)
        : _inputLayout(inputLayout), _material(material)
    {
        //! Do nothing
    }

    Renderable::~Renderable()
    {
        //! Do nothing
    }

    void Renderable::attachInputLayout(InputLayoutPtr inputLayout)
    {
        _inputLayout = inputLayout;
    }

    void Renderable::attachMaterial(MaterialPtr material)
    {
        _material = material;
    }

    void Renderable::render(RendererPtr renderer)
    {
        onRender(renderer);
    }

    void Renderable::release()
    {
        onRelease();
    }
} 
}