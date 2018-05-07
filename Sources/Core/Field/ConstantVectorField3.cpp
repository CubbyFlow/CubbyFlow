/*************************************************************************
> File Name: ConstantVectorField3.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D constant vector field.
> Created Time: 2017/04/01
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Field/ConstantVectorField3.h>
#include <Core/Utils/Macros.h>

namespace CubbyFlow
{
	ConstantVectorField3::ConstantVectorField3(const Vector3D& value) :
		m_value(value)
	{
		// Do nothing
	}

	Vector3D ConstantVectorField3::Sample(const Vector3D& x) const
	{
		UNUSED_VARIABLE(x);

		return m_value;
	}

	std::function<Vector3D(const Vector3D&)> ConstantVectorField3::Sampler() const
	{
		return [this](const Vector3D&) -> Vector3D
		{
			return m_value;
		};
	}

	ConstantVectorField3::Builder ConstantVectorField3::GetBuilder()
	{
		return Builder();
	}

	ConstantVectorField3::Builder& ConstantVectorField3::Builder::WithValue(const Vector3D& value)
	{
		m_value = value;
		return *this;
	}

	ConstantVectorField3 ConstantVectorField3::Builder::Build() const
	{
		return ConstantVectorField3(m_value);
	}

	ConstantVectorField3Ptr ConstantVectorField3::Builder::MakeShared() const
	{
		return std::shared_ptr<ConstantVectorField3>(
			new ConstantVectorField3(m_value),
			[](ConstantVectorField3* obj)
		{
			delete obj;
		});
	}
}
