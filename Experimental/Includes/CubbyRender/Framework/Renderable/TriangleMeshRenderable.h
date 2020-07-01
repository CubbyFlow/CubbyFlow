/*************************************************************************
> File Name: TriangleMeshRenderable.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Triangle Mesh Renderable class for efficiently handle triangle datum.
> Created Time: 2020/05/23
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_TRIANGLE_MESH_RENDERERABLE_H
#define CUBBYFLOW_TRIANGLE_MESH_RENDERERABLE_H

#include <Framework/Renderable/Renderable.h>
#include <Core/Array/ArrayAccessor1.h>
#include <Core/Array/Array1.h>
#include <Core/Vector/Vector3.h>
#include <Core/Vector/Vector4.h>
#include <string>
#include <mutex>

namespace CubbyFlow {
namespace CubbyRender {
    
    //!
    //! \brief Abstract base class for Renderer object.
    //!
    //! This class contains basic interface for renderer. 
    //! The wrapper class of each Graphics API must overrides 
    //! this renderer class.
    //!
    class TriangleMeshRenderable : public Renderable 
    {
    public:
        //! Default constructor.
        TriangleMeshRenderable();
        
        //! Initialize the current position attribute and normal attribute of the vertex buffer.
        TriangleMeshRenderable(const ConstArrayAccessor1<Vector3F>& positions);

        //! Default destructor.
        virtual ~TriangleMeshRenderable();

        //! Update the current position attribute and normal attribute of the vertex buffer.
        void update(const ConstArrayAccessor1<Vector3F>& positions);

    protected:
        //! bind input layout and material and draw renderables.
        virtual void onRender(RendererPtr renderer) override;

        //! initialize input layout, vertex buffers and materials.
        virtual void onInitializeResource(RendererPtr renderer) override;

        //! Release the resources.
        virtual void onRelease() override;

        std::mutex _dataMutex;
    private:
        Array1<float> _vertices;
    };

    using TriangleMeshRenderablePtr = std::shared_ptr<TriangleMeshRenderable>;
} 
}

#endif