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

        //! Constructor with inputlayout and material
        PointsRenderable(RendererPtr renderer,
                         const ConstArrayAccessor1<Vector3F>& positions,
                         const ConstArrayAccessor1<Vector4F>& colors,
                         float radius);

        void update(RendererPtr renderer,
                    const ConstArrayAccessor1<Vector3F>& positions,
                    const ConstArrayAccessor1<Vector4F>& colors);

        void update(RendererPtr renderer,
                    const ConstArrayAccessor1<Vector3F>& positions,
                    const ConstArrayAccessor1<Vector4F>& colors,
                    float radius);

        //! Default destructor.
        virtual ~PointsRenderable();

        //! Set radius of the point
        void setRadius(float radius);

        //! Get radius of the point
        float getRadius() const;

    protected:
        virtual void onRender(RendererPtr renderer) override;

        virtual void onInitializeResource(RendererPtr renderer) override;

        virtual void onRelease() override;

        float _radius = 0.0f;
        std::mutex _dataMutex;
    private:
        Array1<float> _vertices;
    };

    using PointsRenderablePtr = std::shared_ptr<PointsRenderable>;
} 
}

#endif