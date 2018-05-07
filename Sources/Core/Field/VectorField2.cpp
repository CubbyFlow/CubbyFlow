/*************************************************************************
> File Name: VectorField2.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for 2-D vector field.
> Created Time: 2017/03/14
> Copyright (c) 2018, Dongmin Kim
*************************************************************************/
#include <Core/Field/VectorField2.h>

namespace CubbyFlow
{
	VectorField2::VectorField2()
	{
		// Do nothing
	}

	VectorField2::~VectorField2()
	{
		// Do nothing
	}

	double VectorField2::Divergence(const Vector2D&) const
	{
		return 0.0;
	}

	double VectorField2::Curl(const Vector2D&) const
	{
		return 0.0;
	}

	std::function<Vector2D(const Vector2D&)> VectorField2::Sampler() const
	{
		const VectorField2* self = this;
		return [self](const Vector2D& x) -> Vector2D
		{
			return self->Sample(x);
		};
	}
}