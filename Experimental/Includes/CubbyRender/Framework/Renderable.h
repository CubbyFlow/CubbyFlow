/*************************************************************************
> File Name: Renderable.h
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
#include <Framework/Object.h>
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
    class Renderable 
    {
    public:
        //! Default constructor.
        Renderable();

        //! Default destructor.
        virtual ~Renderable();

        void render(RendererPtr renderer);

        void setEntry(EntryPtr entry);

        void destroy();

    protected:
        virtual void onRender(RendererPtr renderer) = 0;

        virtual void onSetEntry() = 0;

        virtual void onDestroy() = 0;

        EntryPtr _entry;
    private:

    };

    using RenderablePtr = std::shared_ptr<Renderable>;
} 
}

#endif