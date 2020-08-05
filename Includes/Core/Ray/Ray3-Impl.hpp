// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_RAY3_IMPL_HPP
#define CUBBYFLOW_RAY3_IMPL_HPP

namespace CubbyFlow
{
template <typename T>
Ray<T, 3>::Ray() : Ray(Vector3<T>(), Vector3<T>(1, 0, 0))
{
    // Do nothing
}

template <typename T>
Ray<T, 3>::Ray(const Vector3<T>& newOrigin, const Vector3<T>& newDirection)
    : origin(newOrigin), direction(newDirection.Normalized())
{
    // Do nothing
}

template <typename T>
Ray<T, 3>::Ray(const Ray& other)
    : origin(other.origin), direction(other.direction)
{
    // Do nothing
}

template <typename T>
Ray<T, 3>& Ray<T, 3>::operator=(const Ray& other)
{
    origin = other.origin;
    direction = other.direction;

    return *this;
}

template <typename T>
Vector3<T> Ray<T, 3>::PointAt(T t) const
{
    return origin + t * direction;
}
}  // namespace CubbyFlow

#endif