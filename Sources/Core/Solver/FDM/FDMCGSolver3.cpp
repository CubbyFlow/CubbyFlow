/*************************************************************************
> File Name: FDMCGSolver3.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 3-D finite difference-type linear system solver using conjugate gradient.
> Created Time: 2017/08/16
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Math/CG.h>
#include <Core/Solver/FDM/FDMCGSolver3.h>

namespace CubbyFlow
{
	FDMCGSolver3::FDMCGSolver3(unsigned int maxNumberOfIterations, double tolerance) :
		m_maxNumberOfIterations(maxNumberOfIterations),
		m_lastNumberOfIterations(0),
		m_tolerance(tolerance),
		m_lastResidual(std::numeric_limits<double>::max())
	{
		// Do nothing
	}

	bool FDMCGSolver3::Solve(FDMLinearSystem3* system)
	{
		FDMMatrix3& matrix = system->A;
		FDMVector3& solution = system->x;
		FDMVector3& rhs = system->b;

		assert(matrix.size() == rhs.size());
		assert(matrix.size() == solution.size());

		ClearCompressedVectors();

		const Size3 size = matrix.size();
		m_r.Resize(size);
		m_d.Resize(size);
		m_q.Resize(size);
		m_s.Resize(size);

		system->x.Set(0.0);
		m_r.Set(0.0);
		m_d.Set(0.0);
		m_q.Set(0.0);
		m_s.Set(0.0);

		CG<FDMBLAS3>(matrix, rhs, m_maxNumberOfIterations, m_tolerance, &solution,
			&m_r, &m_d, &m_q, &m_s, &m_lastNumberOfIterations, &m_lastResidual);

		return (m_lastResidual <= m_tolerance) || (m_lastNumberOfIterations < m_maxNumberOfIterations);
	}

	bool FDMCGSolver3::SolveCompressed(FDMCompressedLinearSystem3* system)
	{
		MatrixCSRD& matrix = system->A;
		VectorND& solution = system->x;
		VectorND& rhs = system->b;

		ClearUncompressedVectors();

		const size_t size = solution.size();
		m_rComp.Resize(size);
		m_dComp.Resize(size);
		m_qComp.Resize(size);
		m_sComp.Resize(size);

		system->x.Set(0.0);
		m_rComp.Set(0.0);
		m_dComp.Set(0.0);
		m_qComp.Set(0.0);
		m_sComp.Set(0.0);

		CG<FDMCompressedBLAS3>(matrix, rhs, m_maxNumberOfIterations, m_tolerance, &solution,
			&m_rComp, &m_dComp, &m_qComp, &m_sComp, &m_lastNumberOfIterations, &m_lastResidual);

		return (m_lastResidual <= m_tolerance) || (m_lastNumberOfIterations < m_maxNumberOfIterations);
	}

	unsigned int FDMCGSolver3::GetMaxNumberOfIterations() const
	{
		return m_maxNumberOfIterations;
	}

	unsigned int FDMCGSolver3::GetLastNumberOfIterations() const
	{
		return m_lastNumberOfIterations;
	}

	double FDMCGSolver3::GetTolerance() const
	{
		return m_tolerance;
	}

	double FDMCGSolver3::GetLastResidual() const
	{
		return m_lastResidual;
	}

	void FDMCGSolver3::ClearUncompressedVectors()
	{
		m_r.Clear();
		m_d.Clear();
		m_q.Clear();
		m_s.Clear();
	}

	void FDMCGSolver3::ClearCompressedVectors()
	{
		m_rComp.Clear();
		m_dComp.Clear();
		m_qComp.Clear();
		m_sComp.Clear();
	}
}