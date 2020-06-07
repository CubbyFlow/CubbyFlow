/*************************************************************************
> File Name: Renderer.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for Renderer object.
> Created Time: 2020/02/15
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#include <Framework/Renderer/Renderer.h>
#include <Framework/Renderable/Renderable.h>
#include <Framework/Renderable/Material.h>
#include <Framework/Buffer/InputLayout.h>
#include <Framework/Shader/Shader.h>
#include <cassert>

namespace CubbyFlow {
namespace CubbyRender {
        
    Renderer::Renderer()
    {
        //! Do nothing
    }

    Renderer::~Renderer()
    {
        clearRenderables();
    }

    //!void Renderer::render(RenderablePtr renderable)
    //!{
    //!    onRenderBegin();
    //!    renderable->render(shared_from_this());
    //!    onRenderEnd();
    //!}

    void Renderer::render()
    {
        onRenderBegin();
        for (auto renderable : _renderables)
            renderable->render(shared_from_this());
        onRenderEnd();
    }
    
    void Renderer::setRenderState(const RenderState& renderState)
    {
        _renderState = renderState;
        onSetRenderState();
    }

    void Renderer::setPrimitiveType(PrimitiveType type)
    {
        _primitiveType = type;
    }
    
    void Renderer::addRenderable(RenderablePtr renderable)
    {
        _renderables.push_back(renderable);
    }
    
    void Renderer::clearRenderables()
    {
        _renderables.clear();
    }
    
    void Renderer::setBackgroundColor(Vector4F color)
    {
        _backgroundColor = color;
    }
    
    MaterialPtr Renderer::createMaterial() const
    {
        MaterialPtr material = std::make_shared<Material>();
        return material;
    }

    void Renderer::bindMaterial(MaterialPtr material)
    {
        assert(material);
        material->bind(shared_from_this());
    }
    
    void Renderer::unbindMaterial(MaterialPtr material)
    {
        assert(material);
        material->unbind(shared_from_this());
    }

    void Renderer::bindInputLayout(InputLayoutPtr inputLayout)
    {
        assert(inputLayout);
        inputLayout->bind(shared_from_this());
    }
    void Renderer::unbindInputLayout(InputLayoutPtr inputLayout)
    {
        assert(inputLayout);
        inputLayout->unbind(shared_from_this());
    }

    void Renderer::setCamera(CameraPtr camera)
    {
        _camera = camera;
    }

    const CameraPtr& Renderer::getCamera() const
    {
        return _camera;
    }
} 
}