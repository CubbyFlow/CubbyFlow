/*************************************************************************
> File Name: Ray3-Impl.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Class for 3-D ray.
> Created Time: 2017/03/25
> Copyright (c) 2018, Dongmin Kim
*************************************************************************/
#ifndef CUBBYFLOW_RAY3_IMPL_H
#define CUBBYFLOW_RAY3_IMPL_H

namespace CubbyFlow
{
	template <typename T>
	Ray<T, 3>::Ray() :
		Ray(Vector3<T>(), Vector3<T>(1, 0, 0))
	{
		// Do nothing
	}

	template <typename T>
	Ray<T, 3>::Ray(const Vector3<T>& newOrigin, const Vector3<T>& newDirection) :
		origin(newOrigin), direction(newDirection.Normalized())
	{
		// Do nothing
	}

	template <typename T>
	Ray<T, 3>::Ray(const Ray& other) :
		origin(other.origin), direction(other.direction)
	{
		// Do nothing
	}

	template <typename T>
	Vector3<T> Ray<T, 3>::PointAt(T t) const
	{
		return origin + t * direction;
	}
}

#endif