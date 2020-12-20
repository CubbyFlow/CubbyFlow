// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_VECTOR3_IMPL_HPP
#define CUBBYFLOW_VECTOR3_IMPL_HPP

#include <Core/Math/MathUtils.hpp>

#include <cassert>

namespace CubbyFlow
{
template <typename T>
template <typename U>
Vector<T, 3>::Vector(const std::initializer_list<U>& list)
{
    Set(list);
}

template <typename T>
Vector<T, 3>& Vector<T, 3>::operator=(const Vector& v)
{
    Set(v);
    return (*this);
}

template <typename T>
Vector<T, 3>& Vector<T, 3>::operator=(Vector&& v) noexcept
{
    Set(v);
    return (*this);
}

template <typename T>
void Vector<T, 3>::Set(T s)
{
    x = s;
    y = s;
    z = s;
}

template <typename T>
void Vector<T, 3>::Set(T newX, T newY, T newZ)
{
    x = newX;
    y = newY;
    z = newZ;
}

template <typename T>
void Vector<T, 3>::Set(const Vector2<T>& pt, T newZ)
{
    x = pt.x;
    y = pt.y;
    z = newZ;
}

template <typename T>
template <typename U>
void Vector<T, 3>::Set(const std::initializer_list<U>& list)
{
    assert(list.size() >= 3);

    auto inputElem = list.begin();
    x = static_cast<T>(*inputElem);
    y = static_cast<T>(*(++inputElem));
    z = static_cast<T>(*(++inputElem));
}

template <typename T>
void Vector<T, 3>::Set(const Vector& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
}

template <typename T>
void Vector<T, 3>::SetZero()
{
    x = 0;
    y = 0;
    z = 0;
}

template <typename T>
void Vector<T, 3>::Normalize()
{
    T length = Length();
    x /= length;
    y /= length;
    z /= length;
}

template <typename T>
Vector<T, 3> Vector<T, 3>::Add(T v) const
{
    return Vector<T, 3>{ x + v, y + v, z + v };
}

template <typename T>
Vector<T, 3> Vector<T, 3>::Add(const Vector& v) const
{
    return Vector<T, 3>{ x + v.x, y + v.y, z + v.z };
}

template <typename T>
Vector<T, 3> Vector<T, 3>::Sub(T v) const
{
    return Vector<T, 3>{ x - v, y - v, z - v };
}

template <typename T>
Vector<T, 3> Vector<T, 3>::Sub(const Vector& v) const
{
    return Vector<T, 3>{ x - v.x, y - v.y, z - v.z };
}

template <typename T>
Vector<T, 3> Vector<T, 3>::Mul(T v) const
{
    return Vector<T, 3>{ x * v, y * v, z * v };
}

template <typename T>
Vector<T, 3> Vector<T, 3>::Mul(const Vector& v) const
{
    return Vector<T, 3>{ x * v.x, y * v.y, z * v.z };
}

template <typename T>
Vector<T, 3> Vector<T, 3>::Div(T v) const
{
    return Vector<T, 3>{ x / v, y / v, z / v };
}

template <typename T>
Vector<T, 3> Vector<T, 3>::Div(const Vector& v) const
{
    return Vector<T, 3>{ x / v.x, y / v.y, z / v.z };
}

template <typename T>
T Vector<T, 3>::Dot(const Vector& v) const
{
    return x * v.x + y * v.y + z * v.z;
}

template <typename T>
Vector<T, 3> Vector<T, 3>::Cross(const Vector& v) const
{
    return Vector<T, 3>{ y * v.z - z * v.y, z * v.x - x * v.z,
                         x * v.y - y * v.x };
}

template <typename T>
Vector<T, 3> Vector<T, 3>::RSub(T v) const
{
    return Vector<T, 3>{ v - x, v - y, v - z };
}

template <typename T>
Vector<T, 3> Vector<T, 3>::RSub(const Vector& v) const
{
    return Vector<T, 3>{ v.x - x, v.y - y, v.z - z };
}

template <typename T>
Vector<T, 3> Vector<T, 3>::RDiv(T v) const
{
    return Vector<T, 3>{ v / x, v / y, v / z };
}

template <typename T>
Vector<T, 3> Vector<T, 3>::RDiv(const Vector& v) const
{
    return Vector<T, 3>{ v.x / x, v.y / y, v.z / z };
}

template <typename T>
Vector<T, 3> Vector<T, 3>::RCross(const Vector& v) const
{
    return Vector<T, 3>{ v.y * z - v.z * y, v.z * x - v.x * z,
                         v.x * y - v.y * x };
}

template <typename T>
void Vector<T, 3>::IAdd(T v)
{
    x += v;
    y += v;
    z += v;
}

template <typename T>
void Vector<T, 3>::IAdd(const Vector& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
}

template <typename T>
void Vector<T, 3>::ISub(T v)
{
    x -= v;
    y -= v;
    z -= v;
}

template <typename T>
void Vector<T, 3>::ISub(const Vector& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
}

template <typename T>
void Vector<T, 3>::IMul(T v)
{
    x *= v;
    y *= v;
    z *= v;
}

template <typename T>
void Vector<T, 3>::IMul(const Vector& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
}

template <typename T>
void Vector<T, 3>::IDiv(T v)
{
    x /= v;
    y /= v;
    z /= v;
}

template <typename T>
void Vector<T, 3>::IDiv(const Vector& v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
}

template <typename T>
const T& Vector<T, 3>::At(size_t i) const
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
T& Vector<T, 3>::At(size_t i)
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
T Vector<T, 3>::Sum() const
{
    return x + y + z;
}

template <typename T>
T Vector<T, 3>::Avg() const
{
    return Sum() / 3;
}

template <typename T>
T Vector<T, 3>::Min() const
{
    return std::min({ x, y, z });
}

template <typename T>
T Vector<T, 3>::Max() const
{
    return std::max({ x, y, z });
}

template <typename T>
T Vector<T, 3>::AbsMin() const
{
    return CubbyFlow::AbsMin(CubbyFlow::AbsMin(x, y), z);
}

template <typename T>
T Vector<T, 3>::AbsMax() const
{
    return CubbyFlow::AbsMax(CubbyFlow::AbsMax(x, y), z);
}

template <typename T>
size_t Vector<T, 3>::DominantAxis() const
{
    return (std::fabs(x) > std::fabs(y))
               ? ((std::fabs(x) > std::fabs(z)) ? 0 : 2)
               : ((std::fabs(y) > std::fabs(z)) ? 1 : 2);
}

template <typename T>
size_t Vector<T, 3>::SubdominantAxis() const
{
    return (std::fabs(x) < std::fabs(y))
               ? ((std::fabs(x) < std::fabs(z)) ? 0 : 2)
               : ((std::fabs(y) < std::fabs(z)) ? 1 : 2);
}

template <typename T>
Vector<T, 3> Vector<T, 3>::Normalized() const
{
    return Vector<T, 3>{ x / Length(), y / Length(), z / Length() };
}

template <typename T>
T Vector<T, 3>::Length() const
{
    return std::sqrt(x * x + y * y + z * z);
}

template <typename T>
T Vector<T, 3>::LengthSquared() const
{
    return x * x + y * y + z * z;
}

template <typename T>
T Vector<T, 3>::DistanceTo(const Vector<T, 3>& other) const
{
    return Sub(other).Length();
}

template <typename T>
T Vector<T, 3>::DistanceSquaredTo(const Vector<T, 3>& other) const
{
    return Sub(other).LengthSquared();
}

template <typename T>
Vector<T, 3> Vector<T, 3>::Reflected(const Vector<T, 3>& normal) const
{
    // this - 2(this.n)n
    return Sub(normal.Mul(2 * Dot(normal)));
}

template <typename T>
Vector<T, 3> Vector<T, 3>::Projected(const Vector<T, 3>& normal) const
{
    // this - (this.n)n
    return Sub(normal.Mul(Dot(normal)));
}

template <typename T>
std::tuple<Vector<T, 3>, Vector<T, 3>> Vector<T, 3>::Tangential() const
{
    Vector<T, 3> a =
        ((std::fabs(y) > 0 || std::fabs(z) > 0) ? Vector<T, 3>{ 1, 0, 0 }
                                                : Vector<T, 3>{ 0, 1, 0 })
            .Cross(*this)
            .Normalized();
    Vector<T, 3> b = Cross(a);

    return std::make_tuple(a, b);
}

template <typename T>
template <typename U>
Vector<U, 3> Vector<T, 3>::CastTo() const
{
    return Vector<U, 3>{ static_cast<U>(x), static_cast<U>(y),
                         static_cast<U>(z) };
}

template <typename T>
bool Vector<T, 3>::IsEqual(const Vector& other) const
{
    return (x == other.x && y == other.y && z == other.z);
}

template <typename T>
bool Vector<T, 3>::IsSimilar(const Vector& other, T epsilon) const
{
    return (std::fabs(x - other.x) < epsilon &&
            std::fabs(y - other.y) < epsilon &&
            std::fabs(z - other.z) < epsilon);
}

template <typename T>
T& Vector<T, 3>::operator[](size_t i)
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
const T& Vector<T, 3>::operator[](size_t i) const
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
template <typename U>
Vector<T, 3>& Vector<T, 3>::operator=(const std::initializer_list<U>& list)
{
    Set(list);
    return (*this);
}

template <typename T>
Vector<T, 3>& Vector<T, 3>::operator+=(T v)
{
    IAdd(v);
    return (*this);
}

template <typename T>
Vector<T, 3>& Vector<T, 3>::operator+=(const Vector& v)
{
    IAdd(v);
    return (*this);
}

template <typename T>
Vector<T, 3>& Vector<T, 3>::operator-=(T v)
{
    ISub(v);
    return (*this);
}

template <typename T>
Vector<T, 3>& Vector<T, 3>::operator-=(const Vector& v)
{
    ISub(v);
    return (*this);
}

template <typename T>
Vector<T, 3>& Vector<T, 3>::operator*=(T v)
{
    IMul(v);
    return (*this);
}

template <typename T>
Vector<T, 3>& Vector<T, 3>::operator*=(const Vector& v)
{
    IMul(v);
    return (*this);
}

template <typename T>
Vector<T, 3>& Vector<T, 3>::operator/=(T v)
{
    IDiv(v);
    return (*this);
}

template <typename T>
Vector<T, 3>& Vector<T, 3>::operator/=(const Vector& v)
{
    IDiv(v);
    return (*this);
}

template <typename T>
bool Vector<T, 3>::operator==(const Vector& v) const
{
    return IsEqual(v);
}

template <typename T>
bool Vector<T, 3>::operator!=(const Vector& v) const
{
    return !IsEqual(v);
}

template <typename T>
Vector<T, 3> operator+(const Vector<T, 3>& a)
{
    return a;
}

template <typename T>
Vector<T, 3> operator-(const Vector<T, 3>& a)
{
    return Vector<T, 3>{ -a.x, -a.y, -a.z };
}

template <typename T>
Vector<T, 3> operator+(const Vector<T, 3>& a, T b)
{
    return a.Add(b);
}

template <typename T>
Vector<T, 3> operator+(T a, const Vector<T, 3>& b)
{
    return b.Add(a);
}

template <typename T>
Vector<T, 3> operator+(const Vector<T, 3>& a, const Vector<T, 3>& b)
{
    return a.Add(b);
}

template <typename T>
Vector<T, 3> operator-(const Vector<T, 3>& a, T b)
{
    return a.Sub(b);
}

template <typename T>
Vector<T, 3> operator-(T a, const Vector<T, 3>& b)
{
    return b.RSub(a);
}

template <typename T>
Vector<T, 3> operator-(const Vector<T, 3>& a, const Vector<T, 3>& b)
{
    return a.Sub(b);
}

template <typename T>
Vector<T, 3> operator*(const Vector<T, 3>& a, T b)
{
    return a.Mul(b);
}

template <typename T>
Vector<T, 3> operator*(T a, const Vector<T, 3>& b)
{
    return b.Mul(a);
}

template <typename T>
Vector<T, 3> operator*(const Vector<T, 3>& a, const Vector<T, 3>& b)
{
    return a.Mul(b);
}

template <typename T>
Vector<T, 3> operator/(const Vector<T, 3>& a, T b)
{
    return a.Div(b);
}

template <typename T>
Vector<T, 3> operator/(T a, const Vector<T, 3>& b)
{
    return b.RDiv(a);
}

template <typename T>
Vector<T, 3> operator/(const Vector<T, 3>& a, const Vector<T, 3>& b)
{
    return a.Div(b);
}

template <typename T>
Vector<T, 3> Min(const Vector<T, 3>& a, const Vector<T, 3>& b)
{
    return Vector<T, 3>{ std::min(a.x, b.x), std::min(a.y, b.y),
                         std::min(a.z, b.z) };
}

template <typename T>
Vector<T, 3> Max(const Vector<T, 3>& a, const Vector<T, 3>& b)
{
    return Vector<T, 3>{ std::max(a.x, b.x), std::max(a.y, b.y),
                         std::max(a.z, b.z) };
}

template <typename T>
Vector<T, 3> Clamp(const Vector<T, 3>& v, const Vector<T, 3>& low,
                   const Vector<T, 3>& high)
{
    return Vector<T, 3>{ Clamp(v.x, low.x, high.x), Clamp(v.y, low.y, high.y),
                         Clamp(v.z, low.z, high.z) };
}

template <typename T>
Vector<T, 3> Ceil(const Vector<T, 3>& a)
{
    return Vector<T, 3>{ std::ceil(a.x), std::ceil(a.y), std::ceil(a.z) };
}

template <typename T>
Vector<T, 3> Floor(const Vector<T, 3>& a)
{
    return Vector<T, 3>{ std::floor(a.x), std::floor(a.y), std::floor(a.z) };
}

template <typename T>
Vector<T, 3> MonotonicCatmullRom(const Vector<T, 3>& v0, const Vector<T, 3>& v1,
                                 const Vector<T, 3>& v2, const Vector<T, 3>& v3,
                                 T f)
{
    static const T two = static_cast<T>(2);
    static const T three = static_cast<T>(3);

    Vector<T, 3> d1 = (v2 - v0) / two;
    Vector<T, 3> d2 = (v3 - v1) / two;
    Vector<T, 3> D1 = v2 - v1;

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

    Vector<T, 3> a3 = d1 + d2 - two * D1;
    Vector<T, 3> a2 = three * D1 - two * d1 - d2;
    Vector<T, 3> a1 = d1;
    Vector<T, 3> a0 = v1;

    return a3 * Cubic(f) + a2 * Square(f) + a1 * f + a0;
}
}  // namespace CubbyFlow

#endif