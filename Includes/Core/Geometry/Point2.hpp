// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_POINT2_HPP
#define CUBBYFLOW_POINT2_HPP

#include <Core/Geometry/Point.hpp>
#include <Core/Utils/Constants.hpp>

namespace CubbyFlow
{
//!
//! \brief 2-D point class.
//!
//! This class defines simple 2-D point data.
//!
//! \tparam T - Type of the element
//!
template <typename T>
class Point<T, 2> final
{
 public:
    static_assert(std::is_arithmetic<T>::value,
                  "Point only can be instantiated with arithmetic types");

    //! Constructs default point (0, 0).
    constexpr Point() : x{ 0 }, y{ 0 }
    {
        // Do nothing
    }

    //! Constructs point with given parameters \p _x and \p _y.
    constexpr Point(T _x, T _y) : x{ _x }, y{ _y }
    {
        // Do nothing
    }

    //! Constructs point with initializer list.
    template <typename U>
    Point(const std::initializer_list<U>& list);

    //! Copy constructor.
    constexpr Point(const Point& pt) : x{ pt.x }, y{ pt.y }
    {
        // Do nothing
    }

    //! Move constructor.
    constexpr Point(Point&& pt) noexcept : x{ pt.x }, y{ pt.y }
    {
        // Do nothing
    }

    //! Default destructor.
    ~Point() = default;

    //! Copy assignment operator.
    Point& operator=(const Point& v);

    //! Move assignment operator.
    Point& operator=(Point&& v) noexcept;

    //! Set both x and y components to \p s.
    void Set(T s);

    //! Set x and y components with given parameters.
    void Set(T x, T y);

    //! Set x and y components with given initializer list.
    template <typename U>
    void Set(const std::initializer_list<U>& list);

    //! Set x and y with other point \p v.
    void Set(const Point& v);

    //! Set both x and y to zero.
    void SetZero();

    //! Computes this + (v, v).
    [[nodiscard]] Point Add(T v) const;

    //! Computes this + (v.x, v.y).
    [[nodiscard]] Point Add(const Point& v) const;

    //! Computes this - (v, v).
    [[nodiscard]] Point Sub(T v) const;

    //! Computes this - (v.x, v.y).
    [[nodiscard]] Point Sub(const Point& v) const;

    //! Computes this * (v, v).
    [[nodiscard]] Point Mul(T v) const;

    //! Computes this * (v.x, v.y).
    [[nodiscard]] Point Mul(const Point& v) const;

    //! Computes this / (v, v).
    [[nodiscard]] Point Div(T v) const;

    //! Computes this / (v.x, v.y).
    [[nodiscard]] Point Div(const Point& v) const;

    //! Computes (v, v) + this.
    [[nodiscard]] Point RAdd(T v) const;

    //! Computes (v.x, v.y) + this.
    [[nodiscard]] Point RAdd(const Point& v) const;

    //! Computes (v, v) - this.
    [[nodiscard]] Point RSub(T v) const;

    //! Computes (v.x, v.y) - this.
    [[nodiscard]] Point RSub(const Point& v) const;

    //! Computes (v, v) * this.
    [[nodiscard]] Point RMul(T v) const;

    //! Computes (v.x, v.y) * this.
    [[nodiscard]] Point RMul(const Point& v) const;

    //! Computes (v, v) / this.
    [[nodiscard]] Point RDiv(T v) const;

    //! Computes (v.x, v.y) / this.
    [[nodiscard]] Point RDiv(const Point& v) const;

    //! Computes this += (v, v).
    void IAdd(T v);

    //! Computes this += (v.x, v.y).
    void IAdd(const Point& v);

    //! Computes this -= (v, v).
    void ISub(T v);

    //! Computes this -= (v.x, v.y).
    void ISub(const Point& v);

    //! Computes this *= (v, v).
    void IMul(T v);

    //! Computes this *= (v.x, v.y).
    void IMul(const Point& v);

    //! Computes this /= (v, v).
    void IDiv(T v);

    //! Computes this /= (v.x, v.y).
    void IDiv(const Point& v);

    //! Returns const reference to the \p i -th element of the point.
    [[nodiscard]] const T& At(size_t i) const;

    //! Returns reference to the \p i -th element of the point.
    [[nodiscard]] T& At(size_t i);

    //! Returns the sum of all the components (i.e. x + y).
    [[nodiscard]] T Sum() const;

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

    //! Returns the index of the subdominant axis.
    [[nodiscard]] size_t SubdominantAxis() const;

    //! Returns a point with different value type.
    template <typename U>
    [[nodiscard]] Point<U, 2> CastTo() const;

    //! Returns true if \p other is the same as this point.
    [[nodiscard]] bool IsEqual(const Point& other) const;

    //! Returns reference to the \p i -th element of the point.
    T& operator[](size_t i);

    //! Returns const reference to the \p i -th element of the point.
    const T& operator[](size_t i) const;

    //! Set x and y components with given initializer list.
    Point& operator=(const std::initializer_list<T>& list);

    //! Computes this += (v, v)
    Point& operator+=(T v);

    //! Computes this += (v.x, v.y)
    Point& operator+=(const Point& v);

    //! Computes this -= (v, v)
    Point& operator-=(T v);

    //! Computes this -= (v.x, v.y)
    Point& operator-=(const Point& v);

    //! Computes this *= (v, v)
    Point& operator*=(T v);

    //! Computes this *= (v.x, v.y)
    Point& operator*=(const Point& v);

    //! Computes this /= (v, v)
    Point& operator/=(T v);

    //! Computes this /= (v.x, v.y)
    Point& operator/=(const Point& v);

    //! Returns true if \p other is the same as this point.
    bool operator==(const Point& v) const;

    //! Returns true if \p other is the not same as this point.
    bool operator!=(const Point& v) const;

    //! X (or the first) component of the point.
    T x;

    //! Y (or the second) component of the point.
    T y;
};

//! Positive sign operator.
template <typename T>
Point<T, 2> operator+(const Point<T, 2>& a);

//! Negative sign operator.
template <typename T>
Point<T, 2> operator-(const Point<T, 2>& a);

//! Computes (a, a) + (b.x, b.y).
template <typename T>
Point<T, 2> operator+(T a, const Point<T, 2>& b);

//! Computes (a.x, a.y) + (b.x, b.y).
template <typename T>
Point<T, 2> operator+(const Point<T, 2>& a, const Point<T, 2>& b);

//! Computes (a.x, a.y) - (b, b).
template <typename T>
Point<T, 2> operator-(const Point<T, 2>& a, T b);

//! Computes (a, a) - (b.x, b.y).
template <typename T>
Point<T, 2> operator-(T a, const Point<T, 2>& b);

//! Computes (a.x, a.y) - (b.x, b.y).
template <typename T>
Point<T, 2> operator-(const Point<T, 2>& a, const Point<T, 2>& b);

//! Computes (a.x, a.y) * (b, b).
template <typename T>
Point<T, 2> operator*(const Point<T, 2>& a, T b);

//! Computes (a, a) * (b.x, b.y).
template <typename T>
Point<T, 2> operator*(T a, const Point<T, 2>& b);

//! Computes (a.x, a.y) * (b.x, b.y).
template <typename T>
Point<T, 2> operator*(const Point<T, 2>& a, const Point<T, 2>& b);

//! Computes (a.x, a.y) / (b, b).
template <typename T>
Point<T, 2> operator/(const Point<T, 2>& a, T b);

//! Computes (a, a) / (b.x, b.y).
template <typename T>
Point<T, 2> operator/(T a, const Point<T, 2>& b);

//! Computes (a.x, a.y) / (b.x, b.y).
template <typename T>
Point<T, 2> operator/(const Point<T, 2>& a, const Point<T, 2>& b);

//! Returns element-wise min point: (min(a.x, b.x), min(a.y, b.y)).
template <typename T>
Point<T, 2> Min(const Point<T, 2>& a, const Point<T, 2>& b);

//! Returns element-wise max point: (max(a.x, b.x), max(a.y, b.y)).
template <typename T>
Point<T, 2> Max(const Point<T, 2>& a, const Point<T, 2>& b);

//! Returns element-wise clamped point.
template <typename T>
Point<T, 2> Clamp(const Point<T, 2>& v, const Point<T, 2>& low,
                  const Point<T, 2>& high);

//! Returns element-wise ceiled point.
template <typename T>
Point<T, 2> Ceil(const Point<T, 2>& a);

//! Returns element-wise floored point.
template <typename T>
Point<T, 2> Floor(const Point<T, 2>& a);

//! Type alias for two dimensional point.
template <typename T>
using Point2 = Point<T, 2>;

//! Float-type 2D point.
using Point2F = Point2<float>;

//! Double-type 2D point.
using Point2D = Point2<double>;

//! Integer-type 2D point.
using Point2I = Point2<ssize_t>;

//! Unsigned integer-type 2D point.
using Point2UI = Point2<size_t>;
}  // namespace CubbyFlow

#include <Core/Geometry/Point2-Impl.hpp>

#endif