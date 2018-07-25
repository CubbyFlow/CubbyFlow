/*************************************************************************
> File Name: ConstantScalarField3.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D constant scalar field.
> Created Time: 2017/04/01
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Field/ConstantScalarField3.h>
#include <Core/Utils/Macros.h>

namespace CubbyFlow
{
	ConstantScalarField3::ConstantScalarField3(double value) :
		m_value(value)
	{
		// Do nothing
	}

	double ConstantScalarField3::Sample(const Vector3D& x) const
	{
		UNUSED_VARIABLE(x);

		return m_value;
	}

	std::function<double(const Vector3D&)> ConstantScalarField3::Sampler() const
	{
		double value = m_value;

		return [value](const Vector3D&) -> double
		{
			return value;
		};
	}

	ConstantScalarField3::Builder ConstantScalarField3::GetBuilder()
	{
		return Builder();
	}

	ConstantScalarField3::Builder& ConstantScalarField3::Builder::WithValue(double value)
	{
		m_value = value;
		return *this;
	}

	ConstantScalarField3 ConstantScalarField3::Builder::Build() const
	{
		return ConstantScalarField3(m_value);
	}

	ConstantScalarField3Ptr ConstantScalarField3::Builder::MakeShared() const
	{
		return std::shared_ptr<ConstantScalarField3>(
			new ConstantScalarField3(m_value),
			[](ConstantScalarField3* obj)
		{
			delete obj;
		});
	}
}
