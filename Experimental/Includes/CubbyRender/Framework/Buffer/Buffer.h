/*************************************************************************
> File Name: Buffer.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for Renderer Buffer
> Created Time: 2020/02/19
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_BUFFER_H
#define CUBBYFLOW_BUFFER_H

#include <memory>
#include <string>

namespace CubbyFlow {
namespace CubbyRender {
    
    class Renderer;
    using RendererPtr = std::shared_ptr<Renderer>;

    //!
    //! \brief Abstract base class for Renderer Buffer
    //!
    class Buffer
    {
    public:
        //! Default constructor.
        Buffer();

        //! Default destructor.
        virtual ~Buffer();

        //! Bind this Buffer to renderer
        void bind(RendererPtr renderer);

        //! Unbind this Buffer from renderer
        void unbind(RendererPtr renderer);    

        //! Destroy this Buffer instance
        void destroy();

    protected:
        //! implementation of bind method
        virtual void onBind(RendererPtr renderer) = 0;
        
        //! implementation of unbind method
        virtual void onUnbind(RendererPtr renderer) = 0;

        //! implementation of destroy method
        virtual void onDestroy() = 0;
    private:
    };

    using BufferPtr = std::shared_ptr<Buffer>;
} 
}

#endif