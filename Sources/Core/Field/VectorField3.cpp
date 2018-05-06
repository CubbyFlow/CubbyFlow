/*************************************************************************
> File Name: VectorField3.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: Abstract base class for 3-D vector field.
> Created Time: 2017/03/14
> Copyright (c) 2018, Dongmin Kim
*************************************************************************/
#include <Core/Field/VectorField3.h>

namespace CubbyFlow
{
	VectorField3::VectorField3()
	{
		// Do nothing
	}

	VectorField3::~VectorField3()
	{
		// Do nothing
	}

	double VectorField3::Divergence(const Vector3D&) const
	{
		return 0.0;
	}

	Vector3D VectorField3::Curl(const Vector3D&) const
	{
		return Vector3D(0, 0, 0);
	}

	std::function<Vector3D(const Vector3D&)> VectorField3::Sampler() const
	{
		const VectorField3* self = this;
		return [self](const Vector3D& x) -> Vector3D
		{
			return self->Sample(x);
		};
	}
}