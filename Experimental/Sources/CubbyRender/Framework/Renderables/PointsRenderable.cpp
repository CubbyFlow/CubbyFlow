/*************************************************************************
> File Name: PointsRenderable.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base rendererable for Renderer object.
> Created Time: 2020/03/29
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#include <Framework/Renderable/PointsRenderable.h>
#include <Framework/Utils/Common.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    PointsRenderable::PointsRenderable()
    {

    }

    PointsRenderable::~PointsRenderable()
    {

    }

    void PointsRenderable::onRender(RendererPtr renderer)
    {
        UNUSED_VARIABLE(renderer);
    }

    void PointsRenderable::onSetEntry()
    {

    }

    void PointsRenderable::onDestroy()
    {

    }

} 
}