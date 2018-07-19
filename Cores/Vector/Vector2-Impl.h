/*************************************************************************
> File Name: Vector2-Impl.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D vector class.
> Created Time: 2017/02/21
> Copyright (c) 2018, Dongmin Kim
*************************************************************************/
#ifndef CUBBYFLOW_VECTOR2_IMPL_H
#define CUBBYFLOW_VECTOR2_IMPL_H

#include <Math/MathUtils.h>

#include <cassert>
#include <limits>

namespace CubbyFlow
{
	// Constructors
	template <typename T>
	template <typename U>
	Vector<T, 2>::Vector(const std::initializer_list<U>& list)
	{
		Set(list);
	}

	template <typename T>
	void Vector<T, 2>::Set(T s)
	{
		x = s;
		y = s;
	}

	template <typename T>
	void Vector<T, 2>::Set(T newX, T newY)
	{
		x = newX;
		y = newY;
	}

	template <typename T>
	template <typename U>
	void Vector<T, 2>::Set(const std::initializer_list<U>& list)
	{
		assert(list.size() >= 2);

		auto inputElem = list.begin();
		x = static_cast<T>(*inputElem);
		y = static_cast<T>(*(++inputElem));
	}

	template <typename T>
	void Vector<T, 2>::Set(const Vector& v)
	{
		x = v.x;
		y = v.y;
	}

	template <typename T>
	void Vector<T, 2>::SetZero()
	{
		x = 0;
		y = 0;
	}

	template <typename T>
	void Vector<T, 2>::Normalize()
	{
		T length = Length();
		x /= length;
		y /= length;
	}

	template <typename T>
	Vector<T, 2> Vector<T, 2>::Add(T v) const
	{
		return Vector(x + v, y + v);
	}

	template <typename T>
	Vector<T, 2> Vector<T, 2>::Add(const Vector& v) const
	{
		return Vector(x + v.x, y + v.y);
	}


	template <typename T>
	Vector<T, 2> Vector<T, 2>::Sub(T v) const
	{
		return Vector(x - v, y - v);
	}

	template <typename T>
	Vector<T, 2> Vector<T, 2>::Sub(const Vector& v) const
	{
		return Vector(x - v.x, y - v.y);
	}

	template <typename T>
	Vector<T, 2> Vector<T, 2>::Mul(T v) const
	{
		return Vector(x * v, y * v);
	}

	template <typename T>
	Vector<T, 2> Vector<T, 2>::Mul(const Vector& v) const
	{
		return Vector(x * v.x, y * v.y);
	}

	template <typename T>
	Vector<T, 2> Vector<T, 2>::Div(T v) const
	{
		return Vector(x / v, y / v);
	}

	template <typename T>
	Vector<T, 2> Vector<T, 2>::Div(const Vector& v) const
	{
		return Vector(x / v.x, y / v.y);
	}

	template <typename T>
	T Vector<T, 2>::Dot(const Vector& v) const
	{
		return x * v.x + y * v.y;
	}

	template <typename T>
	T Vector<T, 2>::Cross(const Vector& v) const
	{
		return x * v.y - v.x * y;
	}

	template <typename T>
	Vector<T, 2> Vector<T, 2>::RSub(T v) const
	{
		return Vector(v - x, v - y);
	}

	template <typename T>
	Vector<T, 2> Vector<T, 2>::RSub(const Vector& v) const
	{
		return Vector(v.x - x, v.y - y);
	}

	template <typename T>
	Vector<T, 2> Vector<T, 2>::RDiv(T v) const
	{
		return Vector(v / x, v / y);
	}

	template <typename T>
	Vector<T, 2> Vector<T, 2>::RDiv(const Vector& v) const
	{
		return Vector(v.x / x, v.y / y);
	}

	template <typename T>
	T Vector<T, 2>::RCross(const Vector& v) const
	{
		return v.x * y - x * v.y;
	}

	template <typename T>
	void Vector<T, 2>::IAdd(T v)
	{
		x += v;
		y += v;
	}

	template <typename T>
	void Vector<T, 2>::IAdd(const Vector& v)
	{
		x += v.x;
		y += v.y;
	}

	template <typename T>
	void Vector<T, 2>::ISub(T v)
	{
		x -= v;
		y -= v;
	}

	template <typename T>
	void Vector<T, 2>::ISub(const Vector& v)
	{
		x -= v.x;
		y -= v.y;
	}

	template <typename T>
	void Vector<T, 2>::IMul(T v)
	{
		x *= v;
		y *= v;
	}

	template <typename T>
	void Vector<T, 2>::IMul(const Vector& v)
	{
		x *= v.x;
		y *= v.y;
	}

	template <typename T>
	void Vector<T, 2>::IDiv(T v)
	{
		x /= v;
		y /= v;
	}

	template <typename T>
	void Vector<T, 2>::IDiv(const Vector& v)
	{
		x /= v.x;
		y /= v.y;
	}

	template <typename T>
	const T& Vector<T, 2>::At(size_t i) const
	{
		assert(i < 2);
		return (&x)[i];
	}

	template <typename T>
	T& Vector<T, 2>::At(size_t i)
	{
		assert(i < 2);
		return (&x)[i];
	}

	template <typename T>
	T Vector<T, 2>::Sum() const
	{
		return x + y;
	}

	template <typename T>
	T Vector<T, 2>::Avg() const
	{
		return (x + y) / 2;
	}

	template <typename T>
	T Vector<T, 2>::Min() const
	{
		return std::min(x, y);
	}

	template <typename T>
	T Vector<T, 2>::Max() const
	{
		return std::max(x, y);
	}

	template <typename T>
	T Vector<T, 2>::AbsMin() const
	{
		return CubbyFlow::AbsMin(x, y);
	}
	
	template <typename T>
	T Vector<T, 2>::AbsMax() const
	{
		return CubbyFlow::AbsMax(x, y);
	}

	template <typename T>
	size_t Vector<T, 2>::DominantAxis() const
	{
		return (std::fabs(x) > std::fabs(y)) ? 0 : 1;
	}

	template <typename T>
	size_t Vector<T, 2>::SubdominantAxis() const
	{
		return (std::fabs(x) < std::fabs(y)) ? 0 : 1;
	}

	template <typename T>
	Vector<T, 2> Vector<T, 2>::Normalized() const
	{
		return Vector(x / Length(), y / Length());
	}

	template <typename T>
	T Vector<T, 2>::Length() const
	{
		return std::sqrt(x * x + y * y);
	}

	template <typename T>
	T Vector<T, 2>::LengthSquared() const
	{
		return x * x + y * y;
	}

	template <typename T>
	T Vector<T, 2>::DistanceTo(const Vector<T, 2>& other) const
	{
		return Sub(other).Length();
	}

	template <typename T>
	T Vector<T, 2>::DistanceSquaredTo(const Vector<T, 2>& other) const
	{
		return Sub(other).LengthSquared();
	}

	template <typename T>
	Vector<T, 2> Vector<T, 2>::Reflected(const Vector<T, 2>& normal) const
	{
		// this - 2(this.n)n
		return Sub(normal.Mul(2 * Dot(normal)));
	}

	template <typename T>
	Vector<T, 2> Vector<T, 2>::Projected(const Vector<T, 2>& normal) const
	{
		// this - (this.n)n
		return Sub(normal.Mul(Dot(normal)));
	}

	template <typename T>
	Vector<T, 2> Vector<T, 2>::Tangential() const
	{
		// Rotate 90 degrees
		return Vector<T, 2>(-y, x);
	}

	template <typename T>
	template <typename U>
	Vector<U, 2> Vector<T, 2>::CastTo() const
	{
		return Vector<U, 2>(static_cast<U>(x), static_cast<U>(y));
	}

	template <typename T>
	bool Vector<T, 2>::IsEqual(const Vector& other) const
	{
		return (x == other.x && y == other.y);
	}

	template <typename T>
	bool Vector<T, 2>::IsSimilar(const Vector& other, T epsilon) const
	{
		return (std::fabs(x - other.x) < epsilon) && (std::fabs(y - other.y) < epsilon);
	}

	template <typename T>
	T& Vector<T, 2>::operator[](size_t i)
	{
		assert(i < 2);
		return (&x)[i];
	}

	template <typename T>
	const T& Vector<T, 2>::operator[](size_t i) const 
	{
		return At(i);
	}

	template <typename T>
	template <typename U>
	Vector<T, 2>& Vector<T, 2>::operator=(const std::initializer_list<U>& list)
	{
		Set(list);
		return (*this);
	}
	
	template <typename T>
	Vector<T, 2>& Vector<T, 2>::operator=(const Vector& v)
	{
		Set(v);
		return (*this);
	}

	template <typename T>
	Vector<T, 2>& Vector<T, 2>::operator+=(T v)
	{
		IAdd(v);
		return (*this);
	}

	template <typename T>
	Vector<T, 2>& Vector<T, 2>::operator+=(const Vector& v)
	{
		IAdd(v);
		return (*this);
	}

	template <typename T>
	Vector<T, 2>& Vector<T, 2>::operator-=(T v)
	{
		ISub(v);
		return (*this);
	}

	template <typename T>
	Vector<T, 2>& Vector<T, 2>::operator-=(const Vector& v)
	{
		ISub(v);
		return (*this);
	}

	template <typename T>
	Vector<T, 2>& Vector<T, 2>::operator*=(T v)
	{
		IMul(v);
		return (*this);
	}

	template <typename T>
	Vector<T, 2>& Vector<T, 2>::operator*=(const Vector& v)
	{
		IMul(v);
		return (*this);
	}

	template <typename T>
	Vector<T, 2>& Vector<T, 2>::operator/=(T v)
	{
		IDiv(v);
		return (*this);
	}

	template <typename T>
	Vector<T, 2>& Vector<T, 2>::operator/=(const Vector& v)
	{
		IDiv(v);
		return (*this);
	}

	template <typename T>
	bool Vector<T, 2>::operator==(const Vector& v) const
	{
		return IsEqual(v);
	}

	template <typename T>
	bool Vector<T, 2>::operator!=(const Vector& v) const
	{
		return !IsEqual(v);
	}

	template <typename T>
	Vector<T, 2> operator+(const Vector<T, 2>& a)
	{
		return a;
	}

	template <typename T>
	Vector<T, 2> operator-(const Vector<T, 2>& a)
	{
		return Vector<T, 2>(-a.x, -a.y);
	}

	template <typename T>
	Vector<T, 2> operator+(const Vector<T, 2>& a, T b)
	{
		return a.Add(b);
	}

	template <typename T>
	Vector<T, 2> operator+(T a, const Vector<T, 2>& b)
	{
		return b.Add(a);
	}

	template <typename T>
	Vector<T, 2> operator+(const Vector<T, 2>& a, const Vector<T, 2>& b)
	{
		return a.Add(b);
	}

	template <typename T>
	Vector<T, 2> operator-(const Vector<T, 2>& a, T b)
	{
		return a.Sub(b);
	}


	template <typename T>
	Vector<T, 2> operator-(T a, const Vector<T, 2>& b)
	{
		return b.RSub(a);
	}

	template <typename T>
	Vector<T, 2> operator-(const Vector<T, 2>& a, const Vector<T, 2>& b)
	{
		return a.Sub(b);
	}

	template <typename T>
	Vector<T, 2> operator*(const Vector<T, 2>& a, T b)
	{
		return a.Mul(b);
	}

	template <typename T>
	Vector<T, 2> operator*(T a, const Vector<T, 2>& b)
	{
		return b.Mul(a);
	}

	template <typename T>
	Vector<T, 2> operator*(const Vector<T, 2>& a, const Vector<T, 2>& b)
	{
		return a.Mul(b);
	}

	template <typename T>
	Vector<T, 2> operator/(const Vector<T, 2>& a, T b)
	{
		return a.Div(b);
	}

	template <typename T>
	Vector<T, 2> operator/(T a, const Vector<T, 2>& b)
	{
		return b.RDiv(a);
	}

	template <typename T>
	Vector<T, 2> operator/(const Vector<T, 2>& a, const Vector<T, 2>& b)
	{
		return a.Div(b);
	}

	template <typename T>
	Vector<T, 2> Min(const Vector<T, 2>& a, const Vector<T, 2>& b)
	{
		return Vector<T, 2>(std::min(a.x, b.x), std::min(a.y, b.y));
	}

	template <typename T>
	Vector<T, 2> Max(const Vector<T, 2>& a, const Vector<T, 2>& b)
	{
		return Vector<T, 2>(std::max(a.x, b.x), std::max(a.y, b.y));
	}

	template <typename T>
	Vector<T, 2> Clamp(const Vector<T, 2>& v, const Vector<T, 2>& low, const Vector<T, 2>& high)
	{
		return Vector<T, 2>(Clamp(v.x, low.x, high.x), Clamp(v.y, low.y, high.y));
	}

	template <typename T>
	Vector<T, 2> Ceil(const Vector<T, 2>& a)
	{
		return Vector<T, 2>(std::ceil(a.x), std::ceil(a.y));
	}

	template <typename T>
	Vector<T, 2> Floor(const Vector<T, 2>& a)
	{
		return Vector<T, 2>(std::floor(a.x), std::floor(a.y));
	}

	template <typename T>
	Vector<T, 2> MonotonicCatmullRom(
		const Vector<T, 2>& v0,
		const Vector<T, 2>& v1,
		const Vector<T, 2>& v2,
		const Vector<T, 2>& v3,
		T f)
	{
		static const T two = static_cast<T>(2);
		static const T three = static_cast<T>(3);

		Vector<T, 2> d1 = (v2 - v0) / two;
		Vector<T, 2> d2 = (v3 - v1) / two;
		Vector<T, 2> D1 = v2 - v1;

		if (std::fabs(D1.x) < std::numeric_limits<T>::epsilon() ||
			Sign(D1.x) != Sign(d1.x) ||
			Sign(D1.x) != Sign(d2.x))
		{
			d1.x = d2.x = 0;
		}

		if (std::fabs(D1.y) < std::numeric_limits<T>::epsilon() ||
			Sign(D1.y) != Sign(d1.y) ||
			Sign(D1.y) != Sign(d2.y))
		{
			d1.y = d2.y = 0;
		}

		Vector<T, 2> a3 = d1 + d2 - two * D1;
		Vector<T, 2> a2 = three * D1 - two * d1 - d2;
		Vector<T, 2> a1 = d1;
		Vector<T, 2> a0 = v1;

		return a3 * Cubic(f) + a2 * Square(f) + a1 * f + a0;
	}
}

#endif