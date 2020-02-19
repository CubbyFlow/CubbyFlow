/*************************************************************************
> File Name: GLRenderer.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: abstract Renderer class implemented with Modern OpenGL
> Created Time: 2020/02/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_GLRENDERER_H
#define CUBBYFLOW_GLRENDERER_H

#include <memory>
#include <Framework/Renderer.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    //!
    //! \brief Abstract base class for Renderer object.
    //!
    //! This class contains basic interface for renderer. 
    //! The wrapper class of each Graphics API must overrides 
    //! this renderer class.
    //!
    class GL3Renderer final : public Renderer
    {
    public:
        //! Default constructor.
        GL3Renderer();

        //! Default destructor.
        ~GL3Renderer();

        //! Initialize and fetch gl commands.
        int initializeGL() override;
    protected:
    private:
    };

    using GL3RendererPtr = std::shared_ptr<GL3Renderer>;
} 
}

#endif