/*************************************************************************
> File Name: FDMMGPCGSolver2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D finite difference-type linear system solver using Multigrid
>          Preconditioned conjugate gradient (MGPCG).
> Created Time: 2017/11/05
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FDM_MGPCG_SOLVER2_H
#define CUBBYFLOW_FDM_MGPCG_SOLVER2_H

#include <Core/Solver/FDM/FDMMGSolver2.h>

namespace CubbyFlow
{
	//!
	//! \brief 2-D finite difference-type linear system solver using Multigrid
	//!        Preconditioned conjugate gradient (MGPCG).
	//!
	//! \see McAdams, Aleka, Eftychios Sifakis, and Joseph Teran.
	//!      "A parallel multigrid Poisson solver for fluids simulation on large
	//!      grids." Proceedings of the 2010 ACM SIGGRAPH/Eurographics Symposium on
	//!      Computer Animation. Eurographics Association, 2010.
	//!
	class FDMMGPCGSolver2 final : public FDMMGSolver2
	{
	public:
		//!
		//! Constructs the solver with given parameters.
		//!
		//! \param numberOfCGIter - Number of CG iterations.
		//! \param maxNumberOfLevels - Number of maximum MG levels.
		//! \param numberOfRestrictionIter - Number of restriction iterations.
		//! \param numberOfCorrectionIter - Number of correction iterations.
		//! \param numberOfCoarsestIter - Number of iterations at the coarsest grid.
		//! \param numberOfFinalIter - Number of final iterations.
		//! \param maxTolerance - Number of max residual tolerance.
		FDMMGPCGSolver2(
			unsigned int numberOfCGIter,
			size_t maxNumberOfLevels,
			unsigned int numberOfRestrictionIter = 5,
			unsigned int numberOfCorrectionIter = 5,
			unsigned int numberOfCoarsestIter = 20,
			unsigned int numberOfFinalIter = 20,
			double maxTolerance = 1e-9,
			double sorFactor = 1.5,
			bool useRedBlackOrdering = false);

		//! Solves the given linear system.
		bool Solve(FDMMGLinearSystem2* system) override;

		//! Returns the max number of Jacobi iterations.
		unsigned int GetMaxNumberOfIterations() const;

		//! Returns the last number of Jacobi iterations the solver made.
		unsigned int GetLastNumberOfIterations() const;

		//! Returns the max residual tolerance for the Jacobi method.
		double GetTolerance() const;

		//! Returns the last residual after the Jacobi iterations.
		double GetLastResidual() const;

	private:
		struct Preconditioner final
		{
			FDMMGLinearSystem2* system;
			MGParameters<FDMBLAS2> mgParams;

			void Build(FDMMGLinearSystem2* system, MGParameters<FDMBLAS2> mgParams);

			void Solve(const FDMVector2& b, FDMVector2* x) const;
		};

		unsigned int m_maxNumberOfIterations;
		unsigned int m_lastNumberOfIterations;
		double m_tolerance;
		double m_lastResidualNorm;

		FDMVector2 m_r;
		FDMVector2 m_d;
		FDMVector2 m_q;
		FDMVector2 m_s;
		Preconditioner m_precond;
	};

	//! Shared pointer type for the FDMMGPCGSolver2.
	using FDMMGPCGSolver2Ptr = std::shared_ptr<FDMMGPCGSolver2>;
}

#endif