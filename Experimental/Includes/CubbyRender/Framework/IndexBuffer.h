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

#include <Framework/Prerequisites.h>
#include <Framework/Object.h>
#include <Core/Array/Array1.h>
#include <Core/Array/ArrayAccessor1.h>
#include <memory>

namespace CubbyFlow {
namespace CubbyRender {
    
    //!
    //! \brief Abstract base class for Shader object.
    //!
    class IndexBuffer : public Object
    {
    public:
        //! Default constructor.
        IndexBuffer();

        //! Default destructor.
        virtual ~IndexBuffer();

        //! Allocate gpu 
        void allocateBuffer(RendererPtr renderer, MaterialPtr material, const ConstArrayAccessor1<unsigned int>& data, size_t numberOfIndices, bool storeData);

        //! Update 
        virtual void updateBuffer(RendererPtr renderer, MaterialPtr material, const ConstArrayAccessor1<unsigned int>& data, bool storeData) = 0;

        size_t getNumberOfIndices() const;

    protected:
        //! implementation of bind method
        virtual void onBind(RendererPtr renderer) = 0;
        
        //! implementation of unbind method
        virtual void onUnbind(RendererPtr renderer) = 0;

        //! implementation of destry method
        virtual void onDestroy(RendererPtr renderer) = 0;

        //! Allocate gpu 
        virtual void onAllocateBuffer(RendererPtr renderer, MaterialPtr material, const ConstArrayAccessor1<unsigned int>& data) = 0;

        Array1<unsigned int> _data;
        size_t _numberOfIndices;
    private:
    };

    using IndexBufferPtr = std::shared_ptr<IndexBuffer>;
} 
}

#endif