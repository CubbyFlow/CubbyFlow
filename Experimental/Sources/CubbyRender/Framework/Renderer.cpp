/*************************************************************************
> File Name: Renderer.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for Renderer object.
> Created Time: 2020/02/15
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#include <Framework/Renderer.h>
#include <Framework/Renderable.h>

namespace CubbyFlow {
namespace CubbyRender {
        
    Renderer::Renderer()
    {
        //! Do nothing
    }

    Renderer::~Renderer()
    {
        //!clearRenderables();
    }

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
        for (auto renderable : _renderables)
            renderable->destroy();
        _renderables.clear();
    }
    
    void Renderer::setBackgroundColor(Vector4F color)
    {
        _backgroundColor = color;
    }
    
} 
}