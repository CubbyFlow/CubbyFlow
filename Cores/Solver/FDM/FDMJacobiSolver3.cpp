/*************************************************************************
> File Name: FDMJacobiSolver3.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D finite difference-type linear system solver using Jacobi method.
> Created Time: 2017/08/17
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Solver/FDM/FDMJacobiSolver3.h>

namespace CubbyFlow
{
	FDMJacobiSolver3::FDMJacobiSolver3(
		unsigned int maxNumberOfIterations,
		unsigned int residualCheckInterval,
		double tolerance) :
		m_maxNumberOfIterations(maxNumberOfIterations),
		m_lastNumberOfIterations(0),
		m_residualCheckInterval(residualCheckInterval),
		m_tolerance(tolerance),
		m_lastResidual(std::numeric_limits<double>::max())
	{
		// Do nothing
	}

	bool FDMJacobiSolver3::Solve(FDMLinearSystem3* system)
	{
		ClearCompressedVectors();

		m_xTemp.Resize(system->x.size());
		m_residual.Resize(system->x.size());

		m_lastNumberOfIterations = m_maxNumberOfIterations;

		for (unsigned int iter = 0; iter < m_maxNumberOfIterations; ++iter)
		{
			Relax(system->A, system->b, &system->x, &m_xTemp);
			m_xTemp.Swap(system->x);

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

	bool FDMJacobiSolver3::SolveCompressed(FDMCompressedLinearSystem3* system)
	{
		ClearUncompressedVectors();

		m_xTempComp.Resize(system->x.size());
		m_residualComp.Resize(system->x.size());

		m_lastNumberOfIterations = m_maxNumberOfIterations;

		for (unsigned int iter = 0; iter < m_maxNumberOfIterations; ++iter)
		{
			Relax(system->A, system->b, &system->x, &m_xTempComp);

			m_xTempComp.Swap(system->x);

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

	unsigned int FDMJacobiSolver3::GetMaxNumberOfIterations() const
	{
		return m_maxNumberOfIterations;
	}

	unsigned int FDMJacobiSolver3::GetLastNumberOfIterations() const
	{
		return m_lastNumberOfIterations;
	}

	double FDMJacobiSolver3::GetTolerance() const
	{
		return m_tolerance;
	}

	double FDMJacobiSolver3::GetLastResidual() const
	{
		return m_lastResidual;
	}

	void FDMJacobiSolver3::Relax(const FDMMatrix3& A, const FDMVector3& b, FDMVector3* x, FDMVector3* xTemp)
	{
		Size3 size = A.size();
		FDMVector3& refX = *x;
		FDMVector3& refXTemp = *xTemp;

		A.ParallelForEachIndex([&](size_t i, size_t j, size_t k)
		{
			double r =
				((i > 0) ? A(i - 1, j, k).right * refX(i - 1, j, k) : 0.0) +
				((i + 1 < size.x) ? A(i, j, k).right * refX(i + 1, j, k) : 0.0) +
				((j > 0) ? A(i, j - 1, k).up * refX(i, j - 1, k) : 0.0) +
				((j + 1 < size.y) ? A(i, j, k).up * refX(i, j + 1, k) : 0.0) +
				((k > 0) ? A(i, j, k - 1).front * refX(i, j, k - 1) : 0.0) +
				((k + 1 < size.z) ? A(i, j, k).front * refX(i, j, k + 1) : 0.0);

			refXTemp(i, j, k) = (b(i, j, k) - r) / A(i, j, k).center;
		});
	}

	void FDMJacobiSolver3::Relax(const MatrixCSRD& A, const VectorND& b, VectorND* x_, VectorND* xTemp_)
	{
		const auto rp = A.RowPointersBegin();
		const auto ci = A.ColumnIndicesBegin();
		const auto nnz = A.NonZeroBegin();

		VectorND& x = *x_;
		VectorND& xTemp = *xTemp_;

		b.ParallelForEachIndex([&](size_t i)
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

			xTemp[i] = (b[i] - r) / diag;
		});
	}


	void FDMJacobiSolver3::ClearUncompressedVectors()
	{
		m_xTempComp.Clear();
		m_residualComp.Clear();
	}

	void FDMJacobiSolver3::ClearCompressedVectors()
	{
		m_xTemp.Clear();
		m_residual.Clear();
	}
}