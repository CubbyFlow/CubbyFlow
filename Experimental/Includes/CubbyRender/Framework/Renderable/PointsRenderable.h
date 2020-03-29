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
    class PointsRenderable : public Renderable 
    {
    public:
        //! Default constructor.
        PointsRenderable();

        //! Default destructor.
        virtual ~PointsRenderable();

    protected:
        virtual void onRender(RendererPtr renderer) override;

        virtual void onSetEntry() override;

        virtual void onDestroy() override;
    private:

    };

    using PointsRenderablePtr = std::shared_ptr<PointsRenderable>;
} 
}

#endif