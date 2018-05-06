/*************************************************************************
> File Name: FDMMGSolver3.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 3-D finite difference-type linear system solver using Multigrid.
> Created Time: 2017/11/03
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Solver/FDM/FDMGaussSeidelSolver3.h>
#include <Core/Solver/FDM/FDMMGSolver3.h>
#include <Core/Utils/MG.h>

namespace CubbyFlow
{
	FDMMGSolver3::FDMMGSolver3(
		size_t maxNumberOfLevels,
		unsigned int numberOfRestrictionIter,
		unsigned int numberOfCorrectionIter,
		unsigned int numberOfCoarsestIter,
		unsigned int numberOfFinalIter,
		double maxTolerance,
		double sorFactor,
		bool useRedBlackOrdering)
	{
		m_mgParams.maxNumberOfLevels = maxNumberOfLevels;
		m_mgParams.numberOfRestrictionIter = numberOfRestrictionIter;
		m_mgParams.numberOfCorrectionIter = numberOfCorrectionIter;
		m_mgParams.numberOfCoarsestIter = numberOfCoarsestIter;
		m_mgParams.numberOfFinalIter = numberOfFinalIter;
		m_mgParams.maxTolerance = maxTolerance;

		if (useRedBlackOrdering)
		{
			m_mgParams.relaxFunc = [sorFactor](const FDMMatrix3& A, const FDMVector3& b,
				unsigned int numberOfIterations, double maxTolerance, FDMVector3* x, FDMVector3* buffer)
			{
				UNUSED_VARIABLE(maxTolerance);
				UNUSED_VARIABLE(buffer);

				for (unsigned int iter = 0; iter < numberOfIterations; ++iter)
				{
					FDMGaussSeidelSolver3::RelaxRedBlack(A, b, sorFactor, x);
				}
			};
		}
		else
		{
			m_mgParams.relaxFunc = [sorFactor](const FDMMatrix3& A, const FDMVector3& b,
				unsigned int numberOfIterations, double maxTolerance, FDMVector3* x, FDMVector3* buffer)
			{
				UNUSED_VARIABLE(maxTolerance);
				UNUSED_VARIABLE(buffer);

				for (unsigned int iter = 0; iter < numberOfIterations; ++iter)
				{
					FDMGaussSeidelSolver3::Relax(A, b, sorFactor, x);
				}
			};
		}

		m_mgParams.restrictFunc = FDMMGUtils3::Restrict;
		m_mgParams.correctFunc = FDMMGUtils3::Correct;
		m_sorFactor = sorFactor;
		m_useRedBlackOrdering = useRedBlackOrdering;
	}

	const MGParameters<FDMBLAS3>& FDMMGSolver3::GetParams() const
	{
		return m_mgParams;
	}

	double FDMMGSolver3::GetSORFactor() const
	{
		return m_sorFactor;
	}

	bool FDMMGSolver3::GetUseRedBlackOrdering() const
	{
		return m_useRedBlackOrdering;
	}

	bool FDMMGSolver3::Solve(FDMLinearSystem3* system)
	{
		UNUSED_VARIABLE(system);

		return false;
	}

	bool FDMMGSolver3::Solve(FDMMGLinearSystem3* system)
	{
		FDMMGVector3 buffer = system->x;
		auto result = MGVCycle(system->A, m_mgParams, &system->x, &system->b, &buffer);
		return result.lastResidualNorm < m_mgParams.maxTolerance;
	}
}