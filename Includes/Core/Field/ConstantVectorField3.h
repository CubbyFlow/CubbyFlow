/*************************************************************************
> File Name: ConstantVectorField3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D constant vector field.
> Created Time: 2017/04/01
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_CONSTANT_VECTOR_FIELD3_H
#define CUBBYFLOW_CONSTANT_VECTOR_FIELD3_H

#include <Core/Field/VectorField3.h>

namespace CubbyFlow
{
	//! 3-D constant vector field.
	class ConstantVectorField3 final : public VectorField3
	{
	public:
		class Builder;

		//! Constructs a constant vector field with given \p value.
		explicit ConstantVectorField3(const Vector3D& value);

		//! Returns the sampled value at given position \p x.
		Vector3D Sample(const Vector3D& x) const override;

		//! Returns the sampler function.
		std::function<Vector3D(const Vector3D&)> Sampler() const override;

		//! Returns builder fox ConstantVectorField3.
		static Builder GetBuilder();

	private:
		Vector3D m_value;
	};

	//! Shared pointer for the ConstantVectorField3 type.
	using ConstantVectorField3Ptr = std::shared_ptr<ConstantVectorField3>;


	//!
	//! \brief Front-end to create ConstantVectorField3 objects step by step.
	//!
	class ConstantVectorField3::Builder final
	{
	public:
		//! Returns builder with value.
		Builder& WithValue(const Vector3D& value);

		//! Builds ConstantVectorField3.
		ConstantVectorField3 Build() const;

		//! Builds shared pointer of ConstantVectorField3 instance.
		ConstantVectorField3Ptr MakeShared() const;

	private:
		Vector3D m_value{ 0, 0, 0 };
	};
}

#endif