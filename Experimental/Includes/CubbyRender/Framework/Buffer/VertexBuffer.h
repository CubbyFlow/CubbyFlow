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

#include <Framework/Utils/Prerequisites.h>
#include <Framework/Buffer/Vertex.h>
#include <Framework/Buffer/Buffer.h>
#include <Core/Array/Array1.h>
#include <Core/Array/ArrayAccessor1.h>
#include <memory>

namespace CubbyFlow {
namespace CubbyRender {

    //!
    //! \brief Abstract base class for Shader object.
    //!
    class VertexBuffer : public Buffer
    {
    public:
        //! Default constructor.
        VertexBuffer();

        //! Default constructor.
        VertexBuffer(VertexFormat format);
        
        //! Default destructor.
        virtual ~VertexBuffer();

        //! Set input vertex format
        void setInputVertexFormat(VertexFormat format);

        //! Get input vertex format.
        VertexFormat getInputVertexFormat() const;

        //! Allocate gpu 
        void allocateBuffer(RendererPtr renderer, const ConstArrayAccessor1<float>& data, size_t numberOfVertices);

        //! Update 
        virtual void updateBuffer(RendererPtr renderer, const ConstArrayAccessor1<float>& data) = 0;

        size_t getNumberOfVertices() const;
        
        void bindState(RendererPtr renderer, MaterialPtr material);

    protected:
        //! implementation of bind method
        virtual void onBind(RendererPtr renderer) override = 0;
        
        //! implementation of unbind method
        virtual void onUnbind(RendererPtr renderer) override = 0;

        //! implementation of destry method
        virtual void onDestroy() override = 0;

        //! Allocate gpu 
        virtual void onAllocateBuffer(RendererPtr renderer, const ConstArrayAccessor1<float>& data) = 0;

        //! implementation of bind state method.
        virtual void onBindState(RendererPtr renderer, MaterialPtr material) = 0;

        VertexFormat _vertexFormat = VertexFormat::Position3Normal3;
        size_t _numberOfVertices;
    private:
    };

    using VertexBufferPtr = std::shared_ptr<VertexBuffer>;
} 
}

#endif