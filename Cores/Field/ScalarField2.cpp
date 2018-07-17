/*************************************************************************
> File Name: ScalarField2.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for 2-D scalar field.
> Created Time: 2017/03/16
> Copyright (c) 2018, Dongmin Kim
*************************************************************************/
#include <Field/ScalarField2.h>

namespace CubbyFlow
{
	ScalarField2::ScalarField2()
	{
		// Do nothing
	}

	ScalarField2::~ScalarField2()
	{
		// Do nothing
	}

	Vector2D ScalarField2::Gradient(const Vector2D&) const
	{
		return Vector2D(0, 0);
	}

	double ScalarField2::Laplacian(const Vector2D&) const
	{
		return 0.0;
	}

	std::function<double(const Vector2D&)> ScalarField2::Sampler() const
	{
		const ScalarField2* self = this;
		return [self](const Vector2D& x) -> double
		{
			return self->Sample(x);
		};
	}
}