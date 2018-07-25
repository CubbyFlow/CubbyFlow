/*************************************************************************
> File Name: ConstantScalarField2.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D constant scalar field.
> Created Time: 2017/04/01
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Field/ConstantScalarField2.h>
#include <Core/Utils/Macros.h>

namespace CubbyFlow
{
	ConstantScalarField2::ConstantScalarField2(double value) :
		m_value(value)
	{
		// Do nothing
	}

	double ConstantScalarField2::Sample(const Vector2D& x) const
	{
		UNUSED_VARIABLE(x);

		return m_value;
	}

	std::function<double(const Vector2D&)> ConstantScalarField2::Sampler() const
	{
		double value = m_value;
		
		return [value](const Vector2D&) -> double
		{
			return value;
		};
	}

	ConstantScalarField2::Builder ConstantScalarField2::GetBuilder()
	{
		return Builder();
	}
	
	ConstantScalarField2::Builder& ConstantScalarField2::Builder::WithValue(double value)
	{
		m_value = value;
		return *this;
	}

	ConstantScalarField2 ConstantScalarField2::Builder::Build() const
	{
		return ConstantScalarField2(m_value);
	}

	ConstantScalarField2Ptr ConstantScalarField2::Builder::MakeShared() const
	{
		return std::shared_ptr<ConstantScalarField2>(
			new ConstantScalarField2(m_value),
			[](ConstantScalarField2* obj) 
		{
			delete obj;
		});
	}
}
