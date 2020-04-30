/*************************************************************************
> File Name: CylinderRenderable.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base rendererable for Renderer object.
> Created Time: 2020/03/16
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_CYLINDER_RENDERABLE_H
#define CUBBYFLOW_CYLINDER_RENDERABLE_H

#include <Framework/Renderable/Renderable.h>
#include <Core/Geometry/Cylinder3.h>
#include <string>

namespace CubbyFlow {
namespace CubbyRender {
    
    //!
    //! \brief Abstract base class for Renderer object.
    //!
    //! This class contains basic interface for renderer. 
    //! The wrapper class of each Graphics API must overrides 
    //! this renderer class.
    //!
    class CylinderRenderable : public Renderable 
    {
    public:
        //! Default constructor.
        CylinderRenderable();

        //! Constructor with Cylinder object.
        CylinderRenderable(const Cylinder3Ptr& cylinder);

        //! Update the buffer with new Cylinder object.
        void update(const Cylinder3Ptr& cylinder);

        //! Default destructor.
        virtual ~CylinderRenderable();

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

    using CylinderRenderablePtr = std::shared_ptr<CylinderRenderable>;
} 
}

#endif