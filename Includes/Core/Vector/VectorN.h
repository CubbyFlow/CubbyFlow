/*************************************************************************
> File Name: VectorN.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: General purpose dynamically-sized N-D vector class.
> Created Time: 2017/09/28
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_VECTORN_H
#define CUBBYFLOW_VECTORN_H

#include <Core/Array/ArrayAccessor1.h>
#include <Core/Vector/VectorExpression.h>

namespace CubbyFlow
{
	// MARK: VectorN
	//!
	//! \brief General purpose dynamically-sizedN-D vector class.
	//!
	//! This class defines N-D vector data where its size can be defined
	//! dynamically.
	//!
	//! \tparam T Type of the element.
	//!
	template <typename T>
	class VectorN final : public VectorExpression<T, VectorN<T>>
	{
	public:
		using ContainerType = std::vector<T>;

		// MARK: Constructors
		//! Constructs empty vector.
		VectorN();

		//! Constructs default vector (val, val, ... , val).
		VectorN(size_t n, const T& val = 0);

		//! Constructs vector with given initializer list.
		template <typename U>
		VectorN(const std::initializer_list<U>& list);

		//! Constructs vector with expression template.
		template <typename E>
		VectorN(const VectorExpression<T, E>& other);

		//! Copy constructor.
		VectorN(const VectorN& other);

		//! Move constructor.
		VectorN(VectorN&& other) noexcept;

		// MARK: Basic setters
		//! Resizes to \p n dimensional vector with initial value \p val.
		void Resize(size_t n, const T& val = 0);

		//! Clears the vector and make it zero-dimensional.
		void Clear();

		//! Sets all elements to \p s.
		void Set(const T& s);

		//! Sets all elements with given initializer list.
		template <typename U>
		void Set(const std::initializer_list<U>& list);

		//! Sets vector with expression template.
		template <typename E>
		void Set(const VectorExpression<T, E>& other);

		//! Adds an element.
		void Append(const T& val);

		//! Swaps the content of the vector with \p other vector.
		void Swap(VectorN& other);

		//! Sets all elements to zero.
		void SetZero();

		//! Normalizes this vector.
		void Normalize();

		// MARK: Basic getters
		//! Returns the size of the vector.
		size_t size() const;

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
		VectorScalarDiv<T, VectorN> Normalized() const;

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
		VectorTypeCast<U, VectorN<T>, T> CastTo() const;

		//! Returns true if \p other is the same as this vector.
		template <typename E>
		bool IsEqual(const E& other) const;

		//! Returns true if \p other is similar to this vector.
		template <typename E>
		bool IsSimilar(const E& other, T epsilon = std::numeric_limits<T>::epsilon()) const;

		// MARK: Binary operations: new instance = this (+) v
		//! Computes this + v.
		template <typename E>
		VectorAdd<T, VectorN, E> Add(const E& v) const;

		//! Computes this + (s, s, ... , s).
		VectorScalarAdd<T, VectorN> Add(const T& s) const;

		//! Computes this - v.
		template <typename E>
		VectorSub<T, VectorN, E> Sub(const E& v) const;

		//! Computes this - (s, s, ... , s).
		VectorScalarSub<T, VectorN> Sub(const T& s) const;

		//! Computes this * v.
		template <typename E>
		VectorMul<T, VectorN, E> Mul(const E& v) const;

		//! Computes this * (s, s, ... , s).
		VectorScalarMul<T, VectorN> Mul(const T& s) const;

		//! Computes this / v.
		template <typename E>
		VectorDiv<T, VectorN, E> Div(const E& v) const;

		//! Computes this / (s, s, ... , s).
		VectorScalarDiv<T, VectorN> Div(const T& s) const;

		//! Computes dot product.
		template <typename E>
		T Dot(const E& v) const;

		// MARK: Binary operations: new instance = v (+) this
		//! Computes (s, s, ... , s) - this.
		VectorScalarRSub<T, VectorN> RSub(const T& s) const;

		//! Computes v - this.
		template <typename E>
		VectorSub<T, VectorN, E> RSub(const E& v) const;

		//! Computes (s, s, ... , s) / this.
		VectorScalarRDiv<T, VectorN> RDiv(const T& s) const;

		//! Computes v / this.
		template <typename E>
		VectorDiv<T, VectorN, E> RDiv(const E& v) const;

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
		//! VectorN<float> vec(10, 4.f);
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
		//! VectorN<float> vec(10, 4.f);
		//! vec.ForEachIndex([&](size_t i)
		//! {
		//!     vec[i] = 4.f * i + 1.5f;
		//! });
		//! \endcode
		//!
		template <typename Callback>
		void ForEachIndex(Callback func) const;

		//!
		//! \brief Iterates the vector and invoke given \p func for each element in
		//!     parallel using multi-threading.
		//!
		//! This function iterates the vector elements and invoke the callback
		//! function \p func in parallel using multi-threading. The callback
		//! function takes vector's element as its input. The order of execution
		//! will be non-deterministic since it runs in parallel.
		//! Below is the sample usage:
		//!
		//! \code{.cpp}
		//! VectorN<float> vec(1000, 4.f);
		//! vec.ParallelForEach([](float& elem)
		//! {
		//!     elem *= 2;
		//! });
		//! \endcode
		//!
		//! The parameter type of the callback function doesn't have to be T&, but
		//! const T& or T can be used as well.
		//!
		template <typename Callback>
		void ParallelForEach(Callback func);

		//!
		//! \brief Iterates the vector and invoke given \p func for each index in
		//!     parallel using multi-threading.
		//!
		//! This function iterates the vector elements and invoke the callback
		//! function \p func in parallel using multi-threading. The callback
		//! function takes one parameter which is the index of the vector. The order
		//! of execution will be non-deterministic since it runs in parallel.
		//! Below is the sample usage:
		//!
		//! \code{.cpp}
		//! VectorN<float> vec(1000, 4.f);
		//! vec.ParallelForEachIndex([](size_t i)
		//! {
		//!     array[i] *= 2;
		//! });
		//! \endcode
		//!
		template <typename Callback>
		void ParallelForEachIndex(Callback func) const;

		//! Returns the \p i -th element.
		T operator[](size_t i) const;

		//! Returns the reference to the \p i -th element.
		T& operator[](size_t i);

		//! Sets vector with given initializer list.
		template <typename U>
		VectorN& operator=(const std::initializer_list<U>& list);

		//! Sets vector with expression template.
		template <typename E>
		VectorN& operator=(const VectorExpression<T, E>& other);

		//! Copy assignment.
		VectorN& operator=(const VectorN& other);

		//! Move assignment.
		VectorN& operator=(VectorN&& other) noexcept;

		//! Computes this += (s, s, ... , s)
		VectorN& operator+=(const T& s);

		//! Computes this += v
		template <typename E>
		VectorN& operator+=(const E& v);

		//! Computes this -= (s, s, ... , s)
		VectorN& operator-=(const T& s);

		//! Computes this -= v
		template <typename E>
		VectorN& operator-=(const E& v);

		//! Computes this *= (s, s, ... , s)
		VectorN& operator*=(const T& s);

		//! Computes this *= v
		template <typename E>
		VectorN& operator*=(const E& v);

		//! Computes this /= (s, s, ... , s)
		VectorN& operator/=(const T& s);

		//! Computes this /= v
		template <typename E>
		VectorN& operator/=(const E& v);

		//! Returns true if \p other is the same as this vector.
		template <typename E>
		bool operator==(const E& v) const;

		//! Returns true if \p other is the not same as this vector.
		template <typename E>
		bool operator!=(const E& v) const;

	private:
		ContainerType m_elements;
	};

	//! Vector type aliasing.
	using VectorNB = VectorN<int8_t>;
	using VectorNUB = VectorN<uint8_t>;
	using VectorNS = VectorN<int16_t>;
	using VectorNUS = VectorN<uint16_t>;
	using VectorNI = VectorN<int32_t>;
	using VectorNUI = VectorN<uint32_t>;
	using VectorNL = VectorN<int64_t>;
	using VectorNUL = VectorN<uint64_t>;
	using VectorNF = VectorN<float>;
	using VectorND = VectorN<double>;
	using VectorNZ = VectorN<ssize_t>;
	using VectorNUZ = VectorN<size_t>;
}

#include <Core/Vector/VectorN-Impl.h>

#endif