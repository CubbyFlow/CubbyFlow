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

    }

    PointsRenderable::~PointsRenderable()
    {

    }

    PointsRenderable::PointsRenderable(const ConstArrayAccessor1<Vector3F>& positions,
                                       const ConstArrayAccessor1<Vector4F>& colors,
                                       float radius)
    {
        update(positions, colors, radius);
    }

    void PointsRenderable::update(const ConstArrayAccessor1<Vector3F>& positions,
                                 const ConstArrayAccessor1<Vector4F>& colors)
    {
        update(positions, colors, _radius);
    }

    void PointsRenderable::update(const ConstArrayAccessor1<Vector3F>& positions,
                                  const ConstArrayAccessor1<Vector4F>& colors,
                                  float radius)
    {
        _radius = radius;
        Array1<VertexPosition3Color4> vertices;

        std::lock_guard<std::mutex> lock(_dataMutex);
        vertices.Resize(positions.size());

        vertices.ParallelForEachIndex([&](size_t i){
            auto& vertex = vertices[i];
            vertex.x = positions[i].x;
            vertex.y = positions[i].y;
            vertex.z = positions[i].z;
            vertex.r = colors[i].x;
            vertex.g = colors[i].y;
            vertex.b = colors[i].z;
            vertex.a = colors[i].w;
        });

        float* data = reinterpret_cast<float*>(vertices.data());
        UNUSED_VARIABLE(data);
    }

    void PointsRenderable::onRender(RendererPtr renderer)
    {
        _material->getShader()->bind(renderer);
        _inputLayout->bind(renderer);
        renderer->setPrimitiveType(_primitiveType);
        renderer->draw(_inputLayout->getNumberOfVertices());
        _inputLayout->unbind(renderer);
        _material->getShader()->unbind(renderer);
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