/*************************************************************************
> File Name: Buffer.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for buffer object which need 
            to be allocated by shader program.
> Created Time: 2020/03/17
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_BUFFER_H
#define CUBBYFLOW_BUFFER_H

#include <Framework/Object.h>
#include <Framework/Vertex.h>
#include <memory>
#include <string>

namespace CubbyFlow {
namespace CubbyRender {
    
    class Renderer;
    using RendererPtr = std::shared_ptr<Renderer>;
    class Material;
    using MaterialPtr = std::shared_ptr<Material>;

    //!
    //! \brief Abstract base class for Renderer Object
    //!
    class Buffer : public Object
    {
    public:
        //! Default constructor.
        Buffer();

        Buffer(const float* data, size_t numberOfElements, VertexFormat format=VertexFormat::Position3);

        //! Default destructor.
        virtual ~Buffer();
        
        //! Destroy the buffer resource.
        void destroy(RendererPtr renderer) override;
        
        //! Allocate gpu 
        void allocateResource(RendererPtr renderer, MaterialPtr material, const float* data, bool storeData);

        //! Update 
        void updateResource(RendererPtr renderer, MaterialPtr material, const float* data, 
                            size_t numberOfElements, VertexFormat format, bool storeData);

    protected:
        //! implementation of bind method
        virtual void onBind(RendererPtr renderer) = 0;
        
        //! implementation of unbind method
        virtual void onUnbind(RendererPtr renderer) = 0;

        //! implementation of destry method
        virtual void onDestroy(RendererPtr renderer) = 0;

        //! Allocate gpu 
        virtual void onAllocateResource(RendererPtr renderer, MaterialPtr material, const float* data, bool storeData) = 0;

        //! Update 
        virtual void onUpdateResource(RendererPtr renderer, MaterialPtr material, const float* data, bool storeData) = 0;

        const float* _data = nullptr; 
        size_t _numberOfElements;
        VertexFormat _format = VertexFormat::Position3;
    private:
    };

    using BufferPtr = std::shared_ptr<Buffer>;
} 
}

#endif