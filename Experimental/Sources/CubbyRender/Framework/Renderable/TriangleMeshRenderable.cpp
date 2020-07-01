/*************************************************************************
> File Name: TriangleMeshRenderable.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Triangle Mesh Renderable class for efficiently handle triangle datum.
> Created Time: 2020/05/23
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#include <Framework/Renderable/TriangleMeshRenderable.h>
#include <Framework/Renderable/Material.h>
#include <Framework/Buffer/VertexBuffer.h>
#include <Framework/Buffer/IndexBuffer.h>
#include <Framework/Buffer/InputLayout.h>
#include <Framework/Buffer/Vertex.h>
#include <Framework/Renderer/Renderer.h>
#include <Framework/Shader/Shader.h>
#include <Framework/Utils/Common.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    TriangleMeshRenderable::TriangleMeshRenderable()
    {
        //! Do nothing.
    }

    TriangleMeshRenderable::TriangleMeshRenderable(const ConstArrayAccessor1<Vector3F>& positions)
    {
        update(positions);
    }

    TriangleMeshRenderable::~TriangleMeshRenderable()
    {
        release();
    }

    void TriangleMeshRenderable::update(const ConstArrayAccessor1<Vector3F>& positions)
    {
        const std::size_t totalSize = positions.size() * 3;
        const std::size_t numVertices = positions.size();
        std::lock_guard<std::mutex> lock(_dataMutex);
        _vertices.Resize(totalSize);
        for (size_t i = 0; i < numVertices; ++i)
        {
            _vertices[i * 3 + 0] = positions[i].x;
            _vertices[i * 3 + 1] = positions[i].y;
            _vertices[i * 3 + 2] = positions[i].z;
        }
        invalidateResources();
    }

    void TriangleMeshRenderable::onInitializeResource(RendererPtr renderer)
    {
        
        //! allocate and initialize material instance.
        if (_material == nullptr)
        {
            _material = renderer->createMaterial();
            ShaderPtr shader = renderer->createShaderPreset("fluid_mesh_shader");
            _material->setShader(shader);
        }
        //! set radius uniform variable of the shader.
        //! auto& params = _material->getShader()->getParameters();
        //! params.setParameter("Radius", _radius);
        
        //! allocate and initialize input layout instance.
        if (_inputLayout == nullptr)
        {
            _inputLayout = renderer->createInputLayout();
            //! create vertex buffer from the positions and colors.
            VertexBufferPtr buffer = renderer->createVertexBuffer(_vertices.ConstAccessor(), 
                                                                  static_cast<size_t>(_vertices.size() / size_t(3)),  
                                                                  VertexFormat::Position3);
            _inputLayout->attachVertexBuffer(renderer, _material, buffer);
        }
        else
        {
            //! without initialization, update the contents of the buffer.
            _inputLayout->updateVertexBuffer(renderer, _vertices.ConstAccessor(), VertexFormat::Position3);
        }
        //! Set the primitive type as Point.
        _primitiveType = PrimitiveType::TriangleStrip;
    }

    void TriangleMeshRenderable::onRender(RendererPtr renderer)
    {
        renderer->bindMaterial(_material);
        renderer->bindInputLayout(_inputLayout);
        renderer->setPrimitiveType(_primitiveType);
        renderer->draw(_inputLayout);
        renderer->unbindInputLayout(_inputLayout);
        renderer->unbindMaterial(_material);
    }

    void TriangleMeshRenderable::onRelease()
    {
        //! Do nothing.
    }
} 
}