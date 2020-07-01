/*************************************************************************
> File Name: PointsRenderable.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base rendererable for Renderer object.
> Created Time: 2020/03/16
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_POINTS_RENDERERABLE_H
#define CUBBYFLOW_POINTS_RENDERERABLE_H

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
    class PointsRenderable : public Renderable 
    {
    public:
        //! Default constructor.
        PointsRenderable();

        //! Constructor with positions, color of the particle point.
        PointsRenderable(const ConstArrayAccessor1<Vector3F>& positions, const Vector3F& color);
        
        //! Update the current position attribute and color uniform of the vertex buffer.
        void update(const ConstArrayAccessor1<Vector3F>& positions);

        //! Update the current position attribute and color uniform of the vertex buffer.
        void update(const ConstArrayAccessor1<Vector3F>& positions, const Vector3F& color);

        //! Default destructor.
        virtual ~PointsRenderable();

        //! Set radius of the fluid particle
        void setColor(const Vector3F& color);

        //! Get radius of the  fluid particle
        Vector3F getColor() const;

    protected:
        //! bind input layout and material and draw renderables.
        virtual void onRender(RendererPtr renderer) override;

        //! initialize input layout, vertex buffers and materials.
        virtual void onInitializeResource(RendererPtr renderer) override;

        //! Release the resources.
        virtual void onRelease() override;

        Vector3F _color = Vector3F(221 / 255.0f, 145 / 255.0f, 48 / 255.0f);
        std::mutex _dataMutex;
    private:
        Array1<float> _vertices;
    };

    using PointsRenderablePtr = std::shared_ptr<PointsRenderable>;
} 
}

#endif