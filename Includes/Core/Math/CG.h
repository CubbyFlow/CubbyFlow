/*************************************************************************
> File Name: CG.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Generic CG(conjugate gradient) operator functions.
> Created Time: 2017/08/13
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_CG_H
#define CUBBYFLOW_CG_H

#include <Core/Math/BLAS.h>

namespace CubbyFlow
{
	//!
	//! \brief No-op pre-conditioner for conjugate gradient.
	//!
	//! This pre-conditioner does nothing but simply copies the input vector to the
	//! output vector. Thus, it can be considered as an identity matrix.
	//!
	template <typename BLASType>
	struct NullCGPreconditioner final
	{
		void Build(const typename BLASType::MatrixType&)
		{
			// Do nothing
		}

		void Solve(const typename BLASType::VectorType& b, typename BLASType::VectorType* x)
		{
			BLASType::Set(b, x);
		}
	};

	//!
	//! \brief Solves conjugate gradient.
	//!
	template <typename BLASType>
	void CG(
		const typename BLASType::MatrixType& A,
		const typename BLASType::VectorType& b,
		unsigned int maxNumberOfIterations,
		double tolerance,
		typename BLASType::VectorType* x,
		typename BLASType::VectorType* r,
		typename BLASType::VectorType* d,
		typename BLASType::VectorType* q,
		typename BLASType::VectorType* s,
		unsigned int* lastNumberOfIterations,
		double* lastResidualNorm);

	//!
	//! \brief Solves pre-conditioned conjugate gradient.
	//!
	template <typename BLASType, typename PrecondType>
	void PCG(
		const typename BLASType::MatrixType& A,
		const typename BLASType::VectorType& b,
		unsigned int maxNumberOfIterations,
		double tolerance,
		PrecondType* M,
		typename BLASType::VectorType* x,
		typename BLASType::VectorType* r,
		typename BLASType::VectorType* d,
		typename BLASType::VectorType* q,
		typename BLASType::VectorType* s,
		unsigned int* lastNumberOfIterations,
		double* lastResidualNorm);
}

#include <Core/Math/CG-Impl.h>

#endif