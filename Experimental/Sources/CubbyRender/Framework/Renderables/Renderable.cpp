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
    
    Entry::Entry()
    {
        //! Do nothing.
    }

    Entry::Entry(InputLayoutPtr inputLayout, MaterialPtr material, PrimitiveType type, unsigned int blendOrder)
          : inputLayout(inputLayout), material(material), type(type), blendOrder(blendOrder)
    {
        //! Do nothing
    }

    Entry::~Entry()
    {
        release();
    }

    void Entry::release()
    {
        inputLayout.reset();
        material.reset();
    }

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
        onRender(renderer);
    }

    void Renderable::setEntry(EntryPtr entry)
    {
        _entry = entry;
    }

    void Renderable::release()
    {
        onRelease();
    }

    

} 
}