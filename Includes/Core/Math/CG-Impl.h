/*************************************************************************
> File Name: CG-Impl.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Generic CG(conjugate gradient) operator functions.
> Created Time: 2017/08/13
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_CG_IMPL_H
#define CUBBYFLOW_CG_IMPL_H

#include <Core/Math/MathUtils.h>

namespace CubbyFlow
{
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
		double* lastResidualNorm)
	{
		using PrecondType = NullCGPreconditioner<BLASType>;
		PrecondType precond;
		
		PCG<BLASType, PrecondType>(
			A,
			b,
			maxNumberOfIterations,
			tolerance,
			&precond,
			x,
			r,
			d,
			q,
			s,
			lastNumberOfIterations,
			lastResidualNorm);
	}

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
		double* lastResidualNorm)
	{
		// Clear
		BLASType::Set(0, r);
		BLASType::Set(0, d);
		BLASType::Set(0, q);
		BLASType::Set(0, s);

		// r = b - Ax
		BLASType::Residual(A, *x, b, r);

		// d = M^-1r
		M->Solve(*r, d);

		// sigmaNew = r.d
		double sigmaNew = BLASType::Dot(*r, *d);

		unsigned int iter = 0;
		bool trigger = false;

		while (sigmaNew > Square(tolerance) && iter < maxNumberOfIterations)
		{
			// q = Ad
			BLASType::MVM(A, *d, q);

			// alpha = sigmaNew / d.q
			double alpha = sigmaNew / BLASType::Dot(*d, *q);

			// x = x + alpha * d
			BLASType::AXPlusY(alpha, *d, *x, x);

			// if i is divisible by 50...
			if (trigger || (iter % 50 == 0 && iter > 0))
			{
				// r = b - Ax
				BLASType::Residual(A, *x, b, r);
				trigger = false;
			}
			else
			{
				// r = r - alpha * q
				BLASType::AXPlusY(-alpha, *q, *r, r);
			}

			// s = M^-1r
			M->Solve(*r, s);

			// sigmaOld = sigmaNew
			double sigmaOld = sigmaNew;

			// sigmaNew = r.s
			sigmaNew = BLASType::Dot(*r, *s);

			if (sigmaNew > sigmaOld)
			{
				trigger = true;
			}

			// beta = sigmaNew / sigmaOld
			double beta = sigmaNew / sigmaOld;

			// d = s + beta*d
			BLASType::AXPlusY(beta, *d, *s, d);

			++iter;
		}

		*lastNumberOfIterations = iter;

		// std::fabs(sigmaNew) - Workaround for negative zero
		*lastResidualNorm = std::sqrt(std::fabs(sigmaNew));
	}
}

#endif