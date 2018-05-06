/*************************************************************************
> File Name: Ray2-Impl.h
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: Class for 2-D ray.
> Created Time: 2017/03/25
> Copyright (c) 2018, Dongmin Kim
*************************************************************************/
#ifndef CUBBYFLOW_RAY2_IMPL_H
#define CUBBYFLOW_RAY2_IMPL_H

namespace CubbyFlow
{
	template <typename T>
	Ray<T, 2>::Ray() :
		Ray(Vector2<T>(), Vector2<T>(1, 0))
	{
		// Do nothing
	}

	template <typename T>
	Ray<T, 2>::Ray(const Vector2<T>& newOrigin, const Vector2<T>& newDirection) :
		origin(newOrigin), direction(newDirection.Normalized())
	{
		// Do nothing
	}

	template <typename T>
	Ray<T, 2>::Ray(const Ray& other) :
		origin(other.origin), direction(other.direction)
	{
		// Do nothing
	}

	template <typename T>
	Vector2<T> Ray<T, 2>::PointAt(T t) const
	{
		return origin + t * direction;
	}
}

#endif