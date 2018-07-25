/*************************************************************************
> File Name: FDMMGSolver2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D finite difference-type linear system solver using Multigrid.
> Created Time: 2017/11/03
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FDM_MG_SOLVER2_H
#define CUBBYFLOW_FDM_MG_SOLVER2_H

#include <Core/FDM/FDMMGLinearSystem2.h>
#include <Core/Solver/FDM/FDMLinearSystemSolver2.h>
#include <Core/Utils/MG.h>

namespace CubbyFlow
{
	//! \brief 2-D finite difference-type linear system solver using Multigrid.
	class FDMMGSolver2 : public FDMLinearSystemSolver2
	{
	public:
		//! Constructs the solver with given parameters.
		FDMMGSolver2(
			size_t maxNumberOfLevels,
			unsigned int numberOfRestrictionIter = 5,
			unsigned int numberOfCorrectionIter = 5,
			unsigned int numberOfCoarsestIter = 20,
			unsigned int numberOfFinalIter = 20,
			double maxTolerance = 1e-9,
			double sorFactor = 1.5,
			bool useRedBlackOrdering = false);

		//! Returns the Multigrid parameters.
		const MGParameters<FDMBLAS2>& GetParams() const;

		//! Returns the SOR (Successive Over Relaxation) factor.
		double GetSORFactor() const;

		//! Returns true if red-black ordering is enabled.
		bool GetUseRedBlackOrdering() const;

		//! No-op. Multigrid-type solvers do not solve FDMLinearSystem2.
		bool Solve(FDMLinearSystem2* system) final;

		//! Solves Multigrid linear system.
		virtual bool Solve(FDMMGLinearSystem2* system);

	private:
		MGParameters<FDMBLAS2> m_mgParams;
		double m_sorFactor;
		bool m_useRedBlackOrdering;
	};

	//! Shared pointer type for the FDMMGSolver2.
	using FDMMGSolver2Ptr = std::shared_ptr<FDMMGSolver2>;
}

#endif