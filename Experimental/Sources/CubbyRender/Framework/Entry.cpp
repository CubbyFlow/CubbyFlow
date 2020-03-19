/*************************************************************************
> File Name: Entry.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for Renderer object.
> Created Time: 2020/03/16
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#include <Framework/Entry.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    Entry::Entry()
    {
        //! Do nothing.
    }

    Entry::Entry(VertexBufferPtr vertices, IndexBufferPtr indices, size_t offset, size_t minIndex, size_t maxIndex, size_t count,
                 MaterialPtr material, PrimitiveType type, unsigned short blendOrder)
          : _vertices(vertices), _indices(indices), _offset(offset), _minIndex(minIndex),
            _maxIndex(maxIndex), _count(count), _material(material), _type(type), _blendOrder(blendOrder)
    {
        //! Do nothing
    }

    Entry::~Entry()
    {
        //! Do nothing.
    }

    Entry::Builder Entry::GetBuilder()
    {
        return Builder();
    }

	Entry::Builder& Entry::Builder::WithVertexBuffer(VertexBufferPtr vertices)
    {
        _vertices = vertices;
        return static_cast<Entry::Builder&>(*this);
    }

	Entry::Builder& Entry::Builder::WithIndexBuffer(IndexBufferPtr indices)
    {
        _indices = indices;
        return static_cast<Entry::Builder&>(*this);
    }

	Entry::Builder& Entry::Builder::WithOffset(size_t offset)
    {
        _offset = offset;
        return static_cast<Entry::Builder&>(*this);
    }

	Entry::Builder& Entry::Builder::WithMinIndex(size_t minIndex)
    {
        _minIndex = minIndex;
        return static_cast<Entry::Builder&>(*this);
    }

	Entry::Builder& Entry::Builder::WithMaxIndex(size_t maxIndex)
    {
        _maxIndex = maxIndex;
        return static_cast<Entry::Builder&>(*this);
    }

	Entry::Builder& Entry::Builder::WithCount(size_t count)
    {
        _count = count;
        return static_cast<Entry::Builder&>(*this);
    }

	Entry::Builder& Entry::Builder::WithMaterial(MaterialPtr material)
    {
        _material = material;
        return static_cast<Entry::Builder&>(*this);
    }

	Entry::Builder& Entry::Builder::WithPrimitiveType(PrimitiveType type)
    {
        _type = type;
        return static_cast<Entry::Builder&>(*this);
    }

	Entry::Builder& Entry::Builder::WithBlendOrder(unsigned short blendOrder)
    {
        _blendOrder = blendOrder;
        return static_cast<Entry::Builder&>(*this);
    }


	Entry Entry::Builder::Build() const
    {
        return Entry(_vertices, _indices, _offset, _minIndex, _maxIndex, 
                     _count, _material, _type, _blendOrder);
    }

	EntryPtr Entry::Builder::MakeShared() const
    {
        return std::shared_ptr<Entry>(new Entry(_vertices, _indices, _offset, _minIndex, _maxIndex, 
                                                _count, _material, _type, _blendOrder),
			[](Entry* obj)
		{
			delete obj;
		});
    }
} 
}