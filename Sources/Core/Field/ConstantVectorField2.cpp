/*************************************************************************
> File Name: ConstantVectorField2.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 2-D constant vector field.
> Created Time: 2017/04/01
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Field/ConstantVectorField2.h>
#include <Core/Utils/Macros.h>

namespace CubbyFlow
{
	ConstantVectorField2::ConstantVectorField2(const Vector2D& value) :
		m_value(value)
	{
		// Do nothing
	}

	// TODO: x is unused variable.
	Vector2D ConstantVectorField2::Sample(const Vector2D& x) const
	{
		UNUSED_VARIABLE(x);

		return m_value;
	}

	std::function<Vector2D(const Vector2D&)> ConstantVectorField2::Sampler() const
	{
		return [this](const Vector2D&) -> Vector2D
		{
			return m_value;
		};
	}

	ConstantVectorField2::Builder ConstantVectorField2::GetBuilder()
	{
		return Builder();
	}

	ConstantVectorField2::Builder& ConstantVectorField2::Builder::WithValue(const Vector2D& value)
	{
		m_value = value;
		return *this;
	}

	ConstantVectorField2 ConstantVectorField2::Builder::Build() const
	{
		return ConstantVectorField2(m_value);
	}

	ConstantVectorField2Ptr ConstantVectorField2::Builder::MakeShared() const
	{
		return std::shared_ptr<ConstantVectorField2>(
			new ConstantVectorField2(m_value),
			[](ConstantVectorField2* obj)
		{
			delete obj;
		});
	}
}
