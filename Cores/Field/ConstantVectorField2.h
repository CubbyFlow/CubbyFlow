/*************************************************************************
> File Name: ConstantVectorField2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D constant vector field.
> Created Time: 2017/04/01
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_CONSTANT_VECTOR_FIELD2_H
#define CUBBYFLOW_CONSTANT_VECTOR_FIELD2_H

#include <Field/VectorField2.h>

namespace CubbyFlow
{
	//! 2-D constant vector field.
	class ConstantVectorField2 final : public VectorField2
	{
	public:
		class Builder;

		//! Constructs a constant vector field with given \p value.
		explicit ConstantVectorField2(const Vector2D& value);

		//! Returns the sampled value at given position \p x.
		Vector2D Sample(const Vector2D& x) const override;

		//! Returns the sampler function.
		std::function<Vector2D(const Vector2D&)> Sampler() const override;

		//! Returns builder fox ConstantVectorField2.
		static Builder GetBuilder();

	private:
		Vector2D m_value;
	};

	//! Shared pointer for the ConstantVectorField2 type.
	using ConstantVectorField2Ptr = std::shared_ptr<ConstantVectorField2>;


	//!
	//! \brief Front-end to create ConstantVectorField2 objects step by step.
	//!
	class ConstantVectorField2::Builder final
	{
	public:
		//! Returns builder with value.
		Builder& WithValue(const Vector2D& value);

		//! Builds ConstantVectorField2.
		ConstantVectorField2 Build() const;

		//! Builds shared pointer of ConstantVectorField2 instance.
		ConstantVectorField2Ptr MakeShared() const;

	private:
		Vector2D m_value{ 0, 0 };
	};
}

#endif