/*************************************************************************
> File Name: ShaderParameter.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Parameters collector for shader
> Created Time: 2020/02/22
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_SHADERPARAMETER_H
#define CUBBYFLOW_SHADERPARAMETER_H

#include <memory>
#include <unordered_map>

namespace CubbyFlow {
namespace CubbyRender {

    class ShaderParameter final
    {
    public:
        //! Default Constructor
        ShaderParameter();

        //! Default Destructor
        ~ShaderParameter();

        struct Metadata;
        struct Builder;
    protected:
    private:
        std::unordered_map<std::string, Metadata> _params;
    };

    using ShaderParameterPtr = std::shared_ptr<ShaderParameter>;

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