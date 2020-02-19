/*************************************************************************
> File Name: VertexBuffer.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for VertexBuffer object.
> Created Time: 2020/02/19
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VERTEXBUFFER_H
#define CUBBYFLOW_VERTEXBUFFER_H

#include <memory>
#include <Framework/Object.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    class Renderer;
    using RendererPtr = std::shared_ptr<Renderer>;

    //!
    //! \brief Abstract base class for Shader object.
    //!
    class VertexBuffer : public Object
    {
    public:
        //! Default constructor.
        VertexBuffer();

        //! Default destructor.
        virtual ~VertexBuffer();
    protected:
        //! implementation of bind method
        virtual void onBind(RendererPtr renderer) = 0;
        
        //! implementation of unbind method
        virtual void onUnbind(RendererPtr renderer) = 0;

        //! implementation of destry method
        virtual void onDestroy() = 0;
    private:
    };

    using VertexBufferPtr = std::shared_ptr<VertexBuffer>;
} 
}

#endif