// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_POINT3_IMPL_HPP
#define CUBBYFLOW_POINT3_IMPL_HPP

#include <Core/Point/Point2.hpp>

#include <cassert>
#include <cmath>

namespace CubbyFlow
{
template <typename T>
template <typename U>
Point<T, 3>::Point(const std::initializer_list<U>& list)
{
    Set(list);
}

template <typename T>
Point<T, 3>& Point<T, 3>::operator=(const Point& v)
{
    Set(v);
    return (*this);
}

template <typename T>
Point<T, 3>& Point<T, 3>::operator=(Point&& v) noexcept
{
    Set(v);
    return (*this);
}

template <typename T>
void Point<T, 3>::Set(T s)
{
    x = s;
    y = s;
    z = s;
}

template <typename T>
void Point<T, 3>::Set(T newX, T newY, T newZ)
{
    x = newX;
    y = newY;
    z = newZ;
}

template <typename T>
void Point<T, 3>::Set(const Point2<T>& pt, T newZ)
{
    x = pt.x;
    y = pt.y;
    z = newZ;
}

template <typename T>
template <typename U>
void Point<T, 3>::Set(const std::initializer_list<U>& list)
{
    assert(list.size() >= 3);

    auto inputElem = list.begin();
    x = static_cast<T>(*inputElem);
    y = static_cast<T>(*(++inputElem));
    z = static_cast<T>(*(++inputElem));
}

template <typename T>
void Point<T, 3>::Set(const Point& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
}

template <typename T>
void Point<T, 3>::SetZero()
{
    x = y = z = 0;
}

template <typename T>
Point<T, 3> Point<T, 3>::Add(T v) const
{
    return Point{ x + v, y + v, z + v };
}

template <typename T>
Point<T, 3> Point<T, 3>::Add(const Point& v) const
{
    return Point{ x + v.x, y + v.y, z + v.z };
}

template <typename T>
Point<T, 3> Point<T, 3>::Sub(T v) const
{
    return Point{ x - v, y - v, z - v };
}

template <typename T>
Point<T, 3> Point<T, 3>::Sub(const Point& v) const
{
    return Point{ x - v.x, y - v.y, z - v.z };
}

template <typename T>
Point<T, 3> Point<T, 3>::Mul(T v) const
{
    return Point{ x * v, y * v, z * v };
}

template <typename T>
Point<T, 3> Point<T, 3>::Mul(const Point& v) const
{
    return Point{ x * v.x, y * v.y, z * v.z };
}

template <typename T>
Point<T, 3> Point<T, 3>::Div(T v) const
{
    return Point{ x / v, y / v, z / v };
}

template <typename T>
Point<T, 3> Point<T, 3>::Div(const Point& v) const
{
    return Point{ x / v.x, y / v.y, z / v.z };
}

template <typename T>
Point<T, 3> Point<T, 3>::RAdd(T v) const
{
    return Point{ v + x, v + y, v + z };
}

template <typename T>
Point<T, 3> Point<T, 3>::RAdd(const Point& v) const
{
    return Point{ v.x + x, v.y + y, v.z + z };
}

template <typename T>
Point<T, 3> Point<T, 3>::RSub(T v) const
{
    return Point{ v - x, v - y, v - z };
}

template <typename T>
Point<T, 3> Point<T, 3>::RSub(const Point& v) const
{
    return Point{ v.x - x, v.y - y, v.z - z };
}

template <typename T>
Point<T, 3> Point<T, 3>::RMul(T v) const
{
    return Point{ v * x, v * y, v * z };
}

template <typename T>
Point<T, 3> Point<T, 3>::RMul(const Point& v) const
{
    return Point{ v.x * x, v.y * y, v.z * z };
}

template <typename T>
Point<T, 3> Point<T, 3>::RDiv(T v) const
{
    return Point{ v / x, v / y, v / z };
}

template <typename T>
Point<T, 3> Point<T, 3>::RDiv(const Point& v) const
{
    return Point{ v.x / x, v.y / y, v.z / z };
}

template <typename T>
void Point<T, 3>::IAdd(T v)
{
    x += v;
    y += v;
    z += v;
}

template <typename T>
void Point<T, 3>::IAdd(const Point& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
}

template <typename T>
void Point<T, 3>::ISub(T v)
{
    x -= v;
    y -= v;
    z -= v;
}

template <typename T>
void Point<T, 3>::ISub(const Point& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
}

template <typename T>
void Point<T, 3>::IMul(T v)
{
    x *= v;
    y *= v;
    z *= v;
}

template <typename T>
void Point<T, 3>::IMul(const Point& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
}

template <typename T>
void Point<T, 3>::IDiv(T v)
{
    x /= v;
    y /= v;
    z /= v;
}

template <typename T>
void Point<T, 3>::IDiv(const Point& v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
}

template <typename T>
const T& Point<T, 3>::At(size_t i) const
{
    assert(i < 3);

    if (i == 0)
    {
        return x;
    }

    if (i == 1)
    {
        return y;
    }

    return z;
}

template <typename T>
T& Point<T, 3>::At(size_t i)
{
    assert(i < 3);

    if (i == 0)
    {
        return x;
    }

    if (i == 1)
    {
        return y;
    }

    return z;
}

template <typename T>
T Point<T, 3>::Sum() const
{
    return x + y + z;
}

template <typename T>
T Point<T, 3>::Min() const
{
    return std::min(std::min(x, y), z);
}

template <typename T>
T Point<T, 3>::Max() const
{
    return std::max(std::max(x, y), z);
}

template <typename T>
T Point<T, 3>::AbsMin() const
{
    return CubbyFlow::AbsMin(CubbyFlow::AbsMin(x, y), z);
}

template <typename T>
T Point<T, 3>::AbsMax() const
{
    return CubbyFlow::AbsMax(CubbyFlow::AbsMax(x, y), z);
}

template <typename T>
size_t Point<T, 3>::DominantAxis() const
{
    return (std::fabs(x) > std::fabs(y))
               ? ((std::fabs(x) > std::fabs(z)) ? 0 : 2)
               : ((std::fabs(y) > std::fabs(z)) ? 1 : 2);
}

template <typename T>
size_t Point<T, 3>::SubdominantAxis() const
{
    return (std::fabs(x) < std::fabs(y))
               ? ((std::fabs(x) < std::fabs(z)) ? 0 : 2)
               : ((std::fabs(y) < std::fabs(z)) ? 1 : 2);
}

template <typename T>
template <typename U>
Point3<U> Point<T, 3>::CastTo() const
{
    return Point3<U>{ static_cast<U>(x), static_cast<U>(y), static_cast<U>(z) };
}

template <typename T>
bool Point<T, 3>::IsEqual(const Point& other) const
{
    return (x == other.x && y == other.y && z == other.z);
}

template <typename T>
T& Point<T, 3>::operator[](size_t i)
{
    assert(i < 3);

    if (i == 0)
    {
        return x;
    }

    if (i == 1)
    {
        return y;
    }

    return z;
}

template <typename T>
const T& Point<T, 3>::operator[](size_t i) const
{
    assert(i < 3);

    if (i == 0)
    {
        return x;
    }

    if (i == 1)
    {
        return y;
    }

    return z;
}

template <typename T>
Point<T, 3>& Point<T, 3>::operator=(const std::initializer_list<T>& list)
{
    Set(list);
    return (*this);
}

template <typename T>
Point<T, 3>& Point<T, 3>::operator+=(T v)
{
    IAdd(v);
    return (*this);
}

template <typename T>
Point<T, 3>& Point<T, 3>::operator+=(const Point& v)
{
    IAdd(v);
    return (*this);
}

template <typename T>
Point<T, 3>& Point<T, 3>::operator-=(T v)
{
    ISub(v);
    return (*this);
}

template <typename T>
Point<T, 3>& Point<T, 3>::operator-=(const Point& v)
{
    ISub(v);
    return (*this);
}

template <typename T>
Point<T, 3>& Point<T, 3>::operator*=(T v)
{
    IMul(v);
    return (*this);
}

template <typename T>
Point<T, 3>& Point<T, 3>::operator*=(const Point& v)
{
    IMul(v);
    return (*this);
}

template <typename T>
Point<T, 3>& Point<T, 3>::operator/=(T v)
{
    IDiv(v);
    return (*this);
}

template <typename T>
Point<T, 3>& Point<T, 3>::operator/=(const Point& v)
{
    IDiv(v);
    return (*this);
}

template <typename T>
bool Point<T, 3>::operator==(const Point& v) const
{
    return IsEqual(v);
}

template <typename T>
bool Point<T, 3>::operator!=(const Point& v) const
{
    return !IsEqual(v);
}

// Math functions
template <typename T>
Point<T, 3> operator+(const Point<T, 3>& a)
{
    return a;
}

template <typename T>
Point<T, 3> operator-(const Point<T, 3>& a)
{
    return Point<T, 3>(-a.x, -a.y, -a.z);
}

template <typename T>
Point<T, 3> operator+(const Point<T, 3>& a, T b)
{
    return a.Add(b);
}

template <typename T>
Point<T, 3> operator+(T a, const Point<T, 3>& b)
{
    return b.RAdd(a);
}

template <typename T>
Point<T, 3> operator+(const Point<T, 3>& a, const Point<T, 3>& b)
{
    return a.Add(b);
}

template <typename T>
Point<T, 3> operator-(const Point<T, 3>& a, T b)
{
    return a.Sub(b);
}

template <typename T>
Point<T, 3> operator-(T a, const Point<T, 3>& b)
{
    return b.RSub(a);
}

template <typename T>
Point<T, 3> operator-(const Point<T, 3>& a, const Point<T, 3>& b)
{
    return a.Sub(b);
}

template <typename T>
Point<T, 3> operator*(const Point<T, 3>& a, T b)
{
    return a.Mul(b);
}

template <typename T>
Point<T, 3> operator*(T a, const Point<T, 3>& b)
{
    return b.RMul(a);
}

template <typename T>
Point<T, 3> operator*(const Point<T, 3>& a, const Point<T, 3>& b)
{
    return a.Mul(b);
}

template <typename T>
Point<T, 3> operator/(const Point<T, 3>& a, T b)
{
    return a.Div(b);
}

template <typename T>
Point<T, 3> operator/(T a, const Point<T, 3>& b)
{
    return b.RDiv(a);
}

template <typename T>
Point<T, 3> operator/(const Point<T, 3>& a, const Point<T, 3>& b)
{
    return a.Div(b);
}

template <typename T>
Point<T, 3> Min(const Point<T, 3>& a, const Point<T, 3>& b)
{
    return Point<T, 3>{ std::min(a.x, b.x), std::min(a.y, b.y),
                        std::min(a.z, b.z) };
}

template <typename T>
Point<T, 3> Max(const Point<T, 3>& a, const Point<T, 3>& b)
{
    return Point<T, 3>{ std::max(a.x, b.x), std::max(a.y, b.y),
                        std::max(a.z, b.z) };
}

template <typename T>
Point<T, 3> Clamp(const Point<T, 3>& v, const Point<T, 3>& low,
                  const Point<T, 3>& high)
{
    return Point<T, 3>{ Clamp(v.x, low.x, high.x), Clamp(v.y, low.y, high.y),
                        Clamp(v.z, low.z, high.z) };
}

template <typename T>
Point<T, 3> Ceil(const Point<T, 3>& a)
{
    return Point<T, 3>{ std::ceil(a.x), std::ceil(a.y), std::ceil(a.z) };
}

template <typename T>
Point<T, 3> Floor(const Point<T, 3>& a)
{
    return Point<T, 3>{ std::floor(a.x), std::floor(a.y), std::floor(a.z) };
}
}  // namespace CubbyFlow

#endif