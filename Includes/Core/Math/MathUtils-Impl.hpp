// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_MATH_UTILS_IMPL_HPP
#define CUBBYFLOW_MATH_UTILS_IMPL_HPP

#include <Core/Utils/Constants.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>

namespace CubbyFlow
{
template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, bool> Similar(T x, T y, T eps)
{
    return std::abs(x - y) <= eps;
}

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T> Sign(T x)
{
    if (x >= 0)
    {
        return 1;
    }

    return -1;
}

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T> Min3(T x, T y, T z)
{
    return std::min(std::min(x, y), z);
}

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T> Max3(T x, T y, T z)
{
    return std::max(std::max(x, y), z);
}

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T> MinN(const T* x, size_t n)
{
    T m = x[0];

    for (size_t i = 1; i < n; i++)
    {
        m = std::min(m, x[i]);
    }

    return m;
}

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T> MaxN(const T* x, size_t n)
{
    T m = x[0];

    for (size_t i = 1; i < n; i++)
    {
        m = std::max(m, x[i]);
    }

    return m;
}

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T> AbsMin(T x, T y)
{
    return (x * x < y * y) ? x : y;
}

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T> AbsMax(T x, T y)
{
    return (x * x > y * y) ? x : y;
}

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T> AbsMinN(const T* x, size_t n)
{
    T m = x[0];

    for (size_t i = 1; i < n; i++)
    {
        m = AbsMin(m, x[i]);
    }

    return m;
}

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T> AbsMaxN(const T* x, size_t n)
{
    T m = x[0];

    for (size_t i = 1; i < n; i++)
    {
        m = AbsMax(m, x[i]);
    }

    return m;
}

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, size_t> ArgMin2(T x, T y)
{
    return (x < y) ? 0 : 1;
}

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, size_t> ArgMax2(T x, T y)
{
    return (x > y) ? 0 : 1;
}

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, size_t> ArgMin3(T x, T y, T z)
{
    if (x < y)
    {
        return (x < z) ? 0 : 2;
    }
    else
    {
        return (y < z) ? 1 : 2;
    }
}

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, size_t> ArgMax3(T x, T y, T z)
{
    if (x > y)
    {
        return (x > z) ? 0 : 2;
    }
    else
    {
        return (y > z) ? 1 : 2;
    }
}

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T> Square(T x)
{
    return x * x;
}

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T> Cubic(T x)
{
    return x * x * x;
}

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T> Clamp(T val, T low, T high)
{
    if (val < low)
    {
        return low;
    }

    if (val > high)
    {
        return high;
    }

    return val;
}

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T> DegreesToRadians(
    T angleInDegrees)
{
    return angleInDegrees * PI<T>() / 180;
}

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T> RadiansToDegrees(
    T angleInRadians)
{
    return angleInRadians * 180 / PI<T>();
}

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value> GetBarycentric(T x, size_t begin,
                                                              size_t end,
                                                              size_t& i, T& t)
{
    assert(end > begin);

    T s = std::floor(x);
    i = static_cast<size_t>(s);
    const size_t size = end - begin;

    if (size == 1)
    {
        i = begin;
        t = 0;
    }
    else if (i > end - 2)
    {
        i = end - 2;
        t = 1;
    }
    else
    {
        t = static_cast<T>(x - s);
    }
}

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value> GetBarycentric(T x, size_t end,
                                                              size_t& i, T& t)
{
    assert(end > 0);

    T s = std::floor(x);
    i = static_cast<size_t>(s);
    t = x - s;

    if (end == 1)
    {
        i = 0;
        t = 0;
    }
    else if (i > end - 2)
    {
        i = end - 2;
        t = 1;
    }
}

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value> GetBarycentric(T x,
                                                              ssize_t begin,
                                                              ssize_t end,
                                                              ssize_t& i, T& t)
{
    assert(end > begin);

    T s = std::floor(x);
    i = static_cast<ssize_t>(s);
    const ssize_t size = end - begin;

    if (size == 1 || i < 0)
    {
        i = begin;
        t = 0;
    }
    else if (i > end - 2)
    {
        i = end - 2;
        t = 1;
    }
    else
    {
        t = static_cast<T>(x - s);
    }
}

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value> GetBarycentric(T x, ssize_t end,
                                                              ssize_t& i, T& t)
{
    assert(end > 0);

    T s = std::floor(x);
    i = static_cast<ssize_t>(s);
    t = x - s;

    if (end == 1 || i < 0)
    {
        i = 0;
        t = 0;
    }
    else if (i > end - 2)
    {
        i = end - 2;
        t = 1;
    }
}

template <typename S, typename T>
std::enable_if_t<std::is_arithmetic<T>::value, S> Lerp(const S& f0, const S& f1,
                                                       T t)
{
    return (1 - t) * f0 + t * f1;
}

template <typename S, typename T>
std::enable_if_t<std::is_arithmetic<T>::value, S> BiLerp(
    const S& f00, const S& f10, const S& f01, const S& f11, T tx, T ty)
{
    return Lerp(Lerp(f00, f10, tx), Lerp(f01, f11, tx), ty);
}

template <typename S, typename T>
std::enable_if_t<std::is_arithmetic<T>::value, S> TriLerp(
    const S& f000, const S& f100, const S& f010, const S& f110, const S& f001,
    const S& f101, const S& f011, const S& f111, T tx, T ty, T tz)
{
    return Lerp(BiLerp(f000, f100, f010, f110, tx, ty),
                BiLerp(f001, f101, f011, f111, tx, ty), tz);
}

template <typename S, typename T>
std::enable_if_t<std::is_arithmetic<T>::value, S> CatmullRom(const S& f0,
                                                             const S& f1,
                                                             const S& f2,
                                                             const S& f3, T t)
{
    S d1 = (f2 - f0) / 2;
    S d2 = (f3 - f1) / 2;
    S D1 = f2 - f1;

    S a3 = d1 + d2 - 2 * D1;
    S a2 = 3 * D1 - 2 * d1 - d2;
    S a1 = d1;
    S a0 = f1;

    return a3 * Cubic(t) + a2 * Square(t) + a1 * t + a0;
}

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T> MonotonicCatmullRom(
    const T& f0, const T& f1, const T& f2, const T& f3, T t)
{
    T d1 = (f2 - f0) / 2;
    T d2 = (f3 - f1) / 2;
    T D1 = f2 - f1;

    if (std::fabs(D1) < std::numeric_limits<double>::epsilon())
    {
        d1 = d2 = 0;
    }

    if (Sign(D1) != Sign(d1))
    {
        d1 = 0;
    }

    if (Sign(D1) != Sign(d2))
    {
        d2 = 0;
    }

    T a3 = d1 + d2 - 2 * D1;
    T a2 = 3 * D1 - 2 * d1 - d2;
    T a1 = d1;
    T a0 = f1;

    return a3 * Cubic(t) + a2 * Square(t) + a1 * t + a0;
}
}  // namespace CubbyFlow

#endif