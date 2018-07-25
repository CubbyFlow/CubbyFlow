/*************************************************************************
> File Name: FDMGaussSeidelSolver3.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D finite difference-type linear system solver using Gauss-Seidel method.
> Created Time: 2017/08/17
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Solver/FDM/FDMGaussSeidelSolver3.h>

namespace CubbyFlow
{
	FDMGaussSeidelSolver3::FDMGaussSeidelSolver3(
		unsigned int maxNumberOfIterations,
		unsigned int residualCheckInterval,
		double tolerance,
		double sorFactor,
		bool useRedBlackOrdering) :
		m_maxNumberOfIterations(maxNumberOfIterations),
		m_lastNumberOfIterations(0),
		m_residualCheckInterval(residualCheckInterval),
		m_tolerance(tolerance),
		m_lastResidual(std::numeric_limits<double>::max()),
		m_sorFactor(sorFactor),
		m_useRedBlackOrdering(useRedBlackOrdering)
	{
		// Do nothing
	}

	bool FDMGaussSeidelSolver3::Solve(FDMLinearSystem3* system)
	{
		ClearCompressedVectors();

		m_residual.Resize(system->x.size());

		m_lastNumberOfIterations = m_maxNumberOfIterations;

		for (unsigned int iter = 0; iter < m_maxNumberOfIterations; ++iter)
		{
			if (m_useRedBlackOrdering)
			{
				RelaxRedBlack(system->A, system->b, m_sorFactor, &system->x);
			}
			else
			{
				Relax(system->A, system->b, m_sorFactor, &system->x);
			}

			if (iter != 0 && iter % m_residualCheckInterval == 0)
			{
				FDMBLAS3::Residual(system->A, system->x, system->b, &m_residual);

				if (FDMBLAS3::L2Norm(m_residual) < m_tolerance)
				{
					m_lastNumberOfIterations = iter + 1;
					break;
				}
			}
		}

		FDMBLAS3::Residual(system->A, system->x, system->b, &m_residual);
		m_lastResidual = FDMBLAS3::L2Norm(m_residual);

		return m_lastResidual < m_tolerance;
	}

	bool FDMGaussSeidelSolver3::SolveCompressed(FDMCompressedLinearSystem3* system)
	{
		ClearUncompressedVectors();

		m_residualComp.Resize(system->x.size());

		m_lastNumberOfIterations = m_maxNumberOfIterations;

		for (unsigned int iter = 0; iter < m_maxNumberOfIterations; ++iter)
		{
			Relax(system->A, system->b, m_sorFactor, &system->x);

			if (iter != 0 && iter % m_residualCheckInterval == 0)
			{
				FDMCompressedBLAS3::Residual(system->A, system->x, system->b, &m_residualComp);

				if (FDMCompressedBLAS3::L2Norm(m_residualComp) < m_tolerance)
				{
					m_lastNumberOfIterations = iter + 1;
					break;
				}
			}
		}

		FDMCompressedBLAS3::Residual(system->A, system->x, system->b, &m_residualComp);
		m_lastResidual = FDMCompressedBLAS3::L2Norm(m_residualComp);

		return m_lastResidual < m_tolerance;
	}

	unsigned int FDMGaussSeidelSolver3::GetMaxNumberOfIterations() const
	{
		return m_maxNumberOfIterations;
	}

	unsigned int FDMGaussSeidelSolver3::GetLastNumberOfIterations() const
	{
		return m_lastNumberOfIterations;
	}

	double FDMGaussSeidelSolver3::GetTolerance() const
	{
		return m_tolerance;
	}

	double FDMGaussSeidelSolver3::GetLastResidual() const
	{
		return m_lastResidual;
	}

	double FDMGaussSeidelSolver3::GetSORFactor() const
	{
		return m_sorFactor;
	}

	bool FDMGaussSeidelSolver3::GetUseRedBlackOrdering() const
	{
		return m_useRedBlackOrdering;
	}

	void FDMGaussSeidelSolver3::Relax(const FDMMatrix3& A, const FDMVector3& b,
		double sorFactor, FDMVector3* x)
	{
		Size3 size = A.size();
		FDMVector3& refX = *x;

		A.ForEachIndex([&](size_t i, size_t j, size_t k)
		{
			double r =
				((i > 0) ? A(i - 1, j, k).right * refX(i - 1, j, k) : 0.0) +
				((i + 1 < size.x) ? A(i, j, k).right * refX(i + 1, j, k) : 0.0) +
				((j > 0) ? A(i, j - 1, k).up * refX(i, j - 1, k) : 0.0) +
				((j + 1 < size.y) ? A(i, j, k).up * refX(i, j + 1, k) : 0.0) +
				((k > 0) ? A(i, j, k - 1).front * refX(i, j, k - 1) : 0.0) +
				((k + 1 < size.z) ? A(i, j, k).front * refX(i, j, k + 1) : 0.0);

			refX(i, j, k) = (1.0 - sorFactor) * refX(i, j, k) +
				sorFactor * (b(i, j, k) - r) / A(i, j, k).center;
		});
	}

	void FDMGaussSeidelSolver3::Relax(const MatrixCSRD& A, const VectorND& b, double sorFactor, VectorND* x_)
	{
		const auto rp = A.RowPointersBegin();
		const auto ci = A.ColumnIndicesBegin();
		const auto nnz = A.NonZeroBegin();

		VectorND& x = *x_;

		b.ForEachIndex([&](size_t i)
		{
			const size_t rowBegin = rp[i];
			const size_t rowEnd = rp[i + 1];

			double r = 0.0;
			double diag = 1.0;
			for (size_t jj = rowBegin; jj < rowEnd; ++jj)
			{
				size_t j = ci[jj];

				if (i == j)
				{
					diag = nnz[jj];
				}
				else
				{
					r += nnz[jj] * x[j];
				}
			}

			x[i] = (1.0 - sorFactor) * x[i] + sorFactor * (b[i] - r) / diag;
		});
	}

	void FDMGaussSeidelSolver3::RelaxRedBlack(const FDMMatrix3& A, const FDMVector3& b,
		double sorFactor, FDMVector3* x)
	{
		Size3 size = A.size();
		FDMVector3& refX = *x;

		// Red update
		ParallelRangeFor(ZERO_SIZE, size.x, ZERO_SIZE, size.y, ZERO_SIZE, size.z,
			[&](size_t iBegin, size_t iEnd, size_t jBegin, size_t jEnd, size_t kBegin, size_t kEnd)
		{
			for (size_t k = kBegin; k < kEnd; ++k)
			{
				for (size_t j = jBegin; j < jEnd; ++j)
				{
					// i.e. (0, 0, 0)
					size_t i = (j + k) % 2 + iBegin;
					
					for (; i < iEnd; i += 2)
					{
						double r =
							((i > 0) ? A(i - 1, j, k).right * refX(i - 1, j, k) : 0.0) +
							((i + 1 < size.x) ? A(i, j, k).right * refX(i + 1, j, k) : 0.0) +
							((j > 0) ? A(i, j - 1, k).up * refX(i, j - 1, k) : 0.0) +
							((j + 1 < size.y) ? A(i, j, k).up * refX(i, j + 1, k) : 0.0) +
							((k > 0) ? A(i, j, k - 1).front * refX(i, j, k - 1) : 0.0) +
							((k + 1 < size.z) ? A(i, j, k).front * refX(i, j, k + 1) : 0.0);

						refX(i, j, k) = (1.0 - sorFactor) * refX(i, j, k) +
							sorFactor * (b(i, j, k) - r) / A(i, j, k).center;
					}
				}
			}
		});

		// Black update
		ParallelRangeFor(ZERO_SIZE, size.x, ZERO_SIZE, size.y, ZERO_SIZE, size.z,
			[&](size_t iBegin, size_t iEnd, size_t jBegin, size_t jEnd, size_t kBegin, size_t kEnd)
		{
			for (size_t k = kBegin; k < kEnd; ++k)
			{
				for (size_t j = jBegin; j < jEnd; ++j)
				{
					// i.e. (1, 1, 1)
					size_t i = 1 - (j + k) % 2 + iBegin;
					
					for (; i < iEnd; i += 2)
					{
						double r =
							((i > 0) ? A(i - 1, j, k).right * refX(i - 1, j, k) : 0.0) +
							((i + 1 < size.x) ? A(i, j, k).right * refX(i + 1, j, k) : 0.0) +
							((j > 0) ? A(i, j - 1, k).up * refX(i, j - 1, k) : 0.0) +
							((j + 1 < size.y) ? A(i, j, k).up * refX(i, j + 1, k) : 0.0) +
							((k > 0) ? A(i, j, k - 1).front * refX(i, j, k - 1) : 0.0) +
							((k + 1 < size.z) ? A(i, j, k).front * refX(i, j, k + 1) : 0.0);

						refX(i, j, k) = (1.0 - sorFactor) * refX(i, j, k) +
							sorFactor * (b(i, j, k) - r) / A(i, j, k).center;
					}
				}
			}
		});
	}

	void FDMGaussSeidelSolver3::ClearUncompressedVectors()
	{
		m_residual.Clear();
	}

	void FDMGaussSeidelSolver3::ClearCompressedVectors()
	{
		m_residualComp.Clear();
	}
}