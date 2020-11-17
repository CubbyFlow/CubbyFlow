// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_VECTOR4_IMPL_HPP
#define CUBBYFLOW_VECTOR4_IMPL_HPP

#include <Core/Math/MathUtils.hpp>

#include <cassert>

namespace CubbyFlow
{
template <typename T>
template <typename U>
Vector<T, 4>::Vector(const std::initializer_list<U>& list)
{
    Set(list);
}

template <typename T>
Vector<T, 4>& Vector<T, 4>::operator=(const Vector& v)
{
    Set(v);
    return (*this);
}

template <typename T>
Vector<T, 4>& Vector<T, 4>::operator=(Vector&& v) noexcept
{
    Set(v);
    return (*this);
}

template <typename T>
void Vector<T, 4>::Set(T s)
{
    x = s;
    y = s;
    z = s;
    w = s;
}

template <typename T>
void Vector<T, 4>::Set(T newX, T newY, T newZ, T newW)
{
    x = newX;
    y = newY;
    z = newZ;
    w = newW;
}

template <typename T>
void Vector<T, 4>::Set(const Vector<T, 3>& pt, T newW)
{
    x = pt.x;
    y = pt.y;
    z = pt.z;
    w = newW;
}

template <typename T>
template <typename U>
void Vector<T, 4>::Set(const std::initializer_list<U>& list)
{
    assert(list.size() >= 4);

    auto inputElem = list.begin();
    x = static_cast<T>(*inputElem);
    y = static_cast<T>(*(++inputElem));
    z = static_cast<T>(*(++inputElem));
    w = static_cast<T>(*(++inputElem));
}

template <typename T>
void Vector<T, 4>::Set(const Vector& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
}

template <typename T>
void Vector<T, 4>::SetZero()
{
    x = 0;
    y = 0;
    z = 0;
    w = 0;
}

template <typename T>
void Vector<T, 4>::Normalize()
{
    T length = Length();
    x /= length;
    y /= length;
    z /= length;
    w /= length;
}

template <typename T>
Vector<T, 4> Vector<T, 4>::Add(T v) const
{
    return Vector{ x + v, y + v, z + v, w + v };
}

template <typename T>
Vector<T, 4> Vector<T, 4>::Add(const Vector& v) const
{
    return Vector{ x + v.x, y + v.y, z + v.z, w + v.w };
}

template <typename T>
Vector<T, 4> Vector<T, 4>::Sub(T v) const
{
    return Vector{ x - v, y - v, z - v, w - v };
}

template <typename T>
Vector<T, 4> Vector<T, 4>::Sub(const Vector& v) const
{
    return Vector{ x - v.x, y - v.y, z - v.z, w - v.w };
}

template <typename T>
Vector<T, 4> Vector<T, 4>::Mul(T v) const
{
    return Vector{ x * v, y * v, z * v, w * v };
}

template <typename T>
Vector<T, 4> Vector<T, 4>::Mul(const Vector& v) const
{
    return Vector{ x * v.x, y * v.y, z * v.z, w * v.w };
}

template <typename T>
Vector<T, 4> Vector<T, 4>::Div(T v) const
{
    return Vector{ x / v, y / v, z / v, w / v };
}

template <typename T>
Vector<T, 4> Vector<T, 4>::Div(const Vector& v) const
{
    return Vector{ x / v.x, y / v.y, z / v.z, w / v.w };
}

template <typename T>
T Vector<T, 4>::Dot(const Vector& v) const
{
    return x * v.x + y * v.y + z * v.z + w * v.w;
}

template <typename T>
Vector<T, 4> Vector<T, 4>::RSub(T v) const
{
    return Vector{ v - x, v - y, v - z, v - w };
}

template <typename T>
Vector<T, 4> Vector<T, 4>::RSub(const Vector& v) const
{
    return Vector{ v.x - x, v.y - y, v.z - z, v.w - w };
}

template <typename T>
Vector<T, 4> Vector<T, 4>::RDiv(T v) const
{
    return Vector{ v / x, v / y, v / z, v / w };
}

template <typename T>
Vector<T, 4> Vector<T, 4>::RDiv(const Vector& v) const
{
    return Vector{ v.x / x, v.y / y, v.z / z, v.w / w };
}

template <typename T>
void Vector<T, 4>::IAdd(T v)
{
    x += v;
    y += v;
    z += v;
    w += v;
}

template <typename T>
void Vector<T, 4>::IAdd(const Vector& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
}

template <typename T>
void Vector<T, 4>::ISub(T v)
{
    x -= v;
    y -= v;
    z -= v;
    w -= v;
}

template <typename T>
void Vector<T, 4>::ISub(const Vector& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
}

template <typename T>
void Vector<T, 4>::IMul(T v)
{
    x *= v;
    y *= v;
    z *= v;
    w *= v;
}

template <typename T>
void Vector<T, 4>::IMul(const Vector& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
}

template <typename T>
void Vector<T, 4>::IDiv(T v)
{
    x /= v;
    y /= v;
    z /= v;
    w /= v;
}

template <typename T>
void Vector<T, 4>::IDiv(const Vector& v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
}

template <typename T>
const T& Vector<T, 4>::At(size_t i) const
{
    assert(i < 4);

    if (i == 0)
    {
        return x;
    }

    if (i == 1)
    {
        return y;
    }

    if (i == 2)
    {
        return z;
    }

    return w;
}

template <typename T>
T& Vector<T, 4>::At(size_t i)
{
    assert(i < 4);

    if (i == 0)
    {
        return x;
    }

    if (i == 1)
    {
        return y;
    }

    if (i == 2)
    {
        return z;
    }

    return w;
}

template <typename T>
T Vector<T, 4>::Sum() const
{
    return x + y + z + w;
}

template <typename T>
T Vector<T, 4>::Avg() const
{
    return Sum() / 4;
}

template <typename T>
T Vector<T, 4>::Min() const
{
    return std::min({ x, y, z, w });
}

template <typename T>
T Vector<T, 4>::Max() const
{
    return std::max({ x, y, z, w });
}

template <typename T>
T Vector<T, 4>::AbsMin() const
{
    return CubbyFlow::AbsMin(CubbyFlow::AbsMin(x, y), CubbyFlow::AbsMin(z, w));
}

template <typename T>
T Vector<T, 4>::AbsMax() const
{
    return CubbyFlow::AbsMax(CubbyFlow::AbsMax(x, y), CubbyFlow::AbsMax(z, w));
}

template <typename T>
size_t Vector<T, 4>::DominantAxis() const
{
    return (std::fabs(x) > std::fabs(y))
               ? ((std::fabs(x) > std::fabs(z))
                      ? ((std::fabs(x) > std::fabs(w)) ? 0 : 3)
                      : ((std::fabs(z) > std::fabs(w)) ? 2 : 3))
               : ((std::fabs(y) > std::fabs(z))
                      ? ((std::fabs(y) > std::fabs(w)) ? 1 : 3)
                      : ((std::fabs(z) > std::fabs(w)) ? 2 : 3));
}

template <typename T>
size_t Vector<T, 4>::SubdominantAxis() const
{
    return (std::fabs(x) < std::fabs(y))
               ? ((std::fabs(x) < std::fabs(z))
                      ? ((std::fabs(x) < std::fabs(w)) ? 0 : 3)
                      : ((std::fabs(z) < std::fabs(w)) ? 2 : 3))
               : ((std::fabs(y) < std::fabs(z))
                      ? ((std::fabs(y) < std::fabs(w)) ? 1 : 3)
                      : ((std::fabs(z) < std::fabs(w)) ? 2 : 3));
}

template <typename T>
Vector<T, 4> Vector<T, 4>::Normalized() const
{
    return Vector{ x / Length(), y / Length(), z / Length(), w / Length() };
}

template <typename T>
T Vector<T, 4>::Length() const
{
    return std::sqrt(x * x + y * y + z * z + w * w);
}

template <typename T>
T Vector<T, 4>::LengthSquared() const
{
    return x * x + y * y + z * z + w * w;
}

template <typename T>
T Vector<T, 4>::DistanceTo(const Vector<T, 4>& other) const
{
    return Sub(other).Length();
}

template <typename T>
T Vector<T, 4>::DistanceSquaredTo(const Vector<T, 4>& other) const
{
    return Sub(other).LengthSquared();
}

template <typename T>
template <typename U>
Vector<U, 4> Vector<T, 4>::CastTo() const
{
    return Vector<U, 4>{ static_cast<U>(x), static_cast<U>(y),
                         static_cast<U>(z), static_cast<U>(w) };
}

template <typename T>
bool Vector<T, 4>::IsEqual(const Vector& other) const
{
    return (x == other.x && y == other.y && z == other.z && w == other.w);
}

template <typename T>
bool Vector<T, 4>::IsSimilar(const Vector& other, T epsilon) const
{
    return (std::fabs(x - other.x) < epsilon) &&
           (std::fabs(y - other.y) < epsilon &&
            std::fabs(z - other.z) < epsilon) &&
           (std::fabs(w - other.w) < epsilon);
}

template <typename T>
T& Vector<T, 4>::operator[](size_t i)
{
    assert(i < 4);

    if (i == 0)
    {
        return x;
    }

    if (i == 1)
    {
        return y;
    }

    if (i == 2)
    {
        return z;
    }

    return w;
}

template <typename T>
const T& Vector<T, 4>::operator[](size_t i) const
{
    assert(i < 4);

    if (i == 0)
    {
        return x;
    }

    if (i == 1)
    {
        return y;
    }

    if (i == 2)
    {
        return z;
    }

    return w;
}

template <typename T>
template <typename U>
Vector<T, 4>& Vector<T, 4>::operator=(const std::initializer_list<U>& list)
{
    Set(list);
    return (*this);
}

template <typename T>
Vector<T, 4>& Vector<T, 4>::operator+=(T v)
{
    IAdd(v);
    return (*this);
}

template <typename T>
Vector<T, 4>& Vector<T, 4>::operator+=(const Vector& v)
{
    IAdd(v);
    return (*this);
}

template <typename T>
Vector<T, 4>& Vector<T, 4>::operator-=(T v)
{
    ISub(v);
    return (*this);
}

template <typename T>
Vector<T, 4>& Vector<T, 4>::operator-=(const Vector& v)
{
    ISub(v);
    return (*this);
}

template <typename T>
Vector<T, 4>& Vector<T, 4>::operator*=(T v)
{
    IMul(v);
    return (*this);
}

template <typename T>
Vector<T, 4>& Vector<T, 4>::operator*=(const Vector& v)
{
    IMul(v);
    return (*this);
}

template <typename T>
Vector<T, 4>& Vector<T, 4>::operator/=(T v)
{
    IDiv(v);
    return (*this);
}

template <typename T>
Vector<T, 4>& Vector<T, 4>::operator/=(const Vector& v)
{
    IDiv(v);
    return (*this);
}

template <typename T>
bool Vector<T, 4>::operator==(const Vector& v) const
{
    return IsEqual(v);
}

template <typename T>
bool Vector<T, 4>::operator!=(const Vector& v) const
{
    return !IsEqual(v);
}

template <typename T>
Vector<T, 4> operator+(const Vector<T, 4>& a)
{
    return a;
}

template <typename T>
Vector<T, 4> operator-(const Vector<T, 4>& a)
{
    return Vector<T, 4>{ -a.x, -a.y, -a.z, -a.w };
}

template <typename T>
Vector<T, 4> operator+(const Vector<T, 4>& a, T b)
{
    return a.Add(b);
}

template <typename T>
Vector<T, 4> operator+(T a, const Vector<T, 4>& b)
{
    return b.Add(a);
}

template <typename T>
Vector<T, 4> operator+(const Vector<T, 4>& a, const Vector<T, 4>& b)
{
    return a.Add(b);
}

template <typename T>
Vector<T, 4> operator-(const Vector<T, 4>& a, T b)
{
    return a.Sub(b);
}

template <typename T>
Vector<T, 4> operator-(T a, const Vector<T, 4>& b)
{
    return b.RSub(a);
}

template <typename T>
Vector<T, 4> operator-(const Vector<T, 4>& a, const Vector<T, 4>& b)
{
    return a.Sub(b);
}

template <typename T>
Vector<T, 4> operator*(const Vector<T, 4>& a, T b)
{
    return a.Mul(b);
}

template <typename T>
Vector<T, 4> operator*(T a, const Vector<T, 4>& b)
{
    return b.Mul(a);
}

template <typename T>
Vector<T, 4> operator*(const Vector<T, 4>& a, const Vector<T, 4>& b)
{
    return a.Mul(b);
}

template <typename T>
Vector<T, 4> operator/(const Vector<T, 4>& a, T b)
{
    return a.Div(b);
}

template <typename T>
Vector<T, 4> operator/(T a, const Vector<T, 4>& b)
{
    return b.RDiv(a);
}

template <typename T>
Vector<T, 4> operator/(const Vector<T, 4>& a, const Vector<T, 4>& b)
{
    return a.Div(b);
}

template <typename T>
Vector<T, 4> Min(const Vector<T, 4>& a, const Vector<T, 4>& b)
{
    return Vector<T, 4>{ std::min(a.x, b.x), std::min(a.y, b.y),
                         std::min(a.z, b.z), std::min(a.w, b.w) };
}

template <typename T>
Vector<T, 4> Max(const Vector<T, 4>& a, const Vector<T, 4>& b)
{
    return Vector<T, 4>{ std::max(a.x, b.x), std::max(a.y, b.y),
                         std::max(a.z, b.z), std::max(a.w, b.w) };
}

template <typename T>
Vector<T, 4> Clamp(const Vector<T, 4>& v, const Vector<T, 4>& low,
                   const Vector<T, 4>& high)
{
    return Vector<T, 4>{ Clamp(v.x, low.x, high.x), Clamp(v.y, low.y, high.y),
                         Clamp(v.z, low.z, high.z), Clamp(v.w, low.w, high.w) };
}

template <typename T>
Vector<T, 4> Ceil(const Vector<T, 4>& a)
{
    return Vector<T, 4>{ std::ceil(a.x), std::ceil(a.y), std::ceil(a.z),
                         std::ceil(a.w) };
}

template <typename T>
Vector<T, 4> Floor(const Vector<T, 4>& a)
{
    return Vector<T, 4>{ std::floor(a.x), std::floor(a.y), std::floor(a.z),
                         std::floor(a.w) };
}

template <typename T>
Vector<T, 4> MonotonicCatmullRom(const Vector<T, 4>& v0, const Vector<T, 4>& v1,
                                 const Vector<T, 4>& v2, const Vector<T, 4>& v3,
                                 T f)
{
    static const T two = static_cast<T>(2);
    static const T three = static_cast<T>(3);

    Vector<T, 4> d1 = (v2 - v0) / two;
    Vector<T, 4> d2 = (v3 - v1) / two;
    Vector<T, 4> D1 = v2 - v1;

    if (std::fabs(D1.x) < std::numeric_limits<float>::epsilon() ||
        Sign(D1.x) != Sign(d1.x) || Sign(D1.x) != Sign(d2.x))
    {
        d1.x = d2.x = 0;
    }

    if (std::fabs(D1.y) < std::numeric_limits<float>::epsilon() ||
        Sign(D1.y) != Sign(d1.y) || Sign(D1.y) != Sign(d2.y))
    {
        d1.y = d2.y = 0;
    }

    if (std::fabs(D1.z) < std::numeric_limits<float>::epsilon() ||
        Sign(D1.z) != Sign(d1.z) || Sign(D1.z) != Sign(d2.z))
    {
        d1.z = d2.z = 0;
    }

    if (std::fabs(D1.w) < std::numeric_limits<float>::epsilon() ||
        Sign(D1.w) != Sign(d1.w) || Sign(D1.w) != Sign(d2.w))
    {
        d1.w = d2.w = 0;
    }

    Vector<T, 4> a3 = d1 + d2 - two * D1;
    Vector<T, 4> a2 = three * D1 - two * d1 - d2;
    Vector<T, 4> a1 = d1;
    Vector<T, 4> a0 = v1;

    return a3 * Cubic(f) + a2 * Square(f) + a1 * f + a0;
}
}  // namespace CubbyFlow

#endif