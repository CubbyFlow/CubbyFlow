/*************************************************************************
> File Name: CylinderRenderable.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base rendererable for Renderer object.
> Created Time: 2020/03/29
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#include <Framework/Renderable/CylinderRenderable.h>
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
    
    CylinderRenderable::CylinderRenderable()
    {

    }

    CylinderRenderable::CylinderRenderable(const Cylinder3Ptr& cylinder)
    {
        update(cylinder);
    }
    
    CylinderRenderable::~CylinderRenderable()
    {

    }

    void CylinderRenderable::update(const Cylinder3Ptr& cylinder)
    {
        std::lock_guard<std::mutex> lock(_dataMutex);
        //! rebuilding vertices ...
        invalidateResources();
    }

    void CylinderRenderable::onInitializeResource(RendererPtr renderer)
    {
        //! allocate and initialize material instance.
        if (_material == nullptr)
        {
            _material = renderer->createMaterial();
            ShaderPtr shader = renderer->createShaderPreset("collider_shader");
            _material->setShader(shader);
        }
        //! set radius uniform variable of the shader.
        auto& params = _material->getShader()->getParameters();
        //! params.setParameter("Radius", _radius);

        //! allocate and initialize input layout instance.
        if (_inputLayout == nullptr)
        {
            _inputLayout = renderer->createInputLayout();
            //! create vertex buffer from the positions and colors.
            VertexBufferPtr buffer = renderer->createVertexBuffer(_vertices.ConstAccessor(), 
                                                                  static_cast<size_t>(_vertices.size() / size_t(7)),  
                                                                  VertexFormat::Position3Color4);
            _inputLayout->attachVertexBuffer(renderer, _material, buffer);
        }
        else
        {
            //! without initialization, update the contents of the buffer.
            _inputLayout->updateVertexBuffer(renderer, _vertices.ConstAccessor(), VertexFormat::Position3Color4);
        }
        //! Set the primitive type as Point.
        _primitiveType = PrimitiveType::TriangleStrip;
    }

    void CylinderRenderable::onRender(RendererPtr renderer)
    {
        renderer->bindMaterial(_material);
        renderer->bindInputLayout(_inputLayout);
        renderer->setPrimitiveType(_primitiveType);
        renderer->draw(_inputLayout);
        renderer->unbindInputLayout(_inputLayout);
        renderer->unbindMaterial(_material);
    }

    void CylinderRenderable::onRelease()
    {
        //! Do nothing.
    }
} 
}