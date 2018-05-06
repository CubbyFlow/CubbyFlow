/*************************************************************************
> File Name: FDMMGSolver3.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 3-D finite difference-type linear system solver using Multigrid.
> Created Time: 2017/11/03
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FDM_MG_SOLVER3_H
#define CUBBYFLOW_FDM_MG_SOLVER3_H

#include <Core/FDM/FDMMGLinearSystem3.h>
#include <Core/Solver/FDM/FDMLinearSystemSolver3.h>
#include <Core/Utils/MG.h>

namespace CubbyFlow
{
	//! \brief 3-D finite difference-type linear system solver using Multigrid.
	class FDMMGSolver3 : public FDMLinearSystemSolver3
	{
	public:
		//! Constructs the solver with given parameters.
		FDMMGSolver3(
			size_t maxNumberOfLevels,
			unsigned int numberOfRestrictionIter = 5,
			unsigned int numberOfCorrectionIter = 5,
			unsigned int numberOfCoarsestIter = 20,
			unsigned int numberOfFinalIter = 20,
			double maxTolerance = 1e-9,
			double sorFactor = 1.5,
			bool useRedBlackOrdering = false);

		//! Returns the Multigrid parameters.
		const MGParameters<FDMBLAS3>& GetParams() const;

		//! Returns the SOR (Successive Over Relaxation) factor.
		double GetSORFactor() const;

		//! Returns true if red-black ordering is enabled.
		bool GetUseRedBlackOrdering() const;

		//! No-op. Multigrid-type solvers do not solve FDMLinearSystem3.
		bool Solve(FDMLinearSystem3* system) final;

		//! Solves Multigrid linear system.
		virtual bool Solve(FDMMGLinearSystem3* system);

	private:
		MGParameters<FDMBLAS3> m_mgParams;
		double m_sorFactor;
		bool m_useRedBlackOrdering;
	};

	//! Shared pointer type for the FDMMGSolver3.
	using FDMMGSolver3Ptr = std::shared_ptr<FDMMGSolver3>;
}

#endif