/*************************************************************************
> File Name: Point2-Impl.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 2-D point class.
> Created Time: 2017/02/03
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_POINT2_IMPL_H
#define CUBBYFLOW_POINT2_IMPL_H

#include <Core/Math/MathUtils.h>

#include <cassert>
#include <cmath>

namespace CubbyFlow
{
	// Constructors
	template <typename T>
	template <typename U>
	Point<T, 2>::Point(const std::initializer_list<U>& list)
	{
		Set(list);
	}

	// Basic setters
	template <typename T>
	void Point<T, 2>::Set(T s)
	{
		x = s;
		y = s;
	}

	template <typename T>
	void Point<T, 2>::Set(T newX, T newY)
	{
		x = newX;
		y = newY;
	}

	template <typename T>
	template <typename U>
	void Point<T, 2>::Set(const std::initializer_list<U>& list)
	{
		assert(list.size() >= 2);

		auto inputElem = list.begin();
		x = static_cast<T>(*inputElem);
		y = static_cast<T>(*(++inputElem));
	}

	template <typename T>
	void Point<T, 2>::Set(const Point& v)
	{
		x = v.x;
		y = v.y;
	}

	template <typename T>
	void Point<T, 2>::SetZero()
	{
		x = y = 0;
	}

	// Binary operators: new instance = this (+) v
	template <typename T>
	Point<T, 2> Point<T, 2>::Add(T v) const
	{
		return Point(x + v, y + v);
	}

	template <typename T>
	Point<T, 2> Point<T, 2>::Add(const Point& v) const
	{
		return Point(x + v.x, y + v.y);
	}

	template <typename T>
	Point<T, 2> Point<T, 2>::Sub(T v) const
	{
		return Point(x - v, y - v);
	}

	template <typename T>
	Point<T, 2> Point<T, 2>::Sub(const Point& v) const
	{
		return Point(x - v.x, y - v.y);
	}

	template <typename T>
	Point<T, 2> Point<T, 2>::Mul(T v) const
	{
		return Point(x * v, y * v);
	}

	template <typename T>
	Point<T, 2> Point<T, 2>::Mul(const Point& v) const
	{
		return Point(x * v.x, y * v.y);
	}

	template <typename T>
	Point<T, 2> Point<T, 2>::Div(T v) const
	{
		return Point(x / v, y / v);
	}

	template <typename T>
	Point<T, 2> Point<T, 2>::Div(const Point& v) const
	{
		return Point(x / v.x, y / v.y);
	}

	// Binary operators: new instance = v (+) this
	template <typename T>
	Point<T, 2> Point<T, 2>::RAdd(T v) const
	{
		return Point(v + x, v + y);
	}

	template <typename T>
	Point<T, 2> Point<T, 2>::RAdd(const Point& v) const
	{
		return Point(v.x + x, v.y + y);
	}

	template <typename T>
	Point<T, 2> Point<T, 2>::RSub(T v) const
	{
		return Point(v - x, v - y);
	}

	template <typename T>
	Point<T, 2> Point<T, 2>::RSub(const Point& v) const
	{
		return Point(v.x - x, v.y - y);
	}

	template <typename T>
	Point<T, 2> Point<T, 2>::RMul(T v) const
	{
		return Point(v * x, v * y);
	}

	template <typename T>
	Point<T, 2> Point<T, 2>::RMul(const Point& v) const
	{
		return Point(v.x * x, v.y * y);
	}

	template <typename T>
	Point<T, 2> Point<T, 2>::RDiv(T v) const
	{
		return Point(v / x, v / y);
	}

	template <typename T>
	Point<T, 2> Point<T, 2>::RDiv(const Point& v) const
	{
		return Point(v.x / x, v.y / y);
	}

	// Augmented operators: this (+)= v
	template <typename T>
	void Point<T, 2>::IAdd(T v)
	{
		x += v;
		y += v;
	}

	template <typename T>
	void Point<T, 2>::IAdd(const Point& v)
	{
		x += v.x;
		y += v.y;
	}

	template <typename T>
	void Point<T, 2>::ISub(T v)
	{
		x -= v;
		y -= v;
	}

	template <typename T>
	void Point<T, 2>::ISub(const Point& v)
	{
		x -= v.x;
		y -= v.y;
	}

	template <typename T>
	void Point<T, 2>::IMul(T v)
	{
		x *= v;
		y *= v;
	}

	template <typename T>
	void Point<T, 2>::IMul(const Point& v)
	{
		x *= v.x;
		y *= v.y;
	}

	template <typename T>
	void Point<T, 2>::IDiv(T v)
	{
		x /= v;
		y /= v;
	}

	template <typename T>
	void Point<T, 2>::IDiv(const Point& v)
	{
		x /= v.x;
		y /= v.y;
	}

	// Basic getters
	template <typename T>
	const T& Point<T, 2>::At(size_t i) const
	{
		assert(i < 2);
		return (&x)[i];
	}

	template <typename T>
	T& Point<T, 2>::At(size_t i)
	{
		assert(i < 2);
		return (&x)[i];
	}

	template <typename T>
	T Point<T, 2>::Sum() const
	{
		return x + y;
	}

	template <typename T>
	T Point<T, 2>::Min() const
	{
		return std::min(x, y);
	}

	template <typename T>
	T Point<T, 2>::Max() const
	{
		return std::max(x, y);
	}

	template <typename T>
	T Point<T, 2>::AbsMin() const
	{
		return CubbyFlow::AbsMin(x, y);
	}

	template <typename T>
	T Point<T, 2>::AbsMax() const
	{
		return CubbyFlow::AbsMax(x, y);
	}

	template <typename T>
	size_t Point<T, 2>::DominantAxis() const
	{
		return (std::fabs(x) > std::fabs(y)) ? 0 : 1;
	}

	template <typename T>
	size_t Point<T, 2>::SubdominantAxis() const
	{
		return (std::fabs(x) < std::fabs(y)) ? 0 : 1;
	}

	template <typename T>
	template <typename U>
	Point2<U> Point<T, 2>::CastTo() const
	{
		return Point2<U>(static_cast<U>(x), static_cast<U>(y));
	}

	template <typename T>
	bool Point<T, 2>::IsEqual(const Point& other) const
	{
		return (x == other.x && y == other.y);
	}

	// Operators
	template <typename T>
	T& Point<T, 2>::operator[](size_t i)
	{
		assert(i < 2);
		return (&x)[i];
	}

	template <typename T>
	const T& Point<T, 2>::operator[](size_t i) const
	{
		assert(i < 2);
		return (&x)[i];
	}

	template <typename T>
	Point<T, 2>& Point<T, 2>::operator=(const std::initializer_list<T>& list)
	{
		Set(list);
		return (*this);
	}

	template <typename T>
	Point<T, 2>& Point<T, 2>::operator=(const Point& v)
	{
		Set(v);
		return (*this);
	}

	template <typename T>
	Point<T, 2>& Point<T, 2>::operator+=(T v)
	{
		IAdd(v);
		return (*this);
	}

	template <typename T>
	Point<T, 2>& Point<T, 2>::operator+=(const Point& v)
	{
		IAdd(v);
		return (*this);
	}

	template <typename T>
	Point<T, 2>& Point<T, 2>::operator-=(T v)
	{
		ISub(v);
		return (*this);
	}

	template <typename T>
	Point<T, 2>& Point<T, 2>::operator-=(const Point& v)
	{
		ISub(v);
		return (*this);
	}

	template <typename T>
	Point<T, 2>& Point<T, 2>::operator*=(T v)
	{
		IMul(v);
		return (*this);
	}

	template <typename T>
	Point<T, 2>& Point<T, 2>::operator*=(const Point& v)
	{
		IMul(v);
		return (*this);
	}

	template <typename T>
	Point<T, 2>& Point<T, 2>::operator/=(T v)
	{
		IDiv(v);
		return (*this);
	}

	template <typename T>
	Point<T, 2>& Point<T, 2>::operator/=(const Point& v)
	{
		IDiv(v);
		return (*this);
	}

	template <typename T>
	bool Point<T, 2>::operator==(const Point& v) const
	{
		return IsEqual(v);
	}

	template <typename T>
	bool Point<T, 2>::operator!=(const Point& v) const
	{
		return !IsEqual(v);
	}

	// Math functions
	template <typename T>
	Point<T, 2> operator+(const Point<T, 2>& a)
	{
		return a;
	}

	template <typename T>
	Point<T, 2> operator-(const Point<T, 2>& a)
	{
		return Point<T, 2>(-a.x, -a.y);
	}

	template <typename T>
	Point<T, 2> operator+(const Point<T, 2>& a, T b)
	{
		return a.Add(b);
	}

	template <typename T>
	Point<T, 2> operator+(T a, const Point<T, 2>& b)
	{
		return b.RAdd(a);
	}

	template <typename T>
	Point<T, 2> operator+(const Point<T, 2>& a, const Point<T, 2>& b)
	{
		return a.Add(b);
	}

	template <typename T>
	Point<T, 2> operator-(const Point<T, 2>& a, T b)
	{
		return a.Sub(b);
	}

	template <typename T>
	Point<T, 2> operator-(T a, const Point<T, 2>& b)
	{
		return b.RSub(a);
	}

	template <typename T>
	Point<T, 2> operator-(const Point<T, 2>& a, const Point<T, 2>& b)
	{
		return a.Sub(b);
	}

	template <typename T>
	Point<T, 2> operator*(const Point<T, 2>& a, T b)
	{
		return a.Mul(b);
	}

	template <typename T>
	Point<T, 2> operator*(T a, const Point<T, 2>& b)
	{
		return b.RMul(a);
	}

	template <typename T>
	Point<T, 2> operator*(const Point<T, 2>& a, const Point<T, 2>& b)
	{
		return a.Mul(b);
	}

	template <typename T>
	Point<T, 2> operator/(const Point<T, 2>& a, T b)
	{
		return a.Div(b);
	}

	template <typename T>
	Point<T, 2> operator/(T a, const Point<T, 2>& b)
	{
		return b.RDiv(a);
	}

	template <typename T>
	Point<T, 2> operator/(const Point<T, 2>& a, const Point<T, 2>& b)
	{
		return a.Div(b);
	}

	template <typename T>
	Point<T, 2> Min(const Point<T, 2>& a, const Point<T, 2>& b)
	{
		return Point<T, 2>(std::min(a.x, b.x), std::min(a.y, b.y));
	}

	template <typename T>
	Point<T, 2> Max(const Point<T, 2>& a, const Point<T, 2>& b)
	{
		return Point<T, 2>(std::max(a.x, b.x), std::max(a.y, b.y));
	}

	template <typename T>
	Point<T, 2> Clamp(const Point<T, 2>& v, const Point<T, 2>& low, const Point<T, 2>& high)
	{
		return Point<T, 2>(Clamp(v.x, low.x, high.x), Clamp(v.y, low.y, high.y));
	}

	template <typename T>
	Point<T, 2> Ceil(const Point<T, 2>& a)
	{
		return Point<T, 2>(std::ceil(a.x), std::ceil(a.y));
	}

	template <typename T>
	Point<T, 2> Floor(const Point<T, 2>& a)
	{
		return Point<T, 2>(std::floor(a.x), std::floor(a.y));
	}
}

#endif