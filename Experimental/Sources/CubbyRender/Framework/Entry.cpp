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
#include <Framework/VertexBuffer.h>
#include <Framework/IndexBuffer.h>
#include <Framework/Material.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    Entry::Entry()
    {
        //! Do nothing.
    }

    Entry::Entry(VertexBufferPtr vertices, IndexBufferPtr indices, MaterialPtr material, PrimitiveType type, unsigned short blendOrder)
          : _vertices(vertices), _indices(indices), _material(material), _type(type), _blendOrder(blendOrder)
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

    void Entry::destroy(RendererPtr renderer)
    {
        _vertices->destroy(renderer);
        _indices->destroy(renderer);
        _material->destroy(renderer);
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
        return Entry(_vertices, _indices, _material, _type, _blendOrder);
    }

	EntryPtr Entry::Builder::MakeShared() const
    {
        return std::shared_ptr<Entry>(new Entry(_vertices, _indices, _material, _type, _blendOrder),
			[](Entry* obj)
		{
			delete obj;
		});
    }
} 
}