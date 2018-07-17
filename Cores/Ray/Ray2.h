/*************************************************************************
> File Name: Ray2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Class for 2-D ray.
> Created Time: 2017/03/21
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_RAY2_H
#define CUBBYFLOW_RAY2_H

#include <Ray/Ray.h>
#include <Vector/Vector2.h>

namespace CubbyFlow
{
	//!
	//! \brief      Class for 2-D ray.
	//!
	//! \tparam     T     The value type.
	//!
	template <typename T>
	class Ray<T, 2> final
	{
	public:
		static_assert(std::is_floating_point<T>::value, "Ray only can be instantiated with floating point types");

		//! The origin of the ray.
		Vector2<T> origin;

		//! The direction of the ray.
		Vector2<T> direction;

		//! Constructs an empty ray that points (1, 0) from (0, 0).
		Ray();

		//! Constructs a ray with given origin and direction.
		Ray(const Vector2<T>& newOrigin, const Vector2<T>& newDirection);

		//! Copy constructor.
		Ray(const Ray& other);

		//! Returns a point on the ray at distance \p t.
		Vector2<T> PointAt(T t) const;
	};

	//! Type alias for 2-D ray.
	template <typename T> using Ray2 = Ray<T, 2>;

	//! Float-type 2-D ray.
	using Ray2F = Ray2<float>;

	//! Double-type 2-D ray.
	using Ray2D = Ray2<double>;
}

#include <Ray/Ray2-Impl.h>

#endif