// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_FUNCTORS_IMPL_HPP
#define CUBBYFLOW_FUNCTORS_IMPL_HPP

#include <Core/Math/MathUtils.hpp>

#include <cmath>

namespace CubbyFlow
{
template <typename T>
constexpr T NoOp<T>::operator()(const T& a) const
{
    return a;
}

template <typename T, typename U>
constexpr U TypeCast<T, U>::operator()(const T& a) const
{
    return static_cast<U>(a);
}

template <typename T>
constexpr T DoCeil<T>::operator()(const T& a) const
{
    return std::ceil(a);
}

template <typename T>
constexpr T DoFloor<T>::operator()(const T& a) const
{
    return std::floor(a);
}

template <typename T>
constexpr T RMinus<T>::operator()(const T& a, const T& b) const
{
    return b - a;
}

template <typename T>
constexpr T RDivides<T>::operator()(const T& a, const T& b) const
{
    return b / a;
}

template <typename T>
constexpr T DoMin<T>::operator()(const T& a, const T& b) const
{
    return std::min(a, b);
}

template <typename T>
constexpr T DoMax<T>::operator()(const T& a, const T& b) const
{
    return std::max(a, b);
}

template <typename T>
constexpr T DoAbsMin<T>::operator()(const T& a, const T& b) const
{
    return AbsMin(a, b);
}

template <typename T>
constexpr T DoAbsMax<T>::operator()(const T& a, const T& b) const
{
    return AbsMax(a, b);
}

template <typename T>
constexpr bool SimilarTo<T>::operator()(const T& a, const T& b) const
{
    return std::fabs(a - b) <= tolerance;
}

template <typename T>
constexpr T DoClamp<T>::operator()(const T& a, const T& low,
                                   const T& high) const
{
    return Clamp(a, low, high);
}
}  // namespace CubbyFlow

#endif