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
#include <cstring>
#include <iostream>

namespace CubbyFlow {
namespace CubbyRender {
    
    TriangleMeshRenderable::TriangleMeshRenderable()
    {
        //! Do nothing.
    }

    TriangleMeshRenderable::TriangleMeshRenderable(const ConstArrayAccessor1<float>& vertices,
                                                   const ConstArrayAccessor1<unsigned int>& indices)
    {
        update(vertices, indices);
    }

    TriangleMeshRenderable::~TriangleMeshRenderable()
    {
        release();
    }

    void TriangleMeshRenderable::update(const ConstArrayAccessor1<float>& vertices,
                                        const ConstArrayAccessor1<unsigned int>& indices)
    {
        std::lock_guard<std::mutex> lock(_dataMutex);
        _vertices.Resize(vertices.size()); _indices.Resize(indices.size());
        std::memcpy(static_cast<void*>(_vertices.data()), static_cast<const void*>(vertices.data()), vertices.size() * sizeof(float));
        std::memcpy(static_cast<void*>(_indices.data()), static_cast<const void*>(indices.data()), indices.size() * sizeof(unsigned int));
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
            VertexBufferPtr vertexBuffer = renderer->createVertexBuffer(_vertices.ConstAccessor(), 
                                                                        static_cast<size_t>(_vertices.size() / 6),  
                                                                        VertexFormat::Position3Normal3);
            //IndexBufferPtr elementBuffer = renderer->createIndexBuffer(_indices.ConstAccessor(), _indices.size());
            _inputLayout->attachVertexBuffer(renderer, _material, vertexBuffer);
            //_inputLayout->attachIndexBuffer(renderer, elementBuffer);
        }
        else
        {
            //! without initialization, update the contents of the buffer.
            _inputLayout->updateVertexBuffer(renderer, _vertices.ConstAccessor(), VertexFormat::Position3Normal3);
            //_inputLayout->updateIndexBuffer(renderer, _indices.ConstAccessor());
        }
        //! Set the primitive type as Point.
        _primitiveType = PrimitiveType::Triangle;
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