/*************************************************************************
> File Name: ImplicitSurface3.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: Abstract base class for 3-D implicit surface.
> Created Time: 2017/04/18
> Copyright (c) 2018, Dongmin Kim
*************************************************************************/
#include <Core/Surface/ImplicitSurface3.h>

namespace CubbyFlow
{
	ImplicitSurface3::ImplicitSurface3(const Transform3& transform, bool isNormalFlipped) :
		Surface3(transform, isNormalFlipped)
	{
		// Do nothing
	}

	ImplicitSurface3::ImplicitSurface3(const ImplicitSurface3& other) :
		Surface3(other)
	{
		// Do nothing
	}

	ImplicitSurface3::~ImplicitSurface3()
	{
		// Do nothing
	}

	double ImplicitSurface3::SignedDistance(const Vector3D& otherPoint) const
	{
		return SignedDistanceLocal(transform.ToLocal(otherPoint));
	}

	double ImplicitSurface3::ClosestDistanceLocal(const Vector3D& otherPoint) const
	{
		return std::fabs(SignedDistanceLocal(otherPoint));
	}
}