// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_VECTOR4_HPP
#define CUBBYFLOW_VECTOR4_HPP

#include <Core/Vector/Vector3.hpp>

#include <limits>

namespace CubbyFlow
{
//!
//! \brief 4-D vector class.
//!
//! This class defines simple 4-D vector data.
//!
//! \tparam T - Type of the element
//!
template <typename T>
class Vector<T, 4> final
{
 public:
    static_assert(std::is_floating_point<T>::value,
                  "Vector only can be instantiated with floating point types");

    //! Constructs default vector (0, 0, 0, 0).
    constexpr Vector() : x{ 0 }, y{ 0 }, z{ 0 }, w{ 0 }
    {
        // Do nothing
    }

    //! Constructs vector with given parameters \p _x, \p _y, \p _z, and \p _w.
    constexpr Vector(T _x, T _y, T _z, T _w)
        : x{ _x }, y{ _y }, z{ _z }, w{ _w }
    {
        // Do nothing
    }

    //! Constructs vector with a 3-D vector (x, y, and z) and a scalar (w).
    constexpr Vector(const Vector<T, 3>& v, T _w)
        : x{ v.x }, y{ v.y }, z{ v.z }, w{ _w }
    {
        // Do nothing
    }

    //! Constructs vector with initializer list.
    template <typename U>
    Vector(const std::initializer_list<U>& list);

    //! Copy constructor.
    constexpr Vector(const Vector& v) : x{ v.x }, y{ v.y }, z{ v.z }, w{ v.w }
    {
        // Do nothing
    }

    //! Move constructor.
    constexpr Vector(Vector&& v) noexcept
        : x{ v.x }, y{ v.y }, z{ v.z }, w{ v.w }
    {
        // Do nothing
    }

    //! Default destructor.
    ~Vector() = default;

    //! Copy assignment operator.
    Vector& operator=(const Vector& v);

    //! Move assignment operator.
    Vector& operator=(Vector&& v) noexcept;

    //! Set both x, y, z, and w components to \p s.
    void Set(T s);

    //! Set x, y, z, and w components with given parameters.
    void Set(T x, T y, T z, T w);

    //! Set x, y, z, and w components with \p pt.x, \p pt.y, \p z, and \p newW.
    void Set(const Vector<T, 3>& pt, T newW);

    //! Set x, y, z, and w components with given initializer list.
    template <typename U>
    void Set(const std::initializer_list<U>& list);

    //! Set x, y, z, and w with other vector \p v.
    void Set(const Vector& v);

    //! Set all x, y, z, and w to zero.
    void SetZero();

    //! Normalizes this vector.
    void Normalize();

    //! Computes this + (v, v, v, v).
    [[nodiscard]] Vector Add(T v) const;

    //! Computes this + (v.x, v.y, v.z, v.w).
    [[nodiscard]] Vector Add(const Vector& v) const;

    //! Computes this - (v, v, v, v).
    [[nodiscard]] Vector Sub(T v) const;

    //! Computes this - (v.x, v.y, v.z, v.w).
    [[nodiscard]] Vector Sub(const Vector& v) const;

    //! Computes this * (v, v, v, v).
    [[nodiscard]] Vector Mul(T v) const;

    //! Computes this * (v.x, v.y, v.z, v.w).
    [[nodiscard]] Vector Mul(const Vector& v) const;

    //! Computes this / (v, v, v, v).
    [[nodiscard]] Vector Div(T v) const;

    //! Computes this / (v.x, v.y, v.z, v.w).
    [[nodiscard]] Vector Div(const Vector& v) const;

    //! Computes dot product.
    [[nodiscard]] T Dot(const Vector& v) const;

    //! Computes (v, v, v, v) - this.
    [[nodiscard]] Vector RSub(T v) const;

    //! Computes (v.x, v.y, v.z, v.w) - this.
    [[nodiscard]] Vector RSub(const Vector& v) const;

    //! Computes (v, v, v, v) / this.
    [[nodiscard]] Vector RDiv(T v) const;

    //! Computes (v.x, v.y, v.z, v.w) / this.
    [[nodiscard]] Vector RDiv(const Vector& v) const;

    //! Computes this += (v, v, v, v).
    void IAdd(T v);

    //! Computes this += (v.x, v.y, v.z, v.w).
    void IAdd(const Vector& v);

    //! Computes this -= (v, v, v, v).
    void ISub(T v);

    //! Computes this -= (v.x, v.y, v.z, v.w).
    void ISub(const Vector& v);

    //! Computes this *= (v, v, v, v).
    void IMul(T v);

    //! Computes this *= (v.x, v.y, v.z, v.w).
    void IMul(const Vector& v);

    //! Computes this /= (v, v, v, v).
    void IDiv(T v);

    //! Computes this /= (v.x, v.y, v.z, v.w).
    void IDiv(const Vector& v);

    //! Returns const reference to the \p i -th element of the vector.
    [[nodiscard]] const T& At(size_t i) const;

    //! Returns reference to the \p i -th element of the vector.
    [[nodiscard]] T& At(size_t i);

    //! Returns the sum of all the components (i.e. x + y + z + w).
    [[nodiscard]] T Sum() const;

    //! Returns the average of all the components.
    [[nodiscard]] T Avg() const;

    //! Returns the minimum value among x, y, z and w.
    [[nodiscard]] T Min() const;

    //! Returns the maximum value among x, y, z and w.
    [[nodiscard]] T Max() const;

    //! Returns the absolute minimum value among x, y, z and w.
    [[nodiscard]] T AbsMin() const;

    //! Returns the absolute maximum value among x, y, z and w.
    [[nodiscard]] T AbsMax() const;

    //! Returns the index of the dominant axis.
    [[nodiscard]] size_t DominantAxis() const;

    //! Returns the index of the sub-dominant axis.
    [[nodiscard]] size_t SubdominantAxis() const;

    //! Returns normalized vector.
    [[nodiscard]] Vector Normalized() const;

    //! Returns the length of the vector.
    [[nodiscard]] T Length() const;

    //! Returns the squared length of the vector.
    [[nodiscard]] T LengthSquared() const;

    //! Returns the distance to the other vector.
    [[nodiscard]] T DistanceTo(const Vector& other) const;

    //! Returns the squared distance to the other vector.
    [[nodiscard]] T DistanceSquaredTo(const Vector& other) const;

    //! Returns a vector with different value type.
    template <typename U>
    [[nodiscard]] Vector<U, 4> CastTo() const;

    //! Returns true if \p other is the same as this vector.
    [[nodiscard]] bool IsEqual(const Vector& other) const;

    //! Returns true if \p other is similar to this vector.
    [[nodiscard]] bool IsSimilar(
        const Vector& other,
        T epsilon = std::numeric_limits<T>::epsilon()) const;

    //! Returns reference to the \p i -th element of the vector.
    T& operator[](size_t i);

    //! Returns const reference to the \p i -th element of the vector.
    const T& operator[](size_t i) const;

    //! Set x and y components with given initializer list.
    template <typename U>
    Vector& operator=(const std::initializer_list<U>& list);

    //! Computes this += (v, v, v, v)
    Vector& operator+=(T v);

    //! Computes this += (v.x, v.y, v.z, v.w)
    Vector& operator+=(const Vector& v);

    //! Computes this -= (v, v, v, v)
    Vector& operator-=(T v);

    //! Computes this -= (v.x, v.y, v.z, v.w)
    Vector& operator-=(const Vector& v);

    //! Computes this *= (v, v, v, v)
    Vector& operator*=(T v);

    //! Computes this *= (v.x, v.y, v.z, v.w)
    Vector& operator*=(const Vector& v);

    //! Computes this /= (v, v, v, v)
    Vector& operator/=(T v);

    //! Computes this /= (v.x, v.y, v.z, v.w)
    Vector& operator/=(const Vector& v);

    //! Returns true if \p other is the same as this vector.
    bool operator==(const Vector& v) const;

    //! Returns true if \p other is the not same as this vector.
    bool operator!=(const Vector& v) const;

    //! X (or the first) component of the vector.
    T x;

    //! Y (or the second) component of the vector.
    T y;

    //! Z (or the third) component of the vector.
    T z;

    //! W (or the fourth) component of the vector.
    T w;
};

//! Type alias for four dimensional vector.
template <typename T>
using Vector4 = Vector<T, 4>;

//! Positive sign operator.
template <typename T>
Vector<T, 4> operator+(const Vector<T, 4>& a);

//! Negative sign operator.
template <typename T>
Vector<T, 4> operator-(const Vector<T, 4>& a);

//! Computes (a, a, a, a) + (b.x, b.y, b.z, b.w).
template <typename T>
Vector<T, 4> operator+(T a, const Vector<T, 4>& b);

//! Computes (a.x, a.y, a.z, a.w) + (b.x, b.y, b.z, b.w).
template <typename T>
Vector<T, 4> operator+(const Vector<T, 4>& a, const Vector<T, 4>& b);

//! Computes (a.x, a.y, a.z, a.w) - (b, b, b, b).
template <typename T>
Vector<T, 4> operator-(const Vector<T, 4>& a, T b);

//! Computes (a, a, a, a) - (b.x, b.y, b.z, b.w).
template <typename T>
Vector<T, 4> operator-(T a, const Vector<T, 4>& b);

//! Computes (a.x, a.y, a.z, a.w) - (b.x, b.y, b.z, b.w).
template <typename T>
Vector<T, 4> operator-(const Vector<T, 4>& a, const Vector<T, 4>& b);

//! Computes (a.x, a.y, a.z, a.w) * (b, b, b, b).
template <typename T>
Vector<T, 4> operator*(const Vector<T, 4>& a, T b);

//! Computes (a, a, a, a) * (b.x, b.y, b.z, b.w).
template <typename T>
Vector<T, 4> operator*(T a, const Vector<T, 4>& b);

//! Computes (a.x, a.y, a.z, a.w) * (b.x, b.y, b.z, b.w).
template <typename T>
Vector<T, 4> operator*(const Vector<T, 4>& a, const Vector<T, 4>& b);

//! Computes (a.x, a.y, a.z, a.w) / (b, b, b, b).
template <typename T>
Vector<T, 4> operator/(const Vector<T, 4>& a, T b);

//! Computes (a, a, a, a) / (b.x, b.y, b.z, b.w).
template <typename T>
Vector<T, 4> operator/(T a, const Vector<T, 4>& b);

//! Computes (a.x, a.y, a.z, a.w) / (b.x, b.y, b.z, b.w).
template <typename T>
Vector<T, 4> operator/(const Vector<T, 4>& a, const Vector<T, 4>& b);

//! Returns element-wise min vector.
template <typename T>
Vector<T, 4> Min(const Vector<T, 4>& a, const Vector<T, 4>& b);

//! Returns element-wise max vector.
template <typename T>
Vector<T, 4> Max(const Vector<T, 4>& a, const Vector<T, 4>& b);

//! Returns element-wise clamped vector.
template <typename T>
Vector<T, 4> Clamp(const Vector<T, 4>& v, const Vector<T, 4>& low,
                   const Vector<T, 4>& high);

//! Returns element-wise ceiled vector.
template <typename T>
Vector<T, 4> Ceil(const Vector<T, 4>& a);

//! Returns element-wise floored vector.
template <typename T>
Vector<T, 4> Floor(const Vector<T, 4>& a);

//! Float-type 4D vector.
using Vector4F = Vector4<float>;

//! Double-type 4D vector.
using Vector4D = Vector4<double>;

//! Returns float-type zero vector.
template <>
constexpr Vector4F Zero<Vector4F>()
{
    return Vector4F(0.f, 0.f, 0.f, 0.f);
}

//! Returns double-type zero vector.
template <>
constexpr Vector4D Zero<Vector4D>()
{
    return Vector4D(0.0, 0.0, 0.0, 0.0);
}

//! Returns the type of the value itself.
template <typename T>
struct ScalarType<Vector<T, 4>>
{
    typedef T value;
};

//! Computes monotonic Catmull-Rom interpolation.
template <typename T>
Vector<T, 4> MonotonicCatmullRom(const Vector<T, 4>& v0, const Vector<T, 4>& v1,
                                 const Vector<T, 4>& v2, const Vector<T, 4>& v3,
                                 T f);
}  // namespace CubbyFlow

#include <Core/Vector/Vector4-Impl.hpp>

#endif