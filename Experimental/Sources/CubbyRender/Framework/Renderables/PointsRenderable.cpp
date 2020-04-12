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
#include <Framework/Buffer/InputLayout.h>
#include <Framework/Buffer/Vertex.h>
#include <Framework/Renderer/Renderer.h>
#include <Framework/Shader/Shader.h>
#include <Framework/Utils/Common.h>
#include <Core/Array/Array1.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    PointsRenderable::PointsRenderable()
    {
        //! Do nothing.
    }

    PointsRenderable::PointsRenderable(RendererPtr renderer,
                                       const ConstArrayAccessor1<Vector3F>& positions,
                                       const ConstArrayAccessor1<Vector4F>& colors,
                                       float radius)
    {
        update(renderer, positions, colors, radius);
    }


    PointsRenderable::~PointsRenderable()
    {
        release();
    }

    void PointsRenderable::update(RendererPtr renderer,
                                  const ConstArrayAccessor1<Vector3F>& positions,
                                  const ConstArrayAccessor1<Vector4F>& colors)
    {
        update(renderer, positions, colors, _radius);
    }

    void PointsRenderable::update(RendererPtr renderer,
                                  const ConstArrayAccessor1<Vector3F>& positions,
                                  const ConstArrayAccessor1<Vector4F>& colors,
                                  float radius)
    {
        _radius = radius;
        Array1<float> vertices;
        const size_t totalSize = positions.size() * 3 + colors.size() * 4;

        std::lock_guard<std::mutex> lock(_dataMutex);
        vertices.Reserve(totalSize);
        for (size_t i = 0; i < totalSize / size_t(7); ++i)
        {
            vertices.Append(positions[i].x);
            vertices.Append(positions[i].y);
            vertices.Append(positions[i].z);
            vertices.Append(colors[i].x);
            vertices.Append(colors[i].y);
            vertices.Append(colors[i].z);
            vertices.Append(colors[i].w);
        }

        _inputLayout->updateVertexBuffer(renderer, vertices.ConstAccessor(), VertexFormat::Position3Color4);
    }

    void PointsRenderable::onRender(RendererPtr renderer)
    {
        renderer->bindMaterial(_material);
        renderer->setPrimitiveType(_primitiveType);
        renderer->draw(_inputLayout);
        renderer->unbindMaterial(_material);
    }

    void PointsRenderable::onRelease()
    {
        _inputLayout.reset();
        _material.reset();
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