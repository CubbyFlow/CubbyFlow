/*************************************************************************
> File Name: Vector3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D vector class.
> Created Time: 2017/02/21
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_VECTOR3_H
#define CUBBYFLOW_VECTOR3_H

#include <Core/Vector/Vector2.h>

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
		//! X (or the first) component of the vector.
		T x;

		//! Y (or the second) component of the vector.
		T y;

		//! Z (or the third) component of the vector.
		T z;

		// MARK: Constructors
		//! Constructs default vector (0, 0, 0).
		constexpr Vector() : x(0), y(0), z(0)
		{
			// Do nothing	
		}

		//! Constructs vector with given parameters \p _x, \p _y, and \p _z.
		constexpr Vector(T _x, T _y, T _z) : x(_x), y(_y), z(_z)
		{
			// Do nothing
		}

		//! Constructs vector with a 2-D vector and a scalar.
		constexpr Vector(const Vector2<T>& v, T _z) : x(v.x), y(v.y), z(_z)
		{
			// Do nothing
		}

		//! Constructs vector with initializer list.
		template <typename U>
		Vector(const std::initializer_list<U>& list);

		//! Copy constructor.
		constexpr Vector(const Vector& v) : x(v.x), y(v.y), z(v.z)
		{
			// Do nothing
		}

		// MARK: Basic setters
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

		// MARK: Binary operations: new instance = this (+) v
		//! Computes this + (v, v, v).
		Vector Add(T v) const;

		//! Computes this + (v.x, v.y, v.z).
		Vector Add(const Vector& v) const;

		//! Computes this - (v, v, v).
		Vector Sub(T v) const;

		//! Computes this - (v.x, v.y, v.z).
		Vector Sub(const Vector& v) const;

		//! Computes this * (v, v, v).
		Vector Mul(T v) const;

		//! Computes this * (v.x, v.y, v.z).
		Vector Mul(const Vector& v) const;
		
		//! Computes this / (v, v, v).
		Vector Div(T v) const;

		//! Computes this / (v.x, v.y, v.z).
		Vector Div(const Vector& v) const;

		//! Computes dot product.
		T Dot(const Vector& v) const;

		//! Computes cross product.
		Vector Cross(const Vector& v) const;

		// MARK: Binary operations: new instance = v (+) this
		//! Computes (v, v, v) - this.
		Vector RSub(T v) const;

		//! Computes (v.x, v.y, v.z) - this.
		Vector RSub(const Vector& v) const;

		//! Computes (v, v, v) / this.
		Vector RDiv(T v) const;

		//! Computes (v.x, v.y, v.z) / this.
		Vector RDiv(const Vector& v) const;

		//! Computes \p v cross this.
		Vector RCross(const Vector& v) const;

		// MARK: Augmented operations: this (+)= v
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

		// MARK: Basic getters
		//! Returns const reference to the \p i -th element of the vector.
		const T& At(size_t i) const;

		//! Returns reference to the \p i -th element of the vector.
		T& At(size_t i);

		//! Returns the sum of all the components (i.e. x + y + z).
		T Sum() const;

		//! Returns the average of all the components.
		T Avg() const;

		//! Returns the minimum value among x, y, and z.
		T Min() const;

		//! Returns the maximum value among x, y, and z.
		T Max() const;

		//! Returns the absolute minimum value among x, y, and z.
		T AbsMin() const;

		//! Returns the absolute maximum value among x, y, and z.
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
		std::tuple<Vector<T, 3>, Vector<T, 3>> Tangential() const;

		//! Returns a vector with different value type.
		template <typename U>
		Vector<U, 3> CastTo() const;

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

	//! Type alias for three dimensional vector.
	template <typename T> using Vector3 = Vector<T, 3>;

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
	Vector<T, 3> Clamp(const Vector<T, 3>& v, const Vector<T, 3>& low, const Vector<T, 3>& high);

	//! Returns element-wise ceiled vector.
	template <typename T>
	Vector<T, 3> Ceil(const Vector<T, 3>& a);

	//! Returns element-wise floored vector.
	template <typename T>
	Vector<T, 3> Floor(const Vector<T, 3>& a);

	//! Vector type aliasing.
	using Vector3B = Vector3<int8_t>;
	using Vector3UB = Vector3<uint8_t>;
	using Vector3S = Vector3<int16_t>;
	using Vector3US = Vector3<uint16_t>;
	using Vector3I = Vector3<int32_t>;
	using Vector3UI = Vector3<uint32_t>;
	using Vector3L = Vector3<int64_t>;
	using Vector3UL = Vector3<uint64_t>;
	using Vector3F = Vector3<float>;
	using Vector3D = Vector3<double>;
	using Vector3Z = Vector3<ssize_t>;
	using Vector3UZ = Vector3<size_t>;

	// MARK: Extensions
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
	Vector3<T> MonotonicCatmullRom(
		const Vector3<T>& v0,
		const Vector3<T>& v1,
		const Vector3<T>& v2,
		const Vector3<T>& v3,
		T f);
}

#include <Core/Vector/Vector3-Impl.h>

#endif