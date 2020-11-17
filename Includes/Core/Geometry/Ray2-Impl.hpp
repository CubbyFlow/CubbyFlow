// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_RAY2_IMPL_HPP
#define CUBBYFLOW_RAY2_IMPL_HPP

namespace CubbyFlow
{
template <typename T>
Ray<T, 2>::Ray() : Ray{ Vector2<T>{}, Vector2<T>(1, 0) }
{
    // Do nothing
}

template <typename T>
Ray<T, 2>::Ray(const Vector2<T>& newOrigin, const Vector2<T>& newDirection)
    : origin(newOrigin), direction(newDirection.Normalized())
{
    // Do nothing
}

template <typename T>
Ray<T, 2>::Ray(const Ray& other)
    : origin(other.origin), direction(other.direction)
{
    // Do nothing
}

template <typename T>
Ray<T, 2>::Ray(Ray&& other) noexcept
    : origin(other.origin), direction(other.direction)
{
    // Do nothing
}

template <typename T>
Ray<T, 2>& Ray<T, 2>::operator=(const Ray& other)
{
    origin = other.origin;
    direction = other.direction;

    return *this;
}

template <typename T>
Ray<T, 2>& Ray<T, 2>::operator=(Ray&& other) noexcept
{
    origin = other.origin;
    direction = other.direction;

    return *this;
}

template <typename T>
Vector2<T> Ray<T, 2>::PointAt(T t) const
{
    return origin + t * direction;
}
}  // namespace CubbyFlow

#endif