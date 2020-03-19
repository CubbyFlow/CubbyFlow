/*************************************************************************
> File Name: IndexBuffer.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for IndexBuffer object.
> Created Time: 2020/03/16
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_INDEXBUFFER_H
#define CUBBYFLOW_INDEXBUFFER_H

#include <memory>
#include <Framework/Buffer.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    class Renderer;
    using RendererPtr = std::shared_ptr<Renderer>;

    //!
    //! \brief Abstract base class for Shader object.
    //!
    class IndexBuffer : public Buffer
    {
    public:
        //! Default constructor.
        IndexBuffer();

        //! Default destructor.
        virtual ~IndexBuffer();
    protected:
        //! implementation of bind method
        virtual void onBind(RendererPtr renderer) = 0;
        
        //! implementation of unbind method
        virtual void onUnbind(RendererPtr renderer) = 0;

        //! implementation of destry method
        virtual void onDestroy(RendererPtr renderer) = 0;

        //! Allocate gpu 
        virtual void onAllocateResource(RendererPtr renderer, MaterialPtr material, float* data, bool storeData) = 0;

        //! Update 
        virtual void onUpdateResource(RendererPtr renderer, MaterialPtr material, float* data, 
                              size_t numberOfVertices, VertexFormat format, bool storeData) = 0;
    private:
    };

    using IndexBufferPtr = std::shared_ptr<IndexBuffer>;
} 
}

#endif