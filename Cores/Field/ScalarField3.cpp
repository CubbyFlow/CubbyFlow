/*************************************************************************
> File Name: ScalarField3.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for 3-D scalar field.
> Created Time: 2017/03/16
> Copyright (c) 2018, Dongmin Kim
*************************************************************************/
#include <Field/ScalarField3.h>

namespace CubbyFlow
{
	ScalarField3::ScalarField3()
	{
		// Do nothing
	}

	ScalarField3::~ScalarField3()
	{
		// Do nothing
	}

	Vector3D ScalarField3::Gradient(const Vector3D&) const
	{
		return Vector3D(0, 0, 0);
	}

	double ScalarField3::Laplacian(const Vector3D&) const
	{
		return 0.0;
	}

	std::function<double(const Vector3D&)> ScalarField3::Sampler() const
	{
		const ScalarField3* self = this;
		return [self](const Vector3D& x) -> double
		{
			return self->Sample(x);
		};
	}
}