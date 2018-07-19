/*************************************************************************
> File Name: Ray3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Class for 3-D ray.
> Created Time: 2017/03/21
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_RAY3_H
#define CUBBYFLOW_RAY3_H

#include <Ray/Ray.h>
#include <Vector/Vector3.h>

namespace CubbyFlow
{
	//!
	//! \brief      Class for 3-D ray.
	//!
	//! \tparam     T     The value type.
	//!
	template <typename T>
	class Ray<T, 3> final
	{
	public:
		static_assert(std::is_floating_point<T>::value, "Ray only can be instantiated with floating point types");

		//! The origin of the ray.
		Vector3<T> origin;

		//! The direction of the ray.
		Vector3<T> direction;

		//! Constructs an empty ray that points (1, 0, 0) from (0, 0, 0).
		Ray();

		//! Constructs a ray with given origin and direction.
		Ray(const Vector3<T>& newOrigin, const Vector3<T>& newDirection);

		//! Copy constructor.
		Ray(const Ray& other);

		//! Returns a point on the ray at distance \p t.
		Vector3<T> PointAt(T t) const;
	};

	//! Type alias for 3-D ray.
	template <typename T> using Ray3 = Ray<T, 3>;

	//! Float-type 3-D ray.
	using Ray3F = Ray3<float>;

	//! Double-type 3-D ray.
	using Ray3D = Ray3<double>;
}

#include <Ray/Ray3-Impl.h>

#endif