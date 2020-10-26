// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_VECTOR3_HPP
#define CUBBYFLOW_VECTOR3_HPP

#include <Core/Vector/Vector2.hpp>

#include <limits>

namespace CubbyFlow
{
//!
//! \brief 3-D vector class.
//!
//! This class defines simple 3-D vector data.
//!
//! \tparam T - Type of the element
//!
template <typename T>
class Vector<T, 3> final
{
 public:
    static_assert(std::is_floating_point<T>::value,
                  "Vector only can be instantiated with floating point types");

    //! Constructs default vector (0, 0, 0).
    constexpr Vector() : x{ 0 }, y{ 0 }, z{ 0 }
    {
        // Do nothing
    }

    //! Constructs vector with given parameters \p _x, \p _y, and \p _z.
    constexpr Vector(T _x, T _y, T _z) : x{ _x }, y{ _y }, z{ _z }
    {
        // Do nothing
    }

    //! Constructs vector with a 2-D vector and a scalar.
    constexpr Vector(const Vector2<T>& v, T _z) : x{ v.x }, y{ v.y }, z{ _z }
    {
        // Do nothing
    }

    //! Constructs vector with initializer list.
    template <typename U>
    Vector(const std::initializer_list<U>& list);

    //! Copy constructor.
    constexpr Vector(const Vector& v) : x{ v.x }, y{ v.y }, z{ v.z }
    {
        // Do nothing
    }

    //! Move constructor.
    constexpr Vector(Vector&& v) noexcept : x{ v.x }, y{ v.y }, z{ v.z }
    {
        // Do nothing
    }

    //! Default destructor.
    ~Vector() = default;

    //! Copy assignment operator.
    Vector& operator=(const Vector& v);

    //! Move assignment operator.
    Vector& operator=(Vector&& v) noexcept;

    //! Set all x, y, and z components to \p s.
    void Set(T s);

    //! Set x, y, and z components with given parameters.
    void Set(T x, T y, T z);

    //! Set x, y, and z components with given \p pt.x, \p pt.y, and \p z.
    void Set(const Vector2<T>& pt, T z);

    //! Set x, y, and z components with given initializer list.
    template <typename U>
    void Set(const std::initializer_list<U>& list);

    //! Set x, y, and z with other vector \p v.
    void Set(const Vector& v);

    //! Set all x, y, and z to zero.
    void SetZero();

    //! Normalizes this vector.
    void Normalize();

    //! Computes this + (v, v, v).
    [[nodiscard]] Vector Add(T v) const;

    //! Computes this + (v.x, v.y, v.z).
    [[nodiscard]] Vector Add(const Vector& v) const;

    //! Computes this - (v, v, v).
    [[nodiscard]] Vector Sub(T v) const;

    //! Computes this - (v.x, v.y, v.z).
    [[nodiscard]] Vector Sub(const Vector& v) const;

    //! Computes this * (v, v, v).
    [[nodiscard]] Vector Mul(T v) const;

    //! Computes this * (v.x, v.y, v.z).
    [[nodiscard]] Vector Mul(const Vector& v) const;

    //! Computes this / (v, v, v).
    [[nodiscard]] Vector Div(T v) const;

    //! Computes this / (v.x, v.y, v.z).
    [[nodiscard]] Vector Div(const Vector& v) const;

    //! Computes dot product.
    [[nodiscard]] T Dot(const Vector& v) const;

    //! Computes cross product.
    [[nodiscard]] Vector Cross(const Vector& v) const;

    //! Computes (v, v, v) - this.
    [[nodiscard]] Vector RSub(T v) const;

    //! Computes (v.x, v.y, v.z) - this.
    [[nodiscard]] Vector RSub(const Vector& v) const;

    //! Computes (v, v, v) / this.
    [[nodiscard]] Vector RDiv(T v) const;

    //! Computes (v.x, v.y, v.z) / this.
    [[nodiscard]] Vector RDiv(const Vector& v) const;

    //! Computes \p v cross this.
    [[nodiscard]] Vector RCross(const Vector& v) const;

    //! Computes this += (v, v, v).
    void IAdd(T v);

    //! Computes this += (v.x, v.y, v.z).
    void IAdd(const Vector& v);

    //! Computes this -= (v, v, v).
    void ISub(T v);

    //! Computes this -= (v.x, v.y, v.z).
    void ISub(const Vector& v);

    //! Computes this *= (v, v, v).
    void IMul(T v);

    //! Computes this *= (v.x, v.y, v.z).
    void IMul(const Vector& v);

    //! Computes this /= (v, v, v).
    void IDiv(T v);

    //! Computes this /= (v.x, v.y, v.z).
    void IDiv(const Vector& v);

    //! Returns const reference to the \p i -th element of the vector.
    [[nodiscard]] const T& At(size_t i) const;

    //! Returns reference to the \p i -th element of the vector.
    [[nodiscard]] T& At(size_t i);

    //! Returns the sum of all the components (i.e. x + y + z).
    [[nodiscard]] T Sum() const;

    //! Returns the average of all the components.
    [[nodiscard]] T Avg() const;

    //! Returns the minimum value among x, y, and z.
    [[nodiscard]] T Min() const;

    //! Returns the maximum value among x, y, and z.
    [[nodiscard]] T Max() const;

    //! Returns the absolute minimum value among x, y, and z.
    [[nodiscard]] T AbsMin() const;

    //! Returns the absolute maximum value among x, y, and z.
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
    [[nodiscard]] std::tuple<Vector<T, 3>, Vector<T, 3>> Tangential() const;

    //! Returns a vector with different value type.
    template <typename U>
    [[nodiscard]] Vector<U, 3> CastTo() const;

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

    //! Computes this += (v, v, v)
    Vector& operator+=(T v);

    //! Computes this += (v.x, v.y, v.z)
    Vector& operator+=(const Vector& v);

    //! Computes this -= (v, v, v)
    Vector& operator-=(T v);

    //! Computes this -= (v.x, v.y, v.z)
    Vector& operator-=(const Vector& v);

    //! Computes this *= (v, v, v)
    Vector& operator*=(T v);

    //! Computes this *= (v.x, v.y, v.z)
    Vector& operator*=(const Vector& v);

    //! Computes this /= (v, v, v)
    Vector& operator/=(T v);

    //! Computes this /= (v.x, v.y, v.z)
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
};

//! Type alias for three dimensional vector.
template <typename T>
using Vector3 = Vector<T, 3>;

//! Positive sign operator.
template <typename T>
Vector<T, 3> operator+(const Vector<T, 3>& a);

//! Negative sign operator.
template <typename T>
Vector<T, 3> operator-(const Vector<T, 3>& a);

//! Computes (a, a, a) + (b.x, b.y, b.z).
template <typename T>
Vector<T, 3> operator+(T a, const Vector<T, 3>& b);

//! Computes (a.x, a.y, a.z) + (b.x, b.y, b.z).
template <typename T>
Vector<T, 3> operator+(const Vector<T, 3>& a, const Vector<T, 3>& b);

//! Computes (a.x, a.y, a.z) - (b, b, b).
template <typename T>
Vector<T, 3> operator-(const Vector<T, 3>& a, T b);

//! Computes (a, a, a) - (b.x, b.y, b.z).
template <typename T>
Vector<T, 3> operator-(T a, const Vector<T, 3>& b);

//! Computes (a.x, a.y, a.z) - (b.x, b.y, b.z).
template <typename T>
Vector<T, 3> operator-(const Vector<T, 3>& a, const Vector<T, 3>& b);

//! Computes (a.x, a.y, a.z) * (b, b, b).
template <typename T>
Vector<T, 3> operator*(const Vector<T, 3>& a, T b);

//! Computes (a, a, a) * (b.x, b.y, b.z).
template <typename T>
Vector<T, 3> operator*(T a, const Vector<T, 3>& b);

//! Computes (a.x, a.y, a.z) * (b.x, b.y, b.z).
template <typename T>
Vector<T, 3> operator*(const Vector<T, 3>& a, const Vector<T, 3>& b);

//! Computes (a.x, a.y, a.z) / (b, b, b).
template <typename T>
Vector<T, 3> operator/(const Vector<T, 3>& a, T b);

//! Computes (a, a, a) / (b.x, b.y, b.z).
template <typename T>
Vector<T, 3> operator/(T a, const Vector<T, 3>& b);

//! Computes (a.x, a.y, a.z) / (b.x, b.y, b.z).
template <typename T>
Vector<T, 3> operator/(const Vector<T, 3>& a, const Vector<T, 3>& b);

//! Returns element-wise min vector.
template <typename T>
Vector<T, 3> Min(const Vector<T, 3>& a, const Vector<T, 3>& b);

//! Returns element-wise max vector.
template <typename T>
Vector<T, 3> Max(const Vector<T, 3>& a, const Vector<T, 3>& b);

//! Returns element-wise clamped vector.
template <typename T>
Vector<T, 3> Clamp(const Vector<T, 3>& v, const Vector<T, 3>& low,
                   const Vector<T, 3>& high);

//! Returns element-wise ceiled vector.
template <typename T>
Vector<T, 3> Ceil(const Vector<T, 3>& a);

//! Returns element-wise floored vector.
template <typename T>
Vector<T, 3> Floor(const Vector<T, 3>& a);

//! Float-type 3D vector.
using Vector3F = Vector3<float>;

//! Double-type 3D vector.
using Vector3D = Vector3<double>;

//! Returns float-type zero vector.
template <>
constexpr Vector3F Zero<Vector3F>()
{
    return Vector3F(0.f, 0.f, 0.f);
}

//! Returns double-type zero vector.
template <>
constexpr Vector3D Zero<Vector3D>()
{
    return Vector3D(0.0, 0.0, 0.0);
}

//! Returns the type of the value itself.
template <typename T>
struct ScalarType<Vector<T, 3>>
{
    typedef T value;
};

//! Computes monotonic Catmull-Rom interpolation.
template <typename T>
Vector3<T> MonotonicCatmullRom(const Vector3<T>& v0, const Vector3<T>& v1,
                               const Vector3<T>& v2, const Vector3<T>& v3, T f);
}  // namespace CubbyFlow

#include <Core/Vector/Vector3-Impl.hpp>

#endif