// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_RAY_HPP
#define CUBBYFLOW_RAY_HPP

#include <Core/Matrix/Matrix.hpp>

namespace CubbyFlow
{
//!
//! \brief Class for N-D ray.
//!
//! \tparam T   The value type.
//! \tparam N   Dimension.
//!
template <typename T, size_t N>
class Ray final
{
 public:
    static_assert(N > 0, "Dimension should be greater than 0");
    static_assert(std::is_floating_point<T>::value,
                  "Ray only can be instantiated with floating point types");

    using VectorType = Vector<T, N>;

    //! The origin of the ray.
    VectorType origin;

    //! The direction of the ray.
    VectorType direction;

    //! Constructs an empty ray that points (1, 0, ...) from (0, 0, ...).
    Ray();

    //! Constructs a ray with given origin and direction.
    Ray(const VectorType& newOrigin, const VectorType& newDirection);

    //! Default destructor.
    ~Ray() = default;

    //! Copy constructor.
    Ray(const Ray& other);

    //! Move constructor.
    Ray(Ray&& other) noexcept;

    //! Copy assignment operator.
    Ray& operator=(const Ray& other);

    //! Move assignment operator.
    Ray& operator=(Ray&& other) noexcept;

    //! Returns a point on the ray at distance \p t.
    [[nodiscard]] VectorType PointAt(T t) const;
};

template <typename T>
using Ray2 = Ray<T, 2>;

template <typename T>
using Ray3 = Ray<T, 3>;

using Ray2F = Ray2<float>;

using Ray2D = Ray2<double>;

using Ray3F = Ray3<float>;

using Ray3D = Ray3<double>;
}  // namespace CubbyFlow

#include <Core/Geometry/Ray-Impl.hpp>

#endif