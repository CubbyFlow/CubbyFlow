/*************************************************************************
> File Name: FDMMGPCGSolver3.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D finite difference-type linear system solver using Multigrid
>          Preconditioned conjugate gradient (MGPCG).
> Created Time: 2017/11/05
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Math/CG.hpp>
#include <Core/Solver/FDM/FDMMGPCGSolver3.h>
#include <Core/Utils/Logging.h>

namespace CubbyFlow
{
	void FDMMGPCGSolver3::Preconditioner::Build(FDMMGLinearSystem3* _system, MGParameters<FDMBLAS3> _mgParams)
	{
		system = _system;
		mgParams = _mgParams;
	}

	void FDMMGPCGSolver3::Preconditioner::Solve(const FDMVector3& b, FDMVector3* x) const
	{
		// Copy dimension
		FDMMGVector3 mgX = system->x;
		FDMMGVector3 mgB = system->x;
		FDMMGVector3 mgBuffer = system->x;

		// Copy input to the top
		mgX.levels.front().Set(*x);
		mgB.levels.front().Set(b);

		MGVCycle(system->A, mgParams, &mgX, &mgB, &mgBuffer);

		// Copy result to the output
		x->Set(mgX.levels.front());
	}

	FDMMGPCGSolver3::FDMMGPCGSolver3(
		unsigned int numberOfCGIter,
		size_t maxNumberOfLevels,
		unsigned int numberOfRestrictionIter,
		unsigned int numberOfCorrectionIter,
		unsigned int numberOfCoarsestIter,
		unsigned int numberOfFinalIter,
		double maxTolerance,
		double sorFactor,
		bool useRedBlackOrdering)
		: FDMMGSolver3(
			maxNumberOfLevels, numberOfRestrictionIter,
			numberOfCorrectionIter, numberOfCoarsestIter,
			numberOfFinalIter, maxTolerance, sorFactor, useRedBlackOrdering),
		m_maxNumberOfIterations(numberOfCGIter),
		m_lastNumberOfIterations(0),
		m_tolerance(maxTolerance),
		m_lastResidualNorm(std::numeric_limits<double>::max())
	{
		// Do nothing
	}

	bool FDMMGPCGSolver3::Solve(FDMMGLinearSystem3* system)
	{
		const Size3 size = system->A.levels.front().size();
		m_r.Resize(size);
		m_d.Resize(size);
		m_q.Resize(size);
		m_s.Resize(size);

		system->x.levels.front().Set(0.0);
		m_r.Set(0.0);
		m_d.Set(0.0);
		m_q.Set(0.0);
		m_s.Set(0.0);

		m_precond.Build(system, GetParams());

		PCG<FDMBLAS3, Preconditioner>(
			system->A.levels.front(),
			system->b.levels.front(),
			m_maxNumberOfIterations, m_tolerance, &m_precond,
			&system->x.levels.front(), &m_r, &m_d, &m_q, &m_s,
			&m_lastNumberOfIterations, &m_lastResidualNorm);

		CUBBYFLOW_INFO << "Residual after solving MGPCG: " << m_lastResidualNorm
			<< " Number of MGPCG iterations: " << m_lastNumberOfIterations;

		return m_lastResidualNorm <= m_tolerance || m_lastNumberOfIterations < m_maxNumberOfIterations;
	}

	unsigned int FDMMGPCGSolver3::GetMaxNumberOfIterations() const
	{
		return m_maxNumberOfIterations;
	}

	unsigned int FDMMGPCGSolver3::GetLastNumberOfIterations() const
	{
		return m_lastNumberOfIterations;
	}

	double FDMMGPCGSolver3::GetTolerance() const
	{
		return m_tolerance;
	}

	double FDMMGPCGSolver3::GetLastResidual() const
	{
		return m_lastResidualNorm;
	}
}