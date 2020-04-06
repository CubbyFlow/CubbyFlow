/*************************************************************************
> File Name: ColliderRenderable.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base rendererable for Renderer object.
> Created Time: 2020/03/29
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#include <Framework/Renderable/ColliderRenderable.h>
#include <Framework/Utils/Common.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    ColliderRenderable::ColliderRenderable()
    {

    }
    
    ColliderRenderable::~ColliderRenderable()
    {

    }

    void ColliderRenderable::onRender(RendererPtr renderer)
    {
        UNUSED_VARIABLE(renderer);
    }

    void ColliderRenderable::onSetEntry()
    {

    }

    void ColliderRenderable::onRelease()
    {

    }
} 
}