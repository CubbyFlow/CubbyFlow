/*************************************************************************
> File Name: FDMMGPCGSolver3.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 3-D finite difference-type linear system solver using Multigrid
>          Preconditioned conjugate gradient (MGPCG).
> Created Time: 2017/11/05
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FDM_MGPCG_SOLVER3_H
#define CUBBYFLOW_FDM_MGPCG_SOLVER3_H

#include <Core/Solver/FDM/FDMMGSolver3.h>

namespace CubbyFlow
{
	//!
	//! \brief 3-D finite difference-type linear system solver using Multigrid
	//!        Preconditioned conjugate gradient (MGPCG).
	//!
	//! \see McAdams, Aleka, Eftychios Sifakis, and Joseph Teran.
	//!      "A parallel multigrid Poisson solver for fluids simulation on large
	//!      grids." Proceedings of the 2010 ACM SIGGRAPH/Eurographics Symposium on
	//!      Computer Animation. Eurographics Association, 2010.
	//!
	class FDMMGPCGSolver3 final : public FDMMGSolver3
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
		FDMMGPCGSolver3(
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
		bool Solve(FDMMGLinearSystem3* system) override;

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
			FDMMGLinearSystem3* system;
			MGParameters<FDMBLAS3> mgParams;

			void Build(FDMMGLinearSystem3* system, MGParameters<FDMBLAS3> mgParams);

			void Solve(const FDMVector3& b, FDMVector3* x) const;
		};

		unsigned int m_maxNumberOfIterations;
		unsigned int m_lastNumberOfIterations;
		double m_tolerance;
		double m_lastResidualNorm;

		FDMVector3 m_r;
		FDMVector3 m_d;
		FDMVector3 m_q;
		FDMVector3 m_s;
		Preconditioner m_precond;
	};

	//! Shared pointer type for the FDMMGPCGSolver3.
	using FDMMGPCGSolver3Ptr = std::shared_ptr<FDMMGPCGSolver3>;
}

#endif