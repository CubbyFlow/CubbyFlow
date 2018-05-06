/*************************************************************************
> File Name: FDMCGSolver2.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 2-D finite difference-type linear system solver using conjugate gradient.
> Created Time: 2017/08/16
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FDM_CG_SOLVER2_H
#define CUBBYFLOW_FDM_CG_SOLVER2_H

#include <Core/Solver/FDM/FDMLinearSystemSolver2.h>

namespace CubbyFlow
{
	//! \brief 2-D finite difference-type linear system solver using conjugate gradient.
	class FDMCGSolver2 final : public FDMLinearSystemSolver2
	{
	public:
		//! Constructs the solver with given parameters.
		FDMCGSolver2(unsigned int maxNumberOfIterations, double tolerance);

		//! Solves the given linear system.
		bool Solve(FDMLinearSystem2* system) override;

		//! Solves the given compressed linear system.
		bool SolveCompressed(FDMCompressedLinearSystem2* system) override;

		//! Returns the max number of Jacobi iterations.
		unsigned int GetMaxNumberOfIterations() const;

		//! Returns the last number of Jacobi iterations the solver made.
		unsigned int GetLastNumberOfIterations() const;

		//! Returns the max residual tolerance for the Jacobi method.
		double GetTolerance() const;

		//! Returns the last residual after the Jacobi iterations.
		double GetLastResidual() const;

	private:
		unsigned int m_maxNumberOfIterations;
		unsigned int m_lastNumberOfIterations;
		double m_tolerance;
		double m_lastResidual;

		// Uncompressed vectors
		FDMVector2 m_r;
		FDMVector2 m_d;
		FDMVector2 m_q;
		FDMVector2 m_s;

		// Compressed vectors
		VectorND m_rComp;
		VectorND m_dComp;
		VectorND m_qComp;
		VectorND m_sComp;

		void ClearUncompressedVectors();
		void ClearCompressedVectors();
	};

	//! Shared pointer type for the FDMCGSolver2.
	using FDMCGSolver2Ptr = std::shared_ptr<FDMCGSolver2>;
}

#endif