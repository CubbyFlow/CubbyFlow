/*************************************************************************
> File Name: Vector.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Generic N-D vector class.
> Created Time: 2017/02/19
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_VECTOR_H
#define CUBBYFLOW_VECTOR_H

#include <Core/Array/ArrayAccessor1.h>
#include <Core/Utils/TypeHelpers.h>
#include <Core/Vector/VectorExpression.h>

#include <array>
#include <cmath>
#include <type_traits>

namespace CubbyFlow
{
	//!
	//! \brief Generic statically-sized N-D vector class.
	//!
	//! This class defines N-D vector data where its size is determined statically
	//! at compile time.
	//!
	//! \tparam T - Number type.
	//! \tparam N - Dimension.
	//!

	template <typename T, size_t N>
	class Vector final : public VectorExpression<T, Vector<T, N>>
	{
	public:
		static_assert(N > 0, "Size of static-sized vector should be greater than zero.");
		static_assert(std::is_floating_point<T>::value, "Vector only can be instantiated with floating point types");

		using ContainerType = std::array<T, N>;

		//! Constructs a vector with zeros.
		Vector();

		//! Constructs vector instance with parameters.
		template <typename... Params>
		explicit Vector(Params... params);

		//! Constructs vector instance with initializer list.
		template <typename U>
		Vector(const std::initializer_list<U>& list);

		//! Constructs vector with expression template.
		template <typename E>
		Vector(const VectorExpression<T, E>& other);

		//! Copy constructor.
		Vector(const Vector& other);

		// MARK: Basic setters
		//! Sets all elements to \p s.
		void Set(const T& s);

		//! Set vector instance with initializer list.
		template <typename U>
		void Set(const std::initializer_list<U>& list);

		//! Sets vector with expression template.
		template <typename E>
		void Set(const VectorExpression<T, E>& other);

		//! Set vector instance with other vector.
		void Set(const Vector& other);

		//! Swaps the content of the vector with \p other vector.
		void Swap(Vector& other);

		//! Sets all elements to zero.
		void SetZero();

		//! Normalizes this vector.
		void Normalize();

		// MARK: Basic getters
		//! Returns the size of the vector.
		constexpr size_t size() const;

		//! Returns the raw pointer to the vector data.
		T* data();

		//! Returns the const raw pointer to the vector data.
		const T* data() const;

		//! Returns the begin iterator of the vector.
		typename ContainerType::iterator begin();

		//! Returns the begin const iterator of the vector.
		typename ContainerType::const_iterator begin() const;

		//! Returns the end iterator of the vector.
		typename ContainerType::iterator end();

		//! Returns the end const iterator of the vector.
		typename ContainerType::const_iterator end() const;

		//! Returns the array accessor.
		ArrayAccessor1<T> Accessor();

		//! Returns the const array accessor.
		ConstArrayAccessor1<T> ConstAccessor() const;

		//! Returns const reference to the \p i -th element of the vector.
		T At(size_t i) const;

		//! Returns reference to the \p i -th element of the vector.
		T& At(size_t i);

		//! Returns the sum of all the elements.
		T Sum() const;

		//! Returns the average of all the elements.
		T Avg() const;

		//! Returns the minimum element.
		T Min() const;

		//! Returns the maximum element.
		T Max() const;

		//! Returns the absolute minimum element.
		T AbsMin() const;

		//! Returns the absolute maximum element.
		T AbsMax() const;

		//! Returns the index of the dominant axis.
		size_t DominantAxis() const;

		//! Returns the index of the subdominant axis.
		size_t SubdominantAxis() const;

		//! Returns normalized vector.
		VectorScalarDiv<T, Vector> Normalized() const;

		//! Returns the length of the vector.
		T Length() const;

		//! Returns the squared length of the vector.
		T LengthSquared() const;

		//! Returns the distance to the other vector.
		template <typename E>
		T DistanceTo(const E& other) const;

		//! Returns the squared distance to the other vector.
		template <typename E>
		T DistanceSquaredTo(const E& other) const;

		//! Returns a vector with different value type.
		template <typename U>
		VectorTypeCast<U, Vector<T, N>, T> CastTo() const;

		//! Returns true if \p other is the same as this vector.
		template <typename E>
		bool IsEqual(const E& other) const;

		//! Returns true if \p other is similar to this vector.
		template <typename E>
		bool IsSimilar(const E& other, T epsilon = std::numeric_limits<T>::epsilon()) const;

		// MARK: Binary operations: new instance = this (+) v
		//! Computes this + v.
		template <typename E>
		VectorAdd<T, Vector, E> Add(const E& v) const;

		//! Computes this + (s, s, ... , s).
		VectorScalarAdd<T, Vector> Add(const T& s) const;

		//! Computes this - v.
		template <typename E>
		VectorSub<T, Vector, E> Sub(const E& v) const;

		//! Computes this - (s, s, ... , s).
		VectorScalarSub<T, Vector> Sub(const T& s) const;

		//! Computes this * v.
		template <typename E>
		VectorMul<T, Vector, E> Mul(const E& v) const;

		//! Computes this * (s, s, ... , s).
		VectorScalarMul<T, Vector> Mul(const T& s) const;

		//! Computes this / v.
		template <typename E>
		VectorDiv<T, Vector, E> Div(const E& v) const;

		//! Computes this / (s, s, ... , s).
		VectorScalarDiv<T, Vector> Div(const T& s) const;

		//! Computes dot product.
		template <typename E>
		T Dot(const E& v) const;

		// MARK: Binary operations: new instance = v (+) this
		//! Computes (s, s, ... , s) - this.
		VectorScalarRSub<T, Vector> RSub(const T& s) const;

		//! Computes v - this.
		template <typename E>
		VectorSub<T, Vector, E> RSub(const E& v) const;

		//! Computes (s, s, ... , s) / this.
		VectorScalarRDiv<T, Vector> RDiv(const T& s) const;

		//! Computes v / this.
		template <typename E>
		VectorDiv<T, Vector, E> RDiv(const E& v) const;

		// MARK: Augmented operations: this (+)= v
		//! Computes this += (s, s, ... , s).
		void IAdd(const T& s);

		//! Computes this += v.
		template <typename E>
		void IAdd(const E& v);

		//! Computes this -= (s, s, ... , s).
		void ISub(const T& s);

		//! Computes this -= v.
		template <typename E>
		void ISub(const E& v);

		//! Computes this *= (s, s, ... , s).
		void IMul(const T& s);

		//! Computes this *= v.
		template <typename E>
		void IMul(const E& v);

		//! Computes this /= (s, s, ... , s).
		void IDiv(const T& s);

		//! Computes this /= v.
		template <typename E>
		void IDiv(const E& v);

		// MARK: Operators
		//!
		//! \brief Iterates the vector and invoke given \p func for each element.
		//!
		//! This function iterates the vector elements and invoke the callback
		//! function \p func. The callback function takes array's element as its
		//! input. The order of execution will be 0 to N-1 where N is the size of
		//! the vector. Below is the sample usage:
		//!
		//! \code{.cpp}
		//! Vector<float, 2> vec(10, 4.f);
		//! vec.ForEach([](float elem)
		//! {
		//!     printf("%d\n", elem);
		//! });
		//! \endcode
		//!
		template <typename Callback>
		void ForEach(Callback func) const;

		//!
		//! \brief Iterates the vector and invoke given \p func for each index.
		//!
		//! This function iterates the vector elements and invoke the callback
		//! function \p func. The callback function takes one parameter which is the
		//! index of the vector. The order of execution will be 0 to N-1 where N is
		//! the size of the array. Below is the sample usage:
		//!
		//! \code{.cpp}
		//! Vector<float, 2> vec(10, 4.f);
		//! vec.ForEachIndex([&](size_t i)
		//! {
		//!     vec[i] = 4.f * i + 1.5f;
		//! });
		//! \endcode
		//!
		template <typename Callback>
		void ForEachIndex(Callback func) const;

		//! Returns the const reference to the \p i -th element.
		const T& operator[](size_t i) const;

		//! Returns the reference to the \p i -th element.
		T& operator[](size_t);

		//! Set vector instance with initializer list.
		template <typename U>
		Vector& operator=(const std::initializer_list<U>& list);

		//! Sets vector with expression template.
		template <typename E>
		Vector& operator=(const VectorExpression<T, E>& other);

		//! Computes this += (s, s, ... , s)
		Vector& operator+=(const T& s);

		//! Computes this += v
		template <typename E>
		Vector& operator+=(const E& v);

		//! Computes this -= (s, s, ... , s)
		Vector& operator-=(const T& s);

		//! Computes this -= v
		template <typename E>
		Vector& operator-=(const E& v);

		//! Computes this *= (s, s, ... , s)
		Vector& operator*=(const T& s);

		//! Computes this *= v
		template <typename E>
		Vector& operator*=(const E& v);

		//! Computes this /= (s, s, ... , s)
		Vector& operator/=(const T& s);

		//! Computes this /= v
		template <typename E>
		Vector& operator/=(const E& v);

		//! Returns true if \p other is the same as this vector.
		template <typename E>
		bool operator==(const E& v) const;

		//! Returns true if \p other is the not same as this vector.
		template <typename E>
		bool operator!=(const E& v) const;

	private:
		ContainerType m_elements;

		template <typename... Params>
		void SetAt(size_t i, T v, Params... params);
		void SetAt(size_t i, T v);
	};

	//! Returns the type of the value.
	template <typename T, size_t N>
	struct ScalarType<Vector<T, N>>
	{
		typedef T value;
	};
}

#include <Core/Vector/Vector-Impl.h>

#endif