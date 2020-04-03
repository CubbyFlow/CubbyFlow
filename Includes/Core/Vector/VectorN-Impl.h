/*************************************************************************
> File Name: VectorN-Impl.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: General purpose dynamically-sized N-D vector class.
> Created Time: 2017/09/28
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_VECTORN_IMPL_H
#define CUBBYFLOW_VECTORN_IMPL_H

#include <Core/Math/MathUtils.hpp>
#include <Core/Utils/Parallel.hpp>

namespace CubbyFlow
{
	// MARK: VectorN
	template <typename T>
	VectorN<T>::VectorN()
	{
		// Do nothing
	}

	template <typename T>
	VectorN<T>::VectorN(size_t n, const T& val) : m_elements(n, val)
	{
		// Do nothing
	}

	template <typename T>
	template <typename U>
	VectorN<T>::VectorN(const std::initializer_list<U>& list)
	{
		Set(list);
	}

	template <typename T>
	template <typename E>
	VectorN<T>::VectorN(const VectorExpression<T, E>& other)
	{
		Set(other);
	}

	template <typename T>
	VectorN<T>::VectorN(const VectorN& other)
	{
		Set(other);
	}

	template <typename T>
	VectorN<T>::VectorN(VectorN&& other) noexcept
	{
		(*this) = std::move(other);
	}

	template <typename T>
	void VectorN<T>::Resize(size_t n, const T& val)
	{
		m_elements.resize(n, val);
	}

	template <typename T>
	void VectorN<T>::Clear()
	{
		m_elements.clear();
	}

	template <typename T>
	void VectorN<T>::Set(const T& s)
	{
		ParallelFill(begin(), end(), s);
	}

	template <typename T>
	template <typename U>
	void VectorN<T>::Set(const std::initializer_list<U>& list)
	{
		m_elements = list;
	}

	template <typename T>
	template <typename E>
	void VectorN<T>::Set(const VectorExpression<T, E>& other)
	{
		Resize(other.size());

		// Parallel evaluation of the expression
		const E& expression = other();
		ParallelForEachIndex([&](size_t i) { m_elements[i] = expression[i]; });
	}

	template <typename T>
	void VectorN<T>::Append(const T& val)
	{
		m_elements.push_back(val);
	}

	template <typename T>
	void VectorN<T>::Swap(VectorN& other)
	{
		std::swap(other.m_elements, m_elements);
	}

	template <typename T>
	void VectorN<T>::SetZero()
	{
		Set(T(0));
	}

	template <typename T>
	void VectorN<T>::Normalize()
	{
		IDiv(Length());
	}

	template <typename T>
	size_t VectorN<T>::size() const
	{
		return m_elements.size();
	}

	template <typename T>
	T* VectorN<T>::data()
	{
		return m_elements.data();
	}

	template <typename T>
	const T* VectorN<T>::data() const
	{
		return m_elements.data();
	}

	template <typename T>
	typename VectorN<T>::ContainerType::iterator VectorN<T>::begin()
	{
		return m_elements.begin();
	}

	template <typename T>
	typename VectorN<T>::ContainerType::const_iterator VectorN<T>::begin() const
	{
		return m_elements.cbegin();
	}

	template <typename T>
	typename VectorN<T>::ContainerType::iterator VectorN<T>::end()
	{
		return m_elements.end();
	}

	template <typename T>
	typename VectorN<T>::ContainerType::const_iterator VectorN<T>::end() const
	{
		return m_elements.cend();
	}

	template <typename T>
	ArrayAccessor1<T> VectorN<T>::Accessor()
	{
		return ArrayAccessor1<T>(size(), data());
	}

	template <typename T>
	ConstArrayAccessor1<T> VectorN<T>::ConstAccessor() const
	{
		return ConstArrayAccessor1<T>(size(), data());
	}

	template <typename T>
	T VectorN<T>::At(size_t i) const
	{
		return m_elements[i];
	}

	template <typename T>
	T& VectorN<T>::At(size_t i)
	{
		return m_elements[i];
	}

	template <typename T>
	T VectorN<T>::Sum() const
	{
		return ParallelReduce(ZERO_SIZE, size(), T(0),
			[&](size_t start, size_t end, T init)
		{
			T result = init;

			for (size_t i = start; i < end; ++i)
			{
				result += m_elements[i];
			}

			return result;
		}, std::plus<T>());
	}

	template <typename T>
	T VectorN<T>::Avg() const
	{
		return Sum() / static_cast<T>(size());
	}

	template <typename T>
	T VectorN<T>::Min() const
	{
		const T& (*_min)(const T&, const T&) = std::min<T>;

		return ParallelReduce(ZERO_SIZE, size(), std::numeric_limits<T>::max(),
			[&](size_t start, size_t end, T init)
		{
			T result = init;

			for (size_t i = start; i < end; ++i)
			{
				result = std::min(result, m_elements[i]);
			}

			return result;
		}, _min);
	}

	template <typename T>
	T VectorN<T>::Max() const
	{
		const T& (*_max)(const T&, const T&) = std::max<T>;

		return ParallelReduce(ZERO_SIZE, size(), std::numeric_limits<T>::min(),
			[&](size_t start, size_t end, T init)
		{
			T result = init;

			for (size_t i = start; i < end; ++i)
			{
				result = std::max(result, m_elements[i]);
			}
			
			return result;
		}, _max);
	}

	template <typename T>
	T VectorN<T>::AbsMin() const
	{
		return ParallelReduce(ZERO_SIZE, size(), std::numeric_limits<T>::max(),
			[&](size_t start, size_t end, T init)
		{
			T result = init;

			for (size_t i = start; i < end; ++i)
			{
				result = CubbyFlow::AbsMin(result, m_elements[i]);
			}

			return result;
		}, CubbyFlow::AbsMin<T>);
	}

	template <typename T>
	T VectorN<T>::AbsMax() const
	{
		return ParallelReduce(ZERO_SIZE, size(), T(0),
			[&](size_t start, size_t end, T init)
		{
			T result = init;

			for (size_t i = start; i < end; ++i)
			{
				result = CubbyFlow::AbsMax(result, m_elements[i]);
			}

			return result;
		}, CubbyFlow::AbsMax<T>);
	}

	template <typename T>
	size_t VectorN<T>::DominantAxis() const
	{
		auto iter = std::max_element(begin(), end(), [](const T& a, const T& b)
		{
			return std::fabs(a) < std::fabs(b);
		});

		return iter - begin();
	}

	template <typename T>
	size_t VectorN<T>::SubdominantAxis() const
	{
		auto iter = std::max_element(begin(), end(), [](const T& a, const T& b)
		{
			return std::fabs(a) > std::fabs(b);
		});

		return iter - begin();
	}

	template <typename T>
	VectorScalarDiv<T, VectorN<T>> VectorN<T>::Normalized() const
	{
		T len = Length();
		return VectorScalarDiv<T, VectorN>(*this, len);
	}

	template <typename T>
	T VectorN<T>::Length() const
	{
		return std::sqrt(LengthSquared());
	}

	template <typename T>
	T VectorN<T>::LengthSquared() const
	{
		return Dot(*this);
	}

	template <typename T>
	template <typename E>
	T VectorN<T>::DistanceTo(const E& other) const
	{
		return std::sqrt(DistanceSquaredTo(other));
	}

	template <typename T>
	template <typename E>
	T VectorN<T>::DistanceSquaredTo(const E& other) const
	{
		assert(size() == other.size());

		return ParallelReduce(ZERO_SIZE, size(), T(0),
			[&](size_t start, size_t end, T init)
		{
			T result = init;

			for (size_t i = start; i < end; ++i)
			{
				T diff = (m_elements[i] - other[i]);
				result += diff * diff;
			}

			return result;
		}, std::plus<T>());
	}

	template <typename T>
	template <typename U>
	VectorTypeCast<U, VectorN<T>, T> VectorN<T>::CastTo() const
	{
		return VectorTypeCast<U, VectorN<T>, T>(*this);
	}

	template <typename T>
	template <typename E>
	bool VectorN<T>::IsEqual(const E& other) const
	{
		if (size() != other.size())
		{
			return false;
		}

		for (size_t i = 0; i < size(); ++i)
		{
			if (At(i) != other[i])
			{
				return false;
			}
		}

		return true;
	}

	template <typename T>
	template <typename E>
	bool VectorN<T>::IsSimilar(const E& other, T epsilon) const
	{
		if (size() != other.size())
		{
			return false;
		}

		for (size_t i = 0; i < size(); ++i)
		{
			if (std::fabs(At(i) - other[i]) > epsilon)
			{
				return false;
			}
		}

		return true;
	}

	template <typename T>
	template <typename E>
	VectorAdd<T, VectorN<T>, E> VectorN<T>::Add(const E& v) const
	{
		return VectorAdd<T, VectorN, E>(*this, v);
	}

	template <typename T>
	VectorScalarAdd<T, VectorN<T>> VectorN<T>::Add(const T& s) const
	{
		return VectorScalarAdd<T, VectorN>(*this, s);
	}

	template <typename T>
	template <typename E>
	VectorSub<T, VectorN<T>, E> VectorN<T>::Sub(const E& v) const
	{
		return VectorSub<T, VectorN, E>(*this, v);
	}

	template <typename T>
	VectorScalarSub<T, VectorN<T>> VectorN<T>::Sub(const T& s) const
	{
		return VectorScalarSub<T, VectorN>(*this, s);
	}

	template <typename T>
	template <typename E>
	VectorMul<T, VectorN<T>, E> VectorN<T>::Mul(const E& v) const
	{
		return VectorMul<T, VectorN, E>(*this, v);
	}

	template <typename T>
	VectorScalarMul<T, VectorN<T>> VectorN<T>::Mul(const T& s) const
	{
		return VectorScalarMul<T, VectorN>(*this, s);
	}

	template <typename T>
	template <typename E>
	VectorDiv<T, VectorN<T>, E> VectorN<T>::Div(const E& v) const
	{
		return VectorDiv<T, VectorN, E>(*this, v);
	}

	template <typename T>
	VectorScalarDiv<T, VectorN<T>> VectorN<T>::Div(const T& s) const
	{
		return VectorScalarDiv<T, VectorN>(*this, s);
	}

	template <typename T>
	template <typename E>
	T VectorN<T>::Dot(const E& v) const
	{
		assert(size() == v.size());

		return ParallelReduce(ZERO_SIZE, size(), T(0),
			[&](size_t start, size_t end, T init)
		{
			T result = init;

			for (size_t i = start; i < end; ++i)
			{
				result += m_elements[i] * v[i];
			}

			return result;
		}, std::plus<T>());
	}

	template <typename T>
	VectorScalarRSub<T, VectorN<T>> VectorN<T>::RSub(const T& s) const
	{
		return VectorScalarRSub<T, VectorN>(*this, s);
	}

	template <typename T>
	template <typename E>
	VectorSub<T, VectorN<T>, E> VectorN<T>::RSub(const E& v) const
	{
		return VectorSub<T, VectorN, E>(v, *this);
	}

	template <typename T>
	VectorScalarRDiv<T, VectorN<T>> VectorN<T>::RDiv(const T& s) const
	{
		return VectorScalarRDiv<T, VectorN>(*this, s);
	}

	template <typename T>
	template <typename E>
	VectorDiv<T, VectorN<T>, E> VectorN<T>::RDiv(const E& v) const
	{
		return VectorDiv<T, VectorN, E>(v, *this);
	}

	template <typename T>
	void VectorN<T>::IAdd(const T& s)
	{
		Set(Add(s));
	}

	template <typename T>
	template <typename E>
	void VectorN<T>::IAdd(const E& v)
	{
		Set(Add(v));
	}

	template <typename T>
	void VectorN<T>::ISub(const T& s)
	{
		Set(Sub(s));
	}

	template <typename T>
	template <typename E>
	void VectorN<T>::ISub(const E& v)
	{
		Set(Sub(v));
	}

	template <typename T>
	void VectorN<T>::IMul(const T& s)
	{
		Set(Mul(s));
	}

	template <typename T>
	template <typename E>
	void VectorN<T>::IMul(const E& v)
	{
		Set(Mul(v));
	}

	template <typename T>
	void VectorN<T>::IDiv(const T& s)
	{
		Set(Div(s));
	}

	template <typename T>
	template <typename E>
	void VectorN<T>::IDiv(const E& v)
	{
		Set(Div(v));
	}

	template <typename T>
	template <typename Callback>
	void VectorN<T>::ForEach(Callback func) const
	{
		ConstAccessor().ForEach(func);
	}

	template <typename T>
	template <typename Callback>
	void VectorN<T>::ForEachIndex(Callback func) const
	{
		ConstAccessor().ForEachIndex(func);
	}

	template <typename T>
	template <typename Callback>
	void VectorN<T>::ParallelForEach(Callback func)
	{
		Accessor().ParallelForEach(func);
	}

	template <typename T>
	template <typename Callback>
	void VectorN<T>::ParallelForEachIndex(Callback func) const
	{
		ConstAccessor().ParallelForEachIndex(func);
	}

	template <typename T>
	T VectorN<T>::operator[](size_t i) const
	{
		return m_elements[i];
	}

	template <typename T>
	T& VectorN<T>::operator[](size_t i)
	{
		return m_elements[i];
	}

	template <typename T>
	template <typename U>
	VectorN<T>& VectorN<T>::operator=(const std::initializer_list<U>& list)
	{
		Set(list);
		return *this;
	}

	template <typename T>
	template <typename E>
	VectorN<T>& VectorN<T>::operator=(const VectorExpression<T, E>& other)
	{
		Set(other);
		return *this;
	}

	template <typename T>
	VectorN<T>& VectorN<T>::operator=(const VectorN& other)
	{
		Set(other);
		return *this;
	}

	template <typename T>
	VectorN<T>& VectorN<T>::operator=(VectorN&& other) noexcept
	{
		m_elements = std::move(other.m_elements);
		return *this;
	}

	template <typename T>
	VectorN<T>& VectorN<T>::operator+=(const T& s)
	{
		IAdd(s);
		return *this;
	}

	template <typename T>
	template <typename E>
	VectorN<T>& VectorN<T>::operator+=(const E& v)
	{
		IAdd(v);
		return *this;
	}

	template <typename T>
	VectorN<T>& VectorN<T>::operator-=(const T& s)
	{
		ISub(s);
		return *this;
	}

	template <typename T>
	template <typename E>
	VectorN<T>& VectorN<T>::operator-=(const E& v)
	{
		ISub(v);
		return *this;
	}

	template <typename T>
	VectorN<T>& VectorN<T>::operator*=(const T& s)
	{
		IMul(s);
		return *this;
	}

	template <typename T>
	template <typename E>
	VectorN<T>& VectorN<T>::operator*=(const E& v)
	{
		IMul(v);
		return *this;
	}

	template <typename T>
	VectorN<T>& VectorN<T>::operator/=(const T& s)
	{
		IDiv(s);
		return *this;
	}

	template <typename T>
	template <typename E>
	VectorN<T>& VectorN<T>::operator/=(const E& v)
	{
		IDiv(v);
		return *this;
	}

	template <typename T>
	template <typename E>
	bool VectorN<T>::operator==(const E& v) const
	{
		return IsEqual(v);
	}

	template <typename T>
	template <typename E>
	bool VectorN<T>::operator!=(const E& v) const
	{
		return !IsEqual(v);
	}
}

#endif