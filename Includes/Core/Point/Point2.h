/*************************************************************************
> File Name: Point2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D point class.
> Created Time: 2017/02/01
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_POINT2_H
#define CUBBYFLOW_POINT2_H

#include <Core/Point/Point.h>
#include <Core/Utils/Constants.h>

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
		static_assert(std::is_arithmetic<T>::value,	"Point only can be instantiated with arithmetic types");

		//! X (or the first) component of the point.
		T x;

		//! Y (or the second) component of the point.
		T y;

		// MARK: Constructors
		//! Constructs default point (0, 0).
		constexpr Point() : x(0), y(0)
		{
			// Do nothing
		}

		//! Constructs point with given parameters \p _x and \p _y.
		constexpr Point(T _x, T _y) : x(_x), y(_y)
		{
			// Do nothing
		}

		//! Constructs point with initializer list.
		template <typename U>
		Point(const std::initializer_list<U>& list);

		//! Copy constructor.
		constexpr Point(const Point& pt) : x(pt.x), y(pt.y)
		{
			// Do nothing
		}

		// MARK: Basic setters
		//! Set both x and y components to \p s.
		void Set(T s);

		//! Set x and y components with given parameters.
		void Set(T x, T y);

		//! Set x and y components with given initializer list.
		template <typename U>
		void Set(const std::initializer_list<U>& list);

		//! Set x and y with other point \p pt.
		void Set(const Point& pt);

		//! Set both x and y to zero.
		void SetZero();

		// MARK: Binary operations: new instance = this (+) v
		//! Computes this + (v, v).
		Point Add(T v) const;

		//! Computes this + (v.x, v.y).
		Point Add(const Point& v) const;

		//! Computes this - (v, v).
		Point Sub(T v) const;

		//! Computes this - (v.x, v.y).
		Point Sub(const Point& v) const;

		//! Computes this * (v, v).
		Point Mul(T v) const;

		//! Computes this * (v.x, v.y).
		Point Mul(const Point& v) const;

		//! Computes this / (v, v).
		Point Div(T v) const;

		//! Computes this / (v.x, v.y).
		Point Div(const Point& v) const;

		// MARK: Binary operations: new instance = v (+) this
		//! Computes (v, v) + this.
		Point RAdd(T v) const;

		//! Computes (v.x, v.y) + this.
		Point RAdd(const Point& v) const;

		//! Computes (v, v) - this.
		Point RSub(T v) const;

		//! Computes (v.x, v.y) - this.
		Point RSub(const Point& v) const;

		//! Computes (v, v) * this.
		Point RMul(T v) const;

		//! Computes (v.x, v.y) * this.
		Point RMul(const Point& v) const;

		//! Computes (v, v) / this.
		Point RDiv(T v) const;

		//! Computes (v.x, v.y) / this.
		Point RDiv(const Point& v) const;

		// MARK: Augmented operations: this (+)= v
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

		// MARK: Basic getters
		//! Returns const reference to the \p i -th element of the point.
		const T& At(size_t i) const;

		//! Returns reference to the \p i -th element of the point.
		T& At(size_t i);

		//! Returns the sum of all the components (i.e. x + y).
		T Sum() const;

		//! Returns the minimum value among x and y.
		T Min() const;

		//! Returns the maximum value among x and y.
		T Max() const;

		//! Returns the absolute minimum value among x and y.
		T AbsMin() const;

		//! Returns the absolute maximum value among x and y.
		T AbsMax() const;

		//! Returns the index of the dominant axis.
		size_t DominantAxis() const;

		//! Returns the index of the subdominant axis.
		size_t SubdominantAxis() const;

		//! Returns a point with different value type.
		template <typename U>
		Point<U, 2> CastTo() const;

		//! Returns true if \p other is the same as this point.
		bool IsEqual(const Point& other) const;

		// MARK: Operators
		//! Returns reference to the \p i -th element of the point.
		T& operator[](size_t i);

		//! Returns const reference to the \p i -th element of the point.
		const T& operator[](size_t i) const;

		//! Set x and y components with given initializer list.
		Point& operator=(const std::initializer_list<T>& list);

		//! Set x and y with other point \p pt.
		Point& operator=(const Point& v);

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
	Point<T, 2> Clamp(const Point<T, 2>& v, const Point<T, 2>& low, const Point<T, 2>& high);

	//! Returns element-wise ceiled point.
	template <typename T>
	Point<T, 2> Ceil(const Point<T, 2>& a);

	//! Returns element-wise floored point.
	template <typename T>
	Point<T, 2> Floor(const Point<T, 2>& a);

	//! Type alias for two dimensional point.
	template <typename T> using Point2 = Point<T, 2>;

	//! Float-type 2D point.
	using Point2F = Point2<float>;

	//! Double-type 2D point.
	using Point2D = Point2<double>;

	//! Integer-type 2D point.
	using Point2I = Point2<ssize_t>;

	//! Unsigned integer-type 2D point.
	using Point2UI = Point2<size_t>;
}

#include <Core/Point/Point2-Impl.h>

#endif