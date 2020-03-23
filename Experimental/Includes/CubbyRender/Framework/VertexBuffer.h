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

#include <Framework/Prerequisites.h>
#include <Framework/Vertex.h>
#include <Framework/Object.h>
#include <Core/Array/Array1.h>
#include <Core/Array/ArrayAccessor1.h>
#include <memory>

namespace CubbyFlow {
namespace CubbyRender {

    //!
    //! \brief Abstract base class for Shader object.
    //!
    class VertexBuffer : public Object
    {
    public:
        //! Default constructor.
        VertexBuffer();

        //! Constructor with parameters
        VertexBuffer(VertexFormat format);
        
        //! Default destructor.
        virtual ~VertexBuffer();
        
        //! Allocate gpu 
        void allocateBuffer(RendererPtr renderer, MaterialPtr material, const ConstArrayAccessor1<float>& data, size_t numberOfVertices, bool storeData);

        //! Update 
        virtual void updateBuffer(RendererPtr renderer, MaterialPtr material, const ConstArrayAccessor1<float>& data, bool storeData) = 0;

        size_t getNumberOfVertices() const;

        VertexFormat getFormat() const;
    protected:
        //! implementation of bind method
        virtual void onBind(RendererPtr renderer) = 0;
        
        //! implementation of unbind method
        virtual void onUnbind(RendererPtr renderer) = 0;

        //! implementation of destry method
        virtual void onDestroy(RendererPtr renderer) = 0;

        //! Allocate gpu 
        virtual void onAllocateBuffer(RendererPtr renderer, MaterialPtr material, const ConstArrayAccessor1<float>& data) = 0;

        Array1<float> _data;
        size_t _numberOfVertices;
        VertexFormat _format;
    private:
    };

    using VertexBufferPtr = std::shared_ptr<VertexBuffer>;
} 
}

#endif