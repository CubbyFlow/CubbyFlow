/*************************************************************************
> File Name: ScreenRenderable.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Renderable object for rendering framebuffer texture to screeen quad.
> Created Time: 2020/05/23
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_SCREEN_RENDERERABLE_H
#define CUBBYFLOW_SCREEN_RENDERERABLE_H

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
    class ScreenRenderable : public Renderable 
    {
    public:
        //! Default constructor.
        ScreenRenderable();
        
        //! Default destructor.
        virtual ~ScreenRenderable();

        //! Attach the screen texture to the renderable.
        void attachScreenTexture(Texture2DPtr texture);

    protected:
        //! bind input layout and material and draw renderables.
        virtual void onRender(RendererPtr renderer) override;

        //! initialize input layout, vertex buffers and materials.
        virtual void onInitializeResource(RendererPtr renderer) override;

        //! Release the resources.
        virtual void onRelease() override;
    private:
        Texture2DPtr _screenTexture;
    };

    using ScreenRenderablePtr = std::shared_ptr<ScreenRenderable>;
} 
}

#endif