// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_RAY3_HPP
#define CUBBYFLOW_RAY3_HPP

#include <Core/Ray/Ray.hpp>
#include <Core/Vector/Vector3.hpp>

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
    static_assert(std::is_floating_point<T>::value,
                  "Ray only can be instantiated with floating point types");

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

    //! Copy assignment operator.
    Ray& operator=(const Ray& other);

    //! Returns a point on the ray at distance \p t.
    Vector3<T> PointAt(T t) const;
};

//! Type alias for 3-D ray.
template <typename T>
using Ray3 = Ray<T, 3>;

//! Float-type 3-D ray.
using Ray3F = Ray3<float>;

//! Double-type 3-D ray.
using Ray3D = Ray3<double>;
}  // namespace CubbyFlow

#include <Core/Ray/Ray3-Impl.hpp>

#endif