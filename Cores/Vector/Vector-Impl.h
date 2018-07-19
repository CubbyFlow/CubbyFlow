/*************************************************************************
> File Name: Vector-Impl.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Generic N-D vector class.
> Created Time: 2017/02/19
> Copyright (c) 2018, Dongmin Kim
*************************************************************************/
#ifndef CUBBYFLOW_VECTOR_IMPL_H
#define CUBBYFLOW_VECTOR_IMPL_H

#include <Math/MathUtils.h>

namespace CubbyFlow
{
	template <typename T, size_t N>
	Vector<T, N>::Vector()
	{
		for (auto& elem : m_elements)
		{
			elem = static_cast<T>(0);
		}
	}

	template <typename T, size_t N>
	template <typename... Params>
	Vector<T, N>::Vector(Params... params)
	{
		static_assert(sizeof...(params) == N, "Invalid number of parameters");

		SetAt(0, params...);
	}

	template <typename T, size_t N>
	template <typename U>
	Vector<T, N>::Vector(const std::initializer_list<U>& list)
	{
		Set(list);
	}

	template <typename T, size_t N>
	template <typename E>
	Vector<T, N>::Vector(const VectorExpression<T, E>& other)
	{
		Set(other);
	}

	template <typename T, size_t N>
	Vector<T, N>::Vector(const Vector& other) : m_elements(other.m_elements)
	{
		// Do nothing
	}

	template <typename T, size_t N>
	void Vector<T, N>::Set(const T& s)
	{
		m_elements.fill(s);
	}

	template <typename T, size_t N>
	template <typename U>
	void Vector<T, N>::Set(const std::initializer_list<U>& list)
	{
		assert(list.size() >= N);

		size_t i = 0;
		for (const auto& inputElem : list)
		{
			m_elements[i] = static_cast<T>(inputElem);
			++i;
		}
	}

	template <typename T, size_t N>
	template <typename E>
	void Vector<T, N>::Set(const VectorExpression<T, E>& other)
	{
		assert(size() == other.size());

		// Parallel evaluation of the expression
		const E& expression = other();
		ForEachIndex([&](size_t i) { m_elements[i] = expression[i]; });
	}

	template <typename T, size_t N>
	void Vector<T, N>::Set(const Vector& other)
	{
		m_elements = other.m_elements;
	}

	template <typename T, size_t N>
	void Vector<T, N>::Swap(Vector& other)
	{
		std::swap(other.m_elements, m_elements);
	}

	template <typename T, size_t N>
	void Vector<T, N>::SetZero()
	{
		Set(T(0));
	}

	template <typename T, size_t N>
	void Vector<T, N>::Normalize()
	{
		IDiv(Length());
	}

	template <typename T, size_t N>
	constexpr size_t Vector<T, N>::size() const
	{
		return N;
	}

	template <typename T, size_t N>
	T* Vector<T, N>::data()
	{
		return m_elements.data();
	}

	template <typename T, size_t N>
	const T* Vector<T, N>::data() const
	{
		return m_elements.data();
	}

	template <typename T, size_t N>
	typename Vector<T, N>::ContainerType::iterator Vector<T, N>::begin()
	{
		return m_elements.begin();
	}

	template <typename T, size_t N>
	typename Vector<T, N>::ContainerType::const_iterator Vector<T, N>::begin() const
	{
		return m_elements.cbegin();
	}

	template <typename T, size_t N>
	typename Vector<T, N>::ContainerType::iterator Vector<T, N>::end()
	{
		return m_elements.end();
	}

	template <typename T, size_t N>
	typename Vector<T, N>::ContainerType::const_iterator Vector<T, N>::end() const
	{
		return m_elements.cend();
	}

	template <typename T, size_t N>
	ArrayAccessor1<T> Vector<T, N>::Accessor()
	{
		return ArrayAccessor1<T>(size(), data());
	}

	template <typename T, size_t N>
	ConstArrayAccessor1<T> Vector<T, N>::ConstAccessor() const
	{
		return ConstArrayAccessor1<T>(size(), data());
	}

	template <typename T, size_t N>
	T Vector<T, N>::At(size_t i) const
	{
		return m_elements[i];
	}

	template <typename T, size_t N>
	T& Vector<T, N>::At(size_t i) 
	{
		return m_elements[i];
	}

	template <typename T, size_t N>
	T Vector<T, N>::Sum() const
	{
		T ret = 0;

		for (T val : m_elements)
		{
			ret += val;
		}

		return ret;
	}

	template <typename T, size_t N>
	T Vector<T, N>::Avg() const
	{
		return Sum() / static_cast<T>(size());
	}

	template <typename T, size_t N>
	T Vector<T, N>::Min() const
	{
		T ret = m_elements.front();

		for (T val : m_elements)
		{
			ret = std::min(ret, val);
		}

		return ret;
	}

	template <typename T, size_t N>
	T Vector<T, N>::Max() const
	{
		T ret = m_elements.front();

		for (T val : m_elements)
		{
			ret = std::max(ret, val);
		}

		return ret;
	}

	template <typename T, size_t N>
	T Vector<T, N>::AbsMin() const
	{
		T ret = m_elements.front();

		for (T val : m_elements)
		{
			ret = CubbyFlow::AbsMin(ret, val);
		}

		return ret;
	}

	template <typename T, size_t N>
	T Vector<T, N>::AbsMax() const
	{
		T ret = m_elements.front();

		for (T val : m_elements)
		{
			ret = CubbyFlow::AbsMax(ret, val);
		}

		return ret;
	}

	template <typename T, size_t N>
	size_t Vector<T, N>::DominantAxis() const
	{
		auto iter = std::max_element(begin(), end(), [](const T& a, const T& b)
		{
			return std::fabs(a) < std::fabs(b);
		});

		return iter - begin();
	}

	template <typename T, size_t N>
	size_t Vector<T, N>::SubdominantAxis() const
	{
		auto iter = std::max_element(begin(), end(), [](const T& a, const T& b)
		{
			return std::fabs(a) > std::fabs(b);
		});

		return iter - begin();
	}

	template <typename T, size_t N>
	VectorScalarDiv<T, Vector<T, N>> Vector<T, N>::Normalized() const
	{
		T len = Length();
		return VectorScalarDiv<T, Vector>(*this, len);
	}

	template <typename T, size_t N>
	T Vector<T, N>::Length() const
	{
		return std::sqrt(LengthSquared());
	}

	template <typename T, size_t N>
	T Vector<T, N>::LengthSquared() const
	{
		return Dot(*this);
	}

	template <typename T, size_t N>
	template <typename E>
	T Vector<T, N>::DistanceTo(const E& other) const
	{
		return std::sqrt(DistanceSquaredTo(other));
	}

	template <typename T, size_t N>
	template <typename E>
	T Vector<T, N>::DistanceSquaredTo(const E& other) const
	{
		assert(size() == other.size());

		T ret = 0;

		for (size_t i = 0; i < N; ++i)
		{
			T diff = (m_elements[i] - other[i]);
			ret += diff * diff;
		}

		return ret;
	}

	template <typename T, size_t N>
	template <typename U>
	VectorTypeCast<U, Vector<T, N>, T> Vector<T, N>::CastTo() const 
	{
		return VectorTypeCast<U, Vector<T, N>, T>(*this);
	}

	template <typename T, size_t N>
	template <typename E>
	bool Vector<T, N>::IsEqual(const E& other) const
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

	template <typename T, size_t N>
	template <typename E>
	bool Vector<T, N>::IsSimilar(const E& other, T epsilon) const
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

	template <typename T, size_t N>
	template <typename E>
	VectorAdd<T, Vector<T, N>, E> Vector<T, N>::Add(const E& v) const
	{
		return VectorAdd<T, Vector, E>(*this, v);
	}

	template <typename T, size_t N>
	VectorScalarAdd<T, Vector<T, N>> Vector<T, N>::Add(const T& s) const
	{
		return VectorScalarAdd<T, Vector>(*this, s);
	}

	template <typename T, size_t N>
	template <typename E>
	VectorSub<T, Vector<T, N>, E> Vector<T, N>::Sub(const E& v) const
	{
		return VectorSub<T, Vector, E>(*this, v);
	}

	template <typename T, size_t N>
	VectorScalarSub<T, Vector<T, N>> Vector<T, N>::Sub(const T& s) const
	{
		return VectorScalarSub<T, Vector>(*this, s);
	}

	template <typename T, size_t N>
	template <typename E>
	VectorMul<T, Vector<T, N>, E> Vector<T, N>::Mul(const E& v) const
	{
		return VectorMul<T, Vector, E>(*this, v);
	}

	template <typename T, size_t N>
	VectorScalarMul<T, Vector<T, N>> Vector<T, N>::Mul(const T& s) const 
	{
		return VectorScalarMul<T, Vector>(*this, s);
	}

	template <typename T, size_t N>
	template <typename E>
	VectorDiv<T, Vector<T, N>, E> Vector<T, N>::Div(const E& v) const
	{
		return VectorDiv<T, Vector, E>(*this, v);
	}

	template <typename T, size_t N>
	VectorScalarDiv<T, Vector<T, N>> Vector<T, N>::Div(const T& s) const
	{
		return VectorScalarDiv<T, Vector>(*this, s);
	}

	template <typename T, size_t N>
	template <typename E>
	T Vector<T, N>::Dot(const E& v) const
	{
		assert(size() == v.size());

		T ret = 0;

		for (size_t i = 0; i < N; ++i)
		{
			ret += m_elements[i] * v[i];
		}

		return ret;
	}

	template <typename T, size_t N>
	VectorScalarRSub<T, Vector<T, N>> Vector<T, N>::RSub(const T& s) const
	{
		return VectorScalarRSub<T, Vector>(*this, s);
	}

	template <typename T, size_t N>
	template <typename E>
	VectorSub<T, Vector<T, N>, E> Vector<T, N>::RSub(const E& v) const
	{
		return VectorSub<T, Vector, E>(v, *this);
	}

	template <typename T, size_t N>
	VectorScalarRDiv<T, Vector<T, N>> Vector<T, N>::RDiv(const T& s) const
	{
		return VectorScalarRDiv<T, Vector>(*this, s);
	}

	template <typename T, size_t N>
	template <typename E>
	VectorDiv<T, Vector<T, N>, E> Vector<T, N>::RDiv(const E& v) const
	{
		return VectorDiv<T, Vector, E>(v, *this);
	}

	template <typename T, size_t N>
	void Vector<T, N>::IAdd(const T& s)
	{
		Set(Add(s));
	}

	template <typename T, size_t N>
	template <typename E>
	void Vector<T, N>::IAdd(const E& v)
	{
		Set(Add(v));
	}

	template <typename T, size_t N>
	void Vector<T, N>::ISub(const T& s)
	{
		Set(Sub(s));
	}

	template <typename T, size_t N>
	template <typename E>
	void Vector<T, N>::ISub(const E& v)
	{
		Set(Sub(v));
	}

	template <typename T, size_t N>
	void Vector<T, N>::IMul(const T& s)
	{
		Set(Mul(s));
	}

	template <typename T, size_t N>
	template <typename E>
	void Vector<T, N>::IMul(const E& v)
	{
		Set(Mul(v));
	}

	template <typename T, size_t N>
	void Vector<T, N>::IDiv(const T& s)
	{
		Set(Div(s));
	}

	template <typename T, size_t N>
	template <typename E>
	void Vector<T, N>::IDiv(const E& v)
	{
		Set(Div(v));
	}

	template <typename T, size_t N>
	template <typename Callback>
	void Vector<T, N>::ForEach(Callback func) const
	{
		ConstAccessor().ForEach(func);
	}

	template <typename T, size_t N>
	template <typename Callback>
	void Vector<T, N>::ForEachIndex(Callback func) const
	{
		ConstAccessor().ForEachIndex(func);
	}

	template <typename T, size_t N>
	const T& Vector<T, N>::operator[](size_t i) const
	{
		return m_elements[i];
	}

	template <typename T, size_t N>
	T& Vector<T, N>::operator[](size_t i)
	{
		return m_elements[i];
	}

	template <typename T, size_t N>
	template <typename U>
	Vector<T, N>& Vector<T, N>::operator=(const std::initializer_list<U>& list)
	{
		Set(list);
		return *this;
	}

	template <typename T, size_t N>
	template <typename E>
	Vector<T, N>& Vector<T, N>::operator=(const VectorExpression<T, E>& other)
	{
		Set(other);
		return *this;
	}

	template <typename T, size_t N>
	Vector<T, N>& Vector<T, N>::operator+=(const T& s)
	{
		IAdd(s);
		return *this;
	}

	template <typename T, size_t N>
	template <typename E>
	Vector<T, N>& Vector<T, N>::operator+=(const E& v)
	{
		IAdd(v);
		return *this;
	}

	template <typename T, size_t N>
	Vector<T, N>& Vector<T, N>::operator-=(const T& s)
	{
		ISub(s);
		return *this;
	}

	template <typename T, size_t N>
	template <typename E>
	Vector<T, N>& Vector<T, N>::operator-=(const E& v)
	{
		ISub(v);
		return *this;
	}

	template <typename T, size_t N>
	Vector<T, N>& Vector<T, N>::operator*=(const T& s)
	{
		IMul(s);
		return *this;
	}

	template <typename T, size_t N>
	template <typename E>
	Vector<T, N>& Vector<T, N>::operator*=(const E& v)
	{
		IMul(v);
		return *this;
	}

	template <typename T, size_t N>
	Vector<T, N>& Vector<T, N>::operator/=(const T& s)
	{
		IDiv(s);
		return *this;
	}

	template <typename T, size_t N>
	template <typename E>
	Vector<T, N>& Vector<T, N>::operator/=(const E& v)
	{
		IDiv(v);
		return *this;
	}

	template <typename T, size_t N>
	template <typename E>
	bool Vector<T, N>::operator==(const E& v) const
	{
		return IsEqual(v);
	}

	template <typename T, size_t N>
	template <typename E>
	bool Vector<T, N>::operator!=(const E& v) const
	{
		return !IsEqual(v);
	}
	
	template <typename T, size_t N>
	template <typename... Params>
	void Vector<T, N>::SetAt(size_t i, T v, Params... params)
	{
		m_elements[i] = v;

		SetAt(i + 1, params...);
	}

	template <typename T, size_t N>
	void Vector<T, N>::SetAt(size_t i, T v)
	{
		m_elements[i] = v;
	}
}

#endif