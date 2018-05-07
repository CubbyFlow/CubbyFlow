/*************************************************************************
> File Name: ConstantScalarField2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D constant scalar field.
> Created Time: 2017/04/01
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_CONSTANT_SCALAR_FIELD2_H
#define CUBBYFLOW_CONSTANT_SCALAR_FIELD2_H

#include <Core/Field/ScalarField2.h>

namespace CubbyFlow
{
	//! 2-D constant scalar field.
	class ConstantScalarField2 final : public ScalarField2
	{
	public:
		class Builder;

		//! Constructs a constant scalar field with given \p value.
		explicit ConstantScalarField2(double value);

		//! Returns the sampled value at given position \p x.
		double Sample(const Vector2D& x) const override;

		//! Returns the sampler function.
		std::function<double(const Vector2D&)> Sampler() const override;

		//! Returns builder fox ConstantScalarField2.
		static Builder GetBuilder();

	private:
		double m_value = 0.0;
	};

	//! Shared pointer for the ConstantScalarField2 type.
	using ConstantScalarField2Ptr = std::shared_ptr<ConstantScalarField2>;

	//!
	//! \brief Front-end to create ConstantScalarField2 objects step by step.
	//!
	class ConstantScalarField2::Builder final
	{
	public:
		//! Returns builder with value.
		Builder& WithValue(double value);

		//! Builds ConstantScalarField2.
		ConstantScalarField2 Build() const;

		//! Builds shared pointer of ConstantScalarField2 instance.
		ConstantScalarField2Ptr MakeShared() const;

	private:
		double m_value = 0.0;
	};
}

#endif