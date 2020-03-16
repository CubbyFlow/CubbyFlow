/*************************************************************************
> File Name: Vector2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D vector class.
> Created Time: 2017/02/19
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_VECTOR2_H
#define CUBBYFLOW_VECTOR2_H

#include <Core/Utils/Constants.h>
#include <Core/Utils/TypeHelpers.h>
#include <Core/Vector/Vector.h>

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
		//! X (or the first) component of the vector.
		T x;

		//! Y (or the second) component of the vector.
		T y;

		// MARK: Constructors
		//! Constructs default vector (0, 0).
		constexpr Vector() : x(0), y(0)
		{
			// Do nothing
		}

		//! Constructs vector with given parameters \p _x and \p _y.
		constexpr Vector(T _x, T _y) : x(_x), y(_y)
		{
			// Do nothing
		}

		//! Constructs vector with initializer list.
		template <typename U>
		Vector(const std::initializer_list<U>& list);

		//! Copy constructor.
		constexpr Vector(const Vector& v) : x(v.x), y(v.y)
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

		//! Set x and y with other vector \p pt.
		void Set(const Vector& pt);

		//! Set both x and y to zero.
		void SetZero();

		//! Normalizes this vector.
		void Normalize();

		// MARK: Binary operations: new instance = this (+) v
		//! Computes this + (v, v).
		Vector Add(T v) const;

		//! Computes this + (v.x, v.y).
		Vector Add(const Vector& v) const;

		//! Computes this - (v, v).
		Vector Sub(T v) const;

		//! Computes this - (v.x, v.y).
		Vector Sub(const Vector& v) const;

		//! Computes this * (v, v).
		Vector Mul(T v) const;

		//! Computes this * (v.x, v.y).
		Vector Mul(const Vector& v) const;

		//! Computes this / (v, v).
		Vector Div(T v) const;

		//! Computes this / (v.x, v.y).
		Vector Div(const Vector& v) const;

		//! Computes dot product.
		T Dot(const Vector& v) const;
		
		//! Computes cross product.
		T Cross(const Vector& v) const;

		// MARK: Binary operations: new instance = v (+) this
		//! Computes (v, v) - this.
		Vector RSub(T v) const;

		//! Computes (v.x, v.y) - this.
		Vector RSub(const Vector& v) const;

		//! Computes (v, v) / this.
		Vector RDiv(T v) const;

		//! Computes (v.x, v.y) / this.
		Vector RDiv(const Vector& v) const;

		//! Computes \p v cross this.
		T RCross(const Vector& v) const;

		// MARK: Augmented operations: this (+)= v
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

		// MARK: Basic getters
		//! Returns const reference to the \p i -th element of the vector.
		const T& At(size_t i) const;

		//! Returns reference to the \p i -th element of the vector.
		T& At(size_t i);

		//! Returns the sum of all the components (i.e. x + y).
		T Sum() const;

		//! Returns the average of all the components.
		T Avg() const;

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

		//! Returns the index of the sub-dominant axis.
		size_t SubdominantAxis() const;

		//! Returns normalized vector.
		Vector Normalized() const;

		//! Returns the length of the vector.
		T Length() const;

		//! Returns the squared length of the vector.
		T LengthSquared() const;

		//! Returns the distance to the other vector.
		T DistanceTo(const Vector& other) const;

		//! Returns the squared distance to the other vector.
		T DistanceSquaredTo(const Vector& other) const;

		//! Returns the reflection vector to the surface with given surface normal.
		Vector Reflected(const Vector& normal) const;

		//! Returns the projected vector to the surface with given surface normal.
		Vector Projected(const Vector& normal) const;

		//! Returns the tangential vector for this vector.
		Vector Tangential() const;
		 
		//! Returns a vector with different value type.
		template <typename U>
		Vector<U, 2> CastTo() const;

		//! Returns true if \p other is the same as this vector.
		bool IsEqual(const Vector& other) const;

		//! Returns true if \p other is similar to this vector.
		bool IsSimilar(const Vector& other, T epsilon = std::numeric_limits<T>::epsilon()) const;

		// MARK: Operators
		//! Returns reference to the \p i -th element of the vector.
		T& operator[](size_t i);

		//! Returns const reference to the \p i -th element of the vector.
		const T& operator[](size_t i) const;

		//! Set x and y components with given initializer list.
		template <typename U>
		Vector& operator=(const std::initializer_list<U>& list);

		//! Set x and y with other vector \p pt.
		Vector& operator=(const Vector& v);

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
	};

	//! Type alias for two dimensional vector.
	template <typename T> using Vector2 = Vector<T, 2>;

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
	Vector<T, 2> Clamp(const Vector<T, 2>& v, const Vector<T, 2>& low, const Vector<T, 2>& high);

	//! Returns element-wise ceiled vector.
	template <typename T>
	Vector<T, 2> Ceil(const Vector<T, 2>& a);

	//! Returns element-wise floored vector.
	template <typename T>
	Vector<T, 2> Floor(const Vector<T, 2>& a);

	//! Vector type aliasing.
	using Vector2B = Vector2<int8_t>;
	using Vector2UB = Vector2<uint8_t>;
	using Vector2S = Vector2<int16_t>;
	using Vector2US = Vector2<uint16_t>;
	using Vector2I = Vector2<int32_t>;
	using Vector2UI = Vector2<uint32_t>;
	using Vector2L = Vector2<int64_t>;
	using Vector2UL = Vector2<uint64_t>;
	using Vector2F = Vector2<float>;
	using Vector2D = Vector2<double>;
	using Vector2Z = Vector2<ssize_t>;
	using Vector2UZ = Vector2<size_t>;

	// MARK: Extensions
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
	Vector<T, 2> MonotonicCatmullRom(
		const Vector<T, 2>& v0,
		const Vector<T, 2>& v1,
		const Vector<T, 2>& v2,
		const Vector<T, 2>& v3,
		T f);
}

#include <Core/Vector/Vector2-Impl.h>

#endif