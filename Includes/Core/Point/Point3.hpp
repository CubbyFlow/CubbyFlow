// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_POINT3_HPP
#define CUBBYFLOW_POINT3_HPP

#include <Core/Point/Point.hpp>
#include <Core/Point/Point2.hpp>
#include <Core/Utils/Constants.hpp>

namespace CubbyFlow
{
//!
//! \brief 3-D point class.
//!
//! This class defines simple 3-D point data.
//!
//! \tparam T - Type of the element
//!
template <typename T>
class Point<T, 3> final
{
 public:
    static_assert(std::is_arithmetic<T>::value,
                  "Point only can be instantiated with arithmetic types");

    //! Constructs default point (0, 0, 0).
    Point();

    //! Constructs point with given parameters \p _x, \p _y, and \p _z.
    Point(T _x, T _y, T _z);

    //! Constructs point with a 2-D point and a scalar.
    Point(const Point2<T>& pt, T _z);

    //! Constructs point with initializer list.
    template <typename U>
    Point(const std::initializer_list<U>& list);

    //! Copy constructor.
    Point(const Point& pt);

    //! Move constructor.
    Point(Point&& pt) noexcept;

    //! Default destructor.
    ~Point() = default;

    //! Copy assignment operator.
    Point& operator=(const Point& v);

    //! Move assignment operator.
    Point& operator=(Point&& v) noexcept;

    //! Set all x, y, and z components to \p s.
    void Set(T s);

    //! Set x, y, and z components with given parameters.
    void Set(T x, T y, T z);

    //! Set x, y, and z components with given \p pt.x, \p pt.y, and \p z.
    void Set(const Point2<T>& pt, T z);

    //! Set x, y, and z components with given initializer list.
    template <typename U>
    void Set(const std::initializer_list<U>& list);

    //! Set x, y, and z with other point \p pt.
    void Set(const Point& v);

    //! Set both x, y, and z to zero.
    void SetZero();

    //! Computes this + (v, v, v).
    [[nodiscard]] Point Add(T v) const;

    //! Computes this + (v.x, v.y, v.z).
    [[nodiscard]] Point Add(const Point& v) const;

    //! Computes this - (v, v, v).
    [[nodiscard]] Point Sub(T v) const;

    //! Computes this - (v.x, v.y, v.z).
    [[nodiscard]] Point Sub(const Point& v) const;

    //! Computes this * (v, v, v).
    [[nodiscard]] Point Mul(T v) const;

    //! Computes this * (v.x, v.y, v.z).
    [[nodiscard]] Point Mul(const Point& v) const;

    //! Computes this / (v, v, v).
    [[nodiscard]] Point Div(T v) const;

    //! Computes this / (v.x, v.y, v.z).
    [[nodiscard]] Point Div(const Point& v) const;

    //! Computes (v, v, v) + this.
    [[nodiscard]] Point RAdd(T v) const;

    //! Computes (v.x, v.y, v.z) + this.
    [[nodiscard]] Point RAdd(const Point& v) const;

    //! Computes (v, v, v) - this.
    [[nodiscard]] Point RSub(T v) const;

    //! Computes (v.x, v.y, v.z) - this.
    [[nodiscard]] Point RSub(const Point& v) const;

    //! Computes (v, v, v) * this.
    [[nodiscard]] Point RMul(T v) const;

    //! Computes (v.x, v.y, v.z) * this.
    [[nodiscard]] Point RMul(const Point& v) const;

    //! Computes (v, v, v) / this.
    [[nodiscard]] Point RDiv(T v) const;

    //! Computes (v.x, v.y, v.z) / this.
    [[nodiscard]] Point RDiv(const Point& v) const;

    //! Computes this += (v, v, v).
    void IAdd(T v);

    //! Computes this += (v.x, v.y, v.z).
    void IAdd(const Point& v);

    //! Computes this -= (v, v, v).
    void ISub(T v);

    //! Computes this -= (v.x, v.y, v.z).
    void ISub(const Point& v);

    //! Computes this *= (v, v, v).
    void IMul(T v);

    //! Computes this *= (v.x, v.y, v.z).
    void IMul(const Point& v);

    //! Computes this /= (v, v, v).
    void IDiv(T v);

    //! Computes this /= (v.x, v.y, v.z).
    void IDiv(const Point& v);

    //! Returns const reference to the \p i -th element of the point.
    [[nodiscard]] const T& At(size_t i) const;

    //! Returns reference to the \p i -th element of the point.
    [[nodiscard]] T& At(size_t i);

    //! Returns the sum of all the components (i.e. x + y).
    [[nodiscard]] T Sum() const;

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

    //! Returns the index of the subdominant axis.
    [[nodiscard]] size_t SubdominantAxis() const;

    //! Returns a point with different value type.
    template <typename U>
    [[nodiscard]] Point<U, 3> CastTo() const;

    //! Returns true if \p other is the same as this point.
    [[nodiscard]] bool IsEqual(const Point& other) const;

    //! Returns reference to the \p i -th element of the point.
    T& operator[](size_t i);

    //! Returns const reference to the \p i -th element of the point.
    const T& operator[](size_t i) const;

    //! Set x, y, and z components with given initializer list.
    Point& operator=(const std::initializer_list<T>& list);

    //! Computes this += (v, v, v)
    Point& operator+=(T v);

    //! Computes this += (v.x, v.y, v.z)
    Point& operator+=(const Point& v);

    //! Computes this -= (v, v, v)
    Point& operator-=(T v);

    //! Computes this -= (v.x, v.y, v.z)
    Point& operator-=(const Point& v);

    //! Computes this *= (v, v, v)
    Point& operator*=(T v);

    //! Computes this *= (v.x, v.y, v.z)
    Point& operator*=(const Point& v);

    //! Computes this /= (v, v, v)
    Point& operator/=(T v);

    //! Computes this /= (v.x, v.y, v.z)
    Point& operator/=(const Point& v);

    //! Returns true if \p other is the same as this point.
    bool operator==(const Point& v) const;

    //! Returns true if \p other is the not same as this point.
    bool operator!=(const Point& v) const;

    //! X (or the first) component of the point.
    T x;

    //! Y (or the second) component of the point.
    T y;

    //! Z (or the third) component of the point.
    T z;
};

//! Positive sign operator.
template <typename T>
Point<T, 3> operator+(const Point<T, 3>& a);

//! Negative sign operator.
template <typename T>
Point<T, 3> operator-(const Point<T, 3>& a);

//! Computes (a, a, a) + (b.x, b.y, b.z).
template <typename T>
Point<T, 3> operator+(T a, const Point<T, 3>& b);

//! Computes (a.x, a.y, a.z) + (b.x, b.y, b.z).
template <typename T>
Point<T, 3> operator+(const Point<T, 3>& a, const Point<T, 3>& b);

//! Computes (a.x, a.y, a.z) - (b, b, b).
template <typename T>
Point<T, 3> operator-(const Point<T, 3>& a, T b);

//! Computes (a, a, a) - (b.x, b.y, b.z).
template <typename T>
Point<T, 3> operator-(T a, const Point<T, 3>& b);

//! Computes (a.x, a.y, a.z) - (b.x, b.y, b.z).
template <typename T>
Point<T, 3> operator-(const Point<T, 3>& a, const Point<T, 3>& b);

//! Computes (a.x, a.y, a.z) * (b, b, b).
template <typename T>
Point<T, 3> operator*(const Point<T, 3>& a, T b);

//! Computes (a, a, a) * (b.x, b.y, b.z).
template <typename T>
Point<T, 3> operator*(T a, const Point<T, 3>& b);

//! Computes (a.x, a.y, a.z) * (b.x, b.y, b.z).
template <typename T>
Point<T, 3> operator*(const Point<T, 3>& a, const Point<T, 3>& b);

//! Computes (a.x, a.y, a.z) / (b, b, b).
template <typename T>
Point<T, 3> operator/(const Point<T, 3>& a, T b);

//! Computes (a, a, a) / (b.x, b.y, b.z).
template <typename T>
Point<T, 3> operator/(T a, const Point<T, 3>& b);

//! Computes (a.x, a.y, a.z) / (b.x, b.y, b.z).
template <typename T>
Point<T, 3> operator/(const Point<T, 3>& a, const Point<T, 3>& b);

//! Returns element-wise min point.
template <typename T>
Point<T, 3> Min(const Point<T, 3>& a, const Point<T, 3>& b);

//! Returns element-wise max point.
template <typename T>
Point<T, 3> Max(const Point<T, 3>& a, const Point<T, 3>& b);

//! Returns element-wise clamped point.
template <typename T>
Point<T, 3> Clamp(const Point<T, 3>& v, const Point<T, 3>& low,
                  const Point<T, 3>& high);

//! Returns element-wise ceiled point.
template <typename T>
Point<T, 3> Ceil(const Point<T, 3>& a);

//! Returns element-wise floored point.
template <typename T>
Point<T, 3> Floor(const Point<T, 3>& a);

//! Type alias for three dimensional point.
template <typename T>
using Point3 = Point<T, 3>;

//! Float-type 3D point.
using Point3F = Point3<float>;

//! Double-type 3D point.
using Point3D = Point3<double>;

//! Integer-type 3D point.
using Point3I = Point3<ssize_t>;

//! Unsigned integer-type 3D point.
using Point3UI = Point3<size_t>;
}  // namespace CubbyFlow

#include <Core/Point/Point3-Impl.hpp>

#endif