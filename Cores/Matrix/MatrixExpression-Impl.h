/*************************************************************************
> File Name: MatrixExpression-Impl.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Base class for matrix expression.
> Created Time: 2017/09/27
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_MATRIX_EXPRESSION_IMPL_H
#define CUBBYFLOW_MATRIX_EXPRESSION_IMPL_H

namespace CubbyFlow
{
	// MARK: MatrixExpression
	template <typename T, typename E>
	Size2 MatrixExpression<T, E>::size() const
	{
		return static_cast<const E&>(*this).size();
	}

	template <typename T, typename E>
	size_t MatrixExpression<T, E>::Rows() const
	{
		return static_cast<const E&>(*this).Rows();
	}

	template <typename T, typename E>
	size_t MatrixExpression<T, E>::Cols() const
	{
		return static_cast<const E&>(*this).Cols();
	}

	template <typename T, typename E>
	const E& MatrixExpression<T, E>::operator()() const
	{
		return static_cast<const E&>(*this);
	}

	template <typename T>
	MatrixConstant<T>::MatrixConstant(size_t m, size_t n, const T& c) : m_m(m), m_n(n), m_c(c)
	{
		// Do nothing
	}

	template <typename T>
	Size2 MatrixConstant<T>::size() const 
	{
		return Size2(Rows(), Cols());
	}

	template <typename T>
	size_t MatrixConstant<T>::Rows() const
	{
		return m_m;
	}

	template <typename T>
	size_t MatrixConstant<T>::Cols() const
	{
		return m_n;
	}

	template <typename T>
	T MatrixConstant<T>::operator()(size_t, size_t) const
	{
		return m_c;
	}

	template <typename T>
	MatrixIdentity<T>::MatrixIdentity(size_t m) : m_m(m)
	{
		// Do nothing
	}

	template <typename T>
	Size2 MatrixIdentity<T>::size() const
	{
		return Size2(m_m, m_m);
	}

	template <typename T>
	size_t MatrixIdentity<T>::Rows() const
	{
		return m_m;
	}

	template <typename T>
	size_t MatrixIdentity<T>::Cols() const
	{
		return m_m;
	}

	template <typename T>
	T MatrixIdentity<T>::operator()(size_t i, size_t j) const
	{
		return (i == j) ? 1 : 0;
	}

	// MARK: MatrixUnaryOp
	template <typename T, typename E, typename Op>
	MatrixUnaryOp<T, E, Op>::MatrixUnaryOp(const E& u) : m_u(u)
	{
		// Do nothing
	}

	template <typename T, typename E, typename Op>
	Size2 MatrixUnaryOp<T, E, Op>::size() const
	{
		return m_u.size();
	}

	template <typename T, typename E, typename Op>
	size_t MatrixUnaryOp<T, E, Op>::Rows() const
	{
		return m_u.Rows();
	}

	template <typename T, typename E, typename Op>
	size_t MatrixUnaryOp<T, E, Op>::Cols() const
	{
		return m_u.Cols();
	}

	template <typename T, typename E, typename Op>
	T MatrixUnaryOp<T, E, Op>::operator()(size_t i, size_t j) const
	{
		return m_op(m_u(i, j));
	}

	template <typename T, typename E>
	MatrixDiagonal<T, E>::MatrixDiagonal(const E& u, bool isDiag) : m_u(u), m_isDiag(isDiag)
	{
		// Do nothing
	}

	template <typename T, typename E>
	Size2 MatrixDiagonal<T, E>::size() const
	{
		return m_u.size();
	}

	template <typename T, typename E>
	size_t MatrixDiagonal<T, E>::Rows() const
	{
		return m_u.Rows();
	}

	template <typename T, typename E>
	size_t MatrixDiagonal<T, E>::Cols() const
	{
		return m_u.Cols();
	}

	template <typename T, typename E>
	T MatrixDiagonal<T, E>::operator()(size_t i, size_t j) const
	{
		if (m_isDiag)
		{
			return (i == j) ? m_u(i, j) : 0;
		}
		
		return (i != j) ? m_u(i, j) : 0;
	}

	template <typename T, typename E>
	MatrixTriangular<T, E>::MatrixTriangular(const E& u, bool isUpper, bool isStrict) :
		m_u(u), m_isUpper(isUpper), m_isStrict(isStrict)
	{
		// Do nothing
	}

	template <typename T, typename E>
	Size2 MatrixTriangular<T, E>::size() const
	{
		return m_u.size();
	}

	template <typename T, typename E>
	size_t MatrixTriangular<T, E>::Rows() const
	{
		return m_u.Rows();
	}

	template <typename T, typename E>
	size_t MatrixTriangular<T, E>::Cols() const
	{
		return m_u.Cols();
	}

	template <typename T, typename E>
	T MatrixTriangular<T, E>::operator()(size_t i, size_t j) const
	{
		if (i < j)
		{
			return (m_isUpper) ? m_u(i, j) : 0;
		}

		if (i > j)
		{
			return (!m_isUpper) ? m_u(i, j) : 0;
		}
		
		return (!m_isStrict) ? m_u(i, j) : 0;
	}

	// MARK: MatrixBinaryOp
	template <typename T, typename E1, typename E2, typename Op>
	MatrixBinaryOp<T, E1, E2, Op>::MatrixBinaryOp(const E1& u, const E2& v) : m_u(u), m_v(v)
	{
		assert(u.size() == v.size());
	}

	template <typename T, typename E1, typename E2, typename Op>
	Size2 MatrixBinaryOp<T, E1, E2, Op>::size() const
	{
		return m_v.size();
	}

	template <typename T, typename E1, typename E2, typename Op>
	size_t MatrixBinaryOp<T, E1, E2, Op>::Rows() const
	{
		return m_v.Rows();
	}

	template <typename T, typename E1, typename E2, typename Op>
	size_t MatrixBinaryOp<T, E1, E2, Op>::Cols() const
	{
		return m_v.Cols();
	}

	template <typename T, typename E1, typename E2, typename Op>
	T MatrixBinaryOp<T, E1, E2, Op>::operator()(size_t i, size_t j) const
	{
		return m_op(m_u(i, j), m_v(i, j));
	}

	// MARK: MatrixScalarBinaryOp
	template <typename T, typename E, typename Op>
	MatrixScalarBinaryOp<T, E, Op>::MatrixScalarBinaryOp(const E& u, const T& v) : m_u(u), m_v(v)
	{
		// Do nothing
	}

	template <typename T, typename E, typename Op>
	Size2 MatrixScalarBinaryOp<T, E, Op>::size() const
	{
		return m_u.size();
	}

	template <typename T, typename E, typename Op>
	size_t MatrixScalarBinaryOp<T, E, Op>::Rows() const
	{
		return m_u.Rows();
	}

	template <typename T, typename E, typename Op>
	size_t MatrixScalarBinaryOp<T, E, Op>::Cols() const
	{
		return m_u.Cols();
	}

	template <typename T, typename E, typename Op>
	T MatrixScalarBinaryOp<T, E, Op>::operator()(size_t i, size_t j) const
	{
		return m_op(m_u(i, j), m_v);
	}

	template <typename T, typename ME, typename VE>
	MatrixVectorMul<T, ME, VE>::MatrixVectorMul(const ME& m, const VE& v) : m_m(m), m_v(v)
	{
		assert(m_m.Cols() == m_v.size());
	}

	template <typename T, typename ME, typename VE>
	size_t MatrixVectorMul<T, ME, VE>::size() const
	{
		return m_v.size();
	}

	template <typename T, typename ME, typename VE>
	T MatrixVectorMul<T, ME, VE>::operator[](size_t i) const
	{
		T sum = 0;
		const size_t n = m_m.Cols();

		for (size_t j = 0; j < n; ++j)
		{
			sum += m_m(i, j) * m_v[j];
		}

		return sum;
	}

	// MARK: MatrixMul
	template <typename T, typename E1, typename E2>
	MatrixMul<T, E1, E2>::MatrixMul(const E1& u, const E2& v) : m_u(u), m_v(v)
	{
		assert(m_u.Cols() == m_v.Rows());
	}

	template <typename T, typename E1, typename E2>
	Size2 MatrixMul<T, E1, E2>::size() const
	{
		return Size2(m_u.Rows(), m_v.Cols());
	}

	template <typename T, typename E1, typename E2>
	size_t MatrixMul<T, E1, E2>::Rows() const
	{
		return m_u.Rows();
	}

	template <typename T, typename E1, typename E2>
	size_t MatrixMul<T, E1, E2>::Cols() const
	{
		return m_v.Cols();
	}

	template <typename T, typename E1, typename E2>
	T MatrixMul<T, E1, E2>::operator()(size_t i, size_t j) const
	{
		// Unoptimized mat-mat-mul
		T sum = 0;
		const size_t n = m_u.Cols();

		for (size_t k = 0; k < n; ++k)
		{
			sum += m_u(i, k) * m_v(k, j);
		}

		return sum;
	}

	// MARK: Operator overloadings
	template <typename T, typename E>
	MatrixScalarMul<T, E> operator-(const MatrixExpression<T, E>& a)
	{
		return MatrixScalarMul<T, E>(a(), T(-1));
	}

	template <typename T, typename E1, typename E2>
	MatrixAdd<T, E1, E2> operator+(const MatrixExpression<T, E1>& a, const MatrixExpression<T, E2>& b)
	{
		return MatrixAdd<T, E1, E2>(a(), b());
	}

	template <typename T, typename E>
	MatrixScalarAdd<T, E> operator+(const MatrixExpression<T, E>& a, T b)
	{
		return MatrixScalarAdd<T, E>(a(), b);
	}

	template <typename T, typename E>
	MatrixScalarAdd<T, E> operator+(T a, const MatrixExpression<T, E>& b)
	{
		return MatrixScalarAdd<T, E>(b(), a);
	}

	template <typename T, typename E1, typename E2>
	MatrixSub<T, E1, E2> operator-(const MatrixExpression<T, E1>& a, const MatrixExpression<T, E2>& b)
	{
		return MatrixSub<T, E1, E2>(a(), b());
	}

	template <typename T, typename E>
	MatrixScalarSub<T, E> operator-(const MatrixExpression<T, E>& a, T b)
	{
		return MatrixScalarSub<T, E>(a(), b);
	}

	template <typename T, typename E>
	MatrixScalarRSub<T, E> operator-(T a, const MatrixExpression<T, E>& b)
	{
		return MatrixScalarRSub<T, E>(b(), a);
	}

	template <typename T, typename E>
	MatrixScalarMul<T, E> operator*(const MatrixExpression<T, E>& a, T b)
	{
		return MatrixScalarMul<T, E>(a(), b);
	}

	template <typename T, typename E>
	MatrixScalarMul<T, E> operator*(T a, const MatrixExpression<T, E>& b)
	{
		return MatrixScalarMul<T, E>(b(), a);
	}

	template <typename T, typename ME, typename VE>
	MatrixVectorMul<T, ME, VE> operator*(const MatrixExpression<T, ME>& a, const VectorExpression<T, VE>& b)
	{
		return MatrixVectorMul<T, ME, VE>(a(), b());
	}

	template <typename T, typename E1, typename E2>
	MatrixMul<T, E1, E2> operator*(const MatrixExpression<T, E1>& a, const MatrixExpression<T, E2>& b)
	{
		return MatrixMul<T, E1, E2>(a(), b());
	}

	template <typename T, typename E>
	MatrixScalarDiv<T, E> operator/(const MatrixExpression<T, E>& a, T b)
	{
		return MatrixScalarDiv<T, E>(a(), b);
	}

	template <typename T, typename E>
	MatrixScalarRDiv<T, E> operator/(T a, const MatrixExpression<T, E>& b)
	{
		return MatrixScalarRDiv<T, E>(a(), b);
	}
}

#endif