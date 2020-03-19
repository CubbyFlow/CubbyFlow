/*************************************************************************
> File Name: Rendererable.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base rendererable for Renderer object.
> Created Time: 2020/03/16
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_RENDERERABLE_H
#define CUBBYFLOW_RENDERERABLE_H

#include <Framework/Prerequisites.h>
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
    class Rendererable
    {
    public:
        //! Default constructor.
        Rendererable();

        //! Default destructor.
        virtual ~Rendererable();

        void render(RendererPtr renderer);

    protected:
        virtual void onRender(RendererPtr renderer) = 0;
    private:

    };

    using RendererablePtr = std::shared_ptr<Rendererable>;
} 
}

#endif