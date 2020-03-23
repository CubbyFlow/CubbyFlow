/*************************************************************************
> File Name: Entry.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for Renderer object.
> Created Time: 2020/02/15
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_ENTRY_H
#define CUBBYFLOW_ENTRY_H

#include <Framework/Prerequisites.h>
#include <Framework/RenderOptions.h>
#include <memory>

namespace CubbyFlow {
namespace CubbyRender {
    
    //!
    //! \brief Abstract base class for Renderer object.
    //!
    //! This class contains basic interface for renderer. 
    //! The wrapper class of each Graphics API must overrides 
    //! this renderer class.
    //!
    class Entry
    {
        friend class Rendererable;
    public:
        class Builder;

        //! Default Constructor
        Entry();

        //! Constructor with parameteres
        Entry(VertexBufferPtr vertices, IndexBufferPtr indices, size_t offset, size_t minIndex, size_t maxIndex, size_t count,
              MaterialPtr material, PrimitiveType type, unsigned short blendOrder);

        //! Default Destructor
        ~Entry();

        //! Returns builder fox Entry.
		static Builder GetBuilder();
    protected:

    private:
        VertexBufferPtr _vertices = nullptr;
        IndexBufferPtr _indices = nullptr;
        size_t _offset = 0;
        size_t _minIndex = 0;
        size_t _maxIndex = 0;
        size_t _count = 0;
        MaterialPtr _material = nullptr;
        PrimitiveType _type = PrimitiveType::TriangleStrip;
        unsigned short _blendOrder = 0;
    };

    using EntryPtr = std::shared_ptr<Entry>;

	class Entry::Builder
	{
	public:
		//! Returns builder with vertex buffer
		Entry::Builder& WithVertexBuffer(VertexBufferPtr vertices);
        //! Returns builder with index buffer
		Entry::Builder& WithIndexBuffer(IndexBufferPtr indices);
        //! Returns builder with offset
		Entry::Builder& WithOffset(size_t offset);
        //! Returns builder with min index
		Entry::Builder& WithMinIndex(size_t minIndex);
        //! Returns builder with max index
		Entry::Builder& WithMaxIndex(size_t maxIndex);
        //! Returns builder with count
		Entry::Builder& WithCount(size_t count);
        //! Returns builder with material
		Entry::Builder& WithMaterial(MaterialPtr material);
        //! Returns builder with primitive type
		Entry::Builder& WithPrimitiveType(PrimitiveType type);
        //! Returns builder with blend order
		Entry::Builder& WithBlendOrder(unsigned short blendOrder);
		//! Builds  Entry.
		Entry Build() const;
		//! Builds shared pointer of Entry instance.
		EntryPtr MakeShared() const;
	protected:
        VertexBufferPtr _vertices = nullptr;
        IndexBufferPtr _indices = nullptr;
        size_t _offset = 0;
        size_t _minIndex = 0;
        size_t _maxIndex = 0;
        size_t _count = 0;
        MaterialPtr _material = nullptr;
        PrimitiveType _type = PrimitiveType::TriangleStrip;
        unsigned short _blendOrder = 0;
	};
} 
}

#endif