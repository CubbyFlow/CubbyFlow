/*************************************************************************
> File Name: ImplicitSurface2.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Abstract base class for 2-D implicit surface.
> Created Time: 2017/04/16
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Surface/ImplicitSurface2.h>

namespace CubbyFlow
{
	ImplicitSurface2::ImplicitSurface2(const Transform2& transform, bool isNormalFlipped) :
		Surface2(transform, isNormalFlipped)
	{
		// Do nothing
	}

	ImplicitSurface2::ImplicitSurface2(const ImplicitSurface2& other) :
		Surface2(other)
	{
		// Do nothing
	}

	ImplicitSurface2::~ImplicitSurface2()
	{
		// Do nothing
	}

	double ImplicitSurface2::SignedDistance(const Vector2D& otherPoint) const
	{
		return SignedDistanceLocal(transform.ToLocal(otherPoint));
	}

	double ImplicitSurface2::ClosestDistanceLocal(const Vector2D& otherPoint) const
	{
		return std::fabs(SignedDistanceLocal(otherPoint));
	}
}