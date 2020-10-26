// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_RAY2_HPP
#define CUBBYFLOW_RAY2_HPP

#include <Core/Ray/Ray.hpp>
#include <Core/Vector/Vector2.hpp>

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
    static_assert(std::is_floating_point<T>::value,
                  "Ray only can be instantiated with floating point types");

    //! Constructs an empty ray that points (1, 0) from (0, 0).
    Ray();

    //! Constructs a ray with given origin and direction.
    Ray(const Vector2<T>& newOrigin, const Vector2<T>& newDirection);

    //! Copy constructor.
    Ray(const Ray& other);

    //! Move constructor.
    Ray(Ray&& other) noexcept;

    //! Default destructor.
    ~Ray() = default;

    //! Copy assignment operator.
    Ray& operator=(const Ray& other);

    //! Move assignment operator.
    Ray& operator=(Ray&& other) noexcept;

    //! Returns a point on the ray at distance \p t.
    Vector2<T> PointAt(T t) const;

    //! The origin of the ray.
    Vector2<T> origin;

    //! The direction of the ray.
    Vector2<T> direction;
};

//! Type alias for 2-D ray.
template <typename T>
using Ray2 = Ray<T, 2>;

//! Float-type 2-D ray.
using Ray2F = Ray2<float>;

//! Double-type 2-D ray.
using Ray2D = Ray2<double>;
}  // namespace CubbyFlow

#include <Core/Ray/Ray2-Impl.hpp>

#endif