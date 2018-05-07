/*************************************************************************
> File Name: BLAS-Impl.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Generic BLAS operator wrapper class.
> Created Time: 2017/08/13
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_BLAS_IMPL_H
#define CUBBYFLOW_BLAS_IMPL_H

#include <algorithm>
#include <cmath>

namespace CubbyFlow
{
	template <typename ScalarType, typename VectorType, typename MatrixType>
	void BLAS<ScalarType, VectorType, MatrixType>::Set(ScalarType s, VectorType* result)
	{
		result->Set(s);
	}

	template <typename ScalarType, typename VectorType, typename MatrixType>
	void BLAS<ScalarType, VectorType, MatrixType>::Set(const VectorType& v, VectorType* result)
	{
		result->Set(v);
	}

	template <typename ScalarType, typename VectorType, typename MatrixType>
	void BLAS<ScalarType, VectorType, MatrixType>::Set(ScalarType s, MatrixType* result)
	{
		result->Set(s);
	}

	template <typename ScalarType, typename VectorType, typename MatrixType>
	void BLAS<ScalarType, VectorType, MatrixType>::Set(const MatrixType& m, MatrixType* result)
	{
		result->Set(m);
	}

	template <typename ScalarType, typename VectorType, typename MatrixType>
	ScalarType BLAS<ScalarType, VectorType, MatrixType>::Dot(const VectorType& a, const VectorType& b)
	{
		return a.Dot(b);
	}

	template <typename ScalarType, typename VectorType, typename MatrixType>
	void BLAS<ScalarType, VectorType, MatrixType>::AXPlusY(ScalarType a, const VectorType& x, const VectorType& y, VectorType* result)
	{
		*result = a * x + y;
	}

	template <typename ScalarType, typename VectorType, typename MatrixType>
	void BLAS<ScalarType, VectorType, MatrixType>::MVM(const MatrixType& m, const VectorType& v, VectorType* result)
	{
		*result = m * v;
	}

	template <typename ScalarType, typename VectorType, typename MatrixType>
	void BLAS<ScalarType, VectorType, MatrixType>::Residual(const MatrixType& a, const VectorType& x, const VectorType& b, VectorType* result)
	{
		*result = b - a * x;
	}

	template <typename ScalarType, typename VectorType, typename MatrixType>
	ScalarType BLAS<ScalarType, VectorType, MatrixType>::L2Norm(const VectorType& v)
	{
		return std::sqrt(v.Dot(v));
	}

	template <typename ScalarType, typename VectorType, typename MatrixType>
	ScalarType BLAS<ScalarType, VectorType, MatrixType>::LInfNorm(const VectorType& v)
	{
		return std::fabs(v.AbsMax());
	}
}

#endif