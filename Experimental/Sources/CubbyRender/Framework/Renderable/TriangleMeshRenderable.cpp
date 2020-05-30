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

    TriangleMeshRenderable::~TriangleMeshRenderable()
    {
        release();
    }

    void TriangleMeshRenderable::onInitializeResource(RendererPtr renderer)
    {
        UNUSED_VARIABLE(renderer);
    }

    void TriangleMeshRenderable::onRender(RendererPtr renderer)
    {
        UNUSED_VARIABLE(renderer);
    }

    void TriangleMeshRenderable::onRelease()
    {
        //! Do nothing.
    }

} 
}