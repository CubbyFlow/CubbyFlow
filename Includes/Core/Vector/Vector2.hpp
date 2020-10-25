// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_VECTOR2_HPP
#define CUBBYFLOW_VECTOR2_HPP

#include <Core/Utils/Constants.hpp>
#include <Core/Utils/TypeHelpers.hpp>
#include <Core/Vector/Vector.hpp>

namespace CubbyFlow
{
//!
//! \brief 2-D vector class.
//!
//! This class defines simple 2-D vector data.
//!
//! \tparam T - Type of the element
//!
template <typename T>
class Vector<T, 2> final
{
 public:
    static_assert(std::is_floating_point<T>::value,
                  "Vector only can be instantiated with floating point types");

    //! Constructs default vector (0, 0).
    constexpr Vector() : x{ 0 }, y{ 0 }
    {
        // Do nothing
    }

    //! Constructs vector with given parameters \p _x and \p _y.
    constexpr Vector(T _x, T _y) : x{ _x }, y{ _y }
    {
        // Do nothing
    }

    //! Constructs vector with initializer list.
    template <typename U>
    Vector(const std::initializer_list<U>& list);

    //! Copy constructor.
    constexpr Vector(const Vector& v) : x{ v.x }, y{ v.y }
    {
        // Do nothing
    }

    //! Move constructor.
    constexpr Vector(Vector&& v) noexcept : x{ v.x }, y{ v.y }
    {
        // Do nothing
    }

    //! Default destructor.
    ~Vector() = default;

    //! Copy assignment operator.
    Vector& operator=(const Vector& v);

    //! Move assignment operator.
    Vector& operator=(Vector&& v) noexcept;

    //! Set both x and y components to \p s.
    void Set(T s);

    //! Set x and y components with given parameters.
    void Set(T x, T y);

    //! Set x and y components with given initializer list.
    template <typename U>
    void Set(const std::initializer_list<U>& list);

    //! Set x and y with other vector \p v.
    void Set(const Vector& v);

    //! Set both x and y to zero.
    void SetZero();

    //! Normalizes this vector.
    void Normalize();

    //! Computes this + (v, v).
    [[nodiscard]] Vector Add(T v) const;

    //! Computes this + (v.x, v.y).
    [[nodiscard]] Vector Add(const Vector& v) const;

    //! Computes this - (v, v).
    [[nodiscard]] Vector Sub(T v) const;

    //! Computes this - (v.x, v.y).
    [[nodiscard]] Vector Sub(const Vector& v) const;

    //! Computes this * (v, v).
    [[nodiscard]] Vector Mul(T v) const;

    //! Computes this * (v.x, v.y).
    [[nodiscard]] Vector Mul(const Vector& v) const;

    //! Computes this / (v, v).
    [[nodiscard]] Vector Div(T v) const;

    //! Computes this / (v.x, v.y).
    [[nodiscard]] Vector Div(const Vector& v) const;

    //! Computes dot product.
    [[nodiscard]] T Dot(const Vector& v) const;

    //! Computes cross product.
    [[nodiscard]] T Cross(const Vector& v) const;

    //! Computes (v, v) - this.
    [[nodiscard]] Vector RSub(T v) const;

    //! Computes (v.x, v.y) - this.
    [[nodiscard]] Vector RSub(const Vector& v) const;

    //! Computes (v, v) / this.
    [[nodiscard]] Vector RDiv(T v) const;

    //! Computes (v.x, v.y) / this.
    [[nodiscard]] Vector RDiv(const Vector& v) const;

    //! Computes \p v cross this.
    [[nodiscard]] T RCross(const Vector& v) const;

    //! Computes this += (v, v).
    void IAdd(T v);

    //! Computes this += (v.x, v.y).
    void IAdd(const Vector& v);

    //! Computes this -= (v, v).
    void ISub(T v);

    //! Computes this -= (v.x, v.y).
    void ISub(const Vector& v);

    //! Computes this *= (v, v).
    void IMul(T v);

    //! Computes this *= (v.x, v.y).
    void IMul(const Vector& v);

    //! Computes this /= (v, v).
    void IDiv(T v);

    //! Computes this /= (v.x, v.y).
    void IDiv(const Vector& v);

    //! Returns const reference to the \p i -th element of the vector.
    [[nodiscard]] const T& At(size_t i) const;

    //! Returns reference to the \p i -th element of the vector.
    [[nodiscard]] T& At(size_t i);

    //! Returns the sum of all the components (i.e. x + y).
    [[nodiscard]] T Sum() const;

    //! Returns the average of all the components.
    [[nodiscard]] T Avg() const;

    //! Returns the minimum value among x and y.
    [[nodiscard]] T Min() const;

    //! Returns the maximum value among x and y.
    [[nodiscard]] T Max() const;

    //! Returns the absolute minimum value among x and y.
    [[nodiscard]] T AbsMin() const;

    //! Returns the absolute maximum value among x and y.
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

    //! Returns the reflection vector to the surface with given surface normal.
    [[nodiscard]] Vector Reflected(const Vector& normal) const;

    //! Returns the projected vector to the surface with given surface normal.
    [[nodiscard]] Vector Projected(const Vector& normal) const;

    //! Returns the tangential vector for this vector.
    [[nodiscard]] Vector Tangential() const;

    //! Returns a vector with different value type.
    template <typename U>
    Vector<U, 2> CastTo() const;

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

    //! Computes this += (v, v)
    Vector& operator+=(T v);

    //! Computes this += (v.x, v.y)
    Vector& operator+=(const Vector& v);

    //! Computes this -= (v, v)
    Vector& operator-=(T v);

    //! Computes this -= (v.x, v.y)
    Vector& operator-=(const Vector& v);

    //! Computes this *= (v, v)
    Vector& operator*=(T v);

    //! Computes this *= (v.x, v.y)
    Vector& operator*=(const Vector& v);

    //! Computes this /= (v, v)
    Vector& operator/=(T v);

    //! Computes this /= (v.x, v.y)
    Vector& operator/=(const Vector& v);

    //! Returns true if \p other is the same as this vector.
    bool operator==(const Vector& v) const;

    //! Returns true if \p other is the not same as this vector.
    bool operator!=(const Vector& v) const;

    //! X (or the first) component of the vector.
    T x;

    //! Y (or the second) component of the vector.
    T y;
};

//! Type alias for two dimensional vector.
template <typename T>
using Vector2 = Vector<T, 2>;

//! Positive sign operator.
template <typename T>
Vector<T, 2> operator+(const Vector<T, 2>& a);

//! Negative sign operator.
template <typename T>
Vector<T, 2> operator-(const Vector<T, 2>& a);

//! Computes (a, a) + (b.x, b.y).
template <typename T>
Vector<T, 2> operator+(T a, const Vector<T, 2>& b);

//! Computes (a.x, a.y) + (b.x, b.y).
template <typename T>
Vector<T, 2> operator+(const Vector<T, 2>& a, const Vector<T, 2>& b);

//! Computes (a.x, a.y) - (b, b).
template <typename T>
Vector<T, 2> operator-(const Vector<T, 2>& a, T b);

//! Computes (a, a) - (b.x, b.y).
template <typename T>
Vector<T, 2> operator-(T a, const Vector<T, 2>& b);

//! Computes (a.x, a.y) - (b.x, b.y).
template <typename T>
Vector<T, 2> operator-(const Vector<T, 2>& a, const Vector<T, 2>& b);

//! Computes (a.x, a.y) * (b, b).
template <typename T>
Vector<T, 2> operator*(const Vector<T, 2>& a, T b);

//! Computes (a, a) * (b.x, b.y).
template <typename T>
Vector<T, 2> operator*(T a, const Vector<T, 2>& b);

//! Computes (a.x, a.y) * (b.x, b.y).
template <typename T>
Vector<T, 2> operator*(const Vector<T, 2>& a, const Vector<T, 2>& b);

//! Computes (a.x, a.y) / (b, b).
template <typename T>
Vector<T, 2> operator/(const Vector<T, 2>& a, T b);

//! Computes (a, a) / (b.x, b.y).
template <typename T>
Vector<T, 2> operator/(T a, const Vector<T, 2>& b);

//! Computes (a.x, a.y) / (b.x, b.y).
template <typename T>
Vector<T, 2> operator/(const Vector<T, 2>& a, const Vector<T, 2>& b);

//! Returns element-wise min vector: (min(a.x, b.x), min(a.y, b.y)).
template <typename T>
Vector<T, 2> Min(const Vector<T, 2>& a, const Vector<T, 2>& b);

//! Returns element-wise max vector: (max(a.x, b.x), max(a.y, b.y)).
template <typename T>
Vector<T, 2> Max(const Vector<T, 2>& a, const Vector<T, 2>& b);

//! Returns element-wise clamped vector.
template <typename T>
Vector<T, 2> Clamp(const Vector<T, 2>& v, const Vector<T, 2>& low,
                   const Vector<T, 2>& high);

//! Returns element-wise ceiled vector.
template <typename T>
Vector<T, 2> Ceil(const Vector<T, 2>& a);

//! Returns element-wise floored vector.
template <typename T>
Vector<T, 2> Floor(const Vector<T, 2>& a);

//! Float-type 2D vector.
using Vector2F = Vector2<float>;

//! Double-type 2D vector.
using Vector2D = Vector2<double>;

//! Returns float-type zero vector.
template <>
constexpr Vector<float, 2> Zero<Vector<float, 2>>()
{
    return Vector<float, 2>(0.f, 0.f);
}

//! Returns double-type zero vector.
template <>
constexpr Vector<double, 2> Zero<Vector<double, 2>>()
{
    return Vector<double, 2>(0.0, 0.0);
}

//! Returns the type of the value itself.
template <typename T>
struct ScalarType<Vector<T, 2>>
{
    typedef T value;
};

//! Computes monotonic Catmull-Rom interpolation.
template <typename T>
Vector<T, 2> MonotonicCatmullRom(const Vector<T, 2>& v0, const Vector<T, 2>& v1,
                                 const Vector<T, 2>& v2, const Vector<T, 2>& v3,
                                 T f);
}  // namespace CubbyFlow

#include <Core/Vector/Vector2-Impl.hpp>

#endif