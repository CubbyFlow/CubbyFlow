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
        Entry(VertexBufferPtr vertices, IndexBufferPtr indices, MaterialPtr material, PrimitiveType type, unsigned short blendOrder);

        //! Default Destructor
        ~Entry();

        //! Returns builder fox Entry.
		static Builder GetBuilder();

        void destroy();
    protected:

    private:
        VertexBufferPtr _vertices = nullptr;
        IndexBufferPtr _indices = nullptr;
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
        MaterialPtr _material = nullptr;
        PrimitiveType _type = PrimitiveType::TriangleStrip;
        unsigned short _blendOrder = 0;
	};
} 
}

#endif