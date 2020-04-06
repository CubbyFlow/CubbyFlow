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
#include <Framework/Renderable/Material.h>
#include <Framework/Buffer/VertexBuffer.h>
#include <Framework/Buffer/IndexBuffer.h>
#include <Framework/Shader/Shader.h>
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
        if (_entry)
        {
            _entry->material->getShader()->bind(renderer);
            //!_entry->inputLayout->bind(renderer);
        }
    }

    void PointsRenderable::onSetEntry()
    {

    }

    void PointsRenderable::onRelease()
    {
        if (_entry)
        {
            _entry.reset();
        }
    }

    void PointsRenderable::setRadius(float radius)
    {
        _radius = radius;
    }

    float PointsRenderable::getRadius() const
    {
        return _radius;
    }

} 
}