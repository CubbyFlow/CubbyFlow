/*************************************************************************
> File Name: FDMCGSolver3.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 3-D finite difference-type linear system solver using conjugate gradient.
> Created Time: 2017/08/16
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FDM_CG_SOLVER3_H
#define CUBBYFLOW_FDM_CG_SOLVER3_H

#include <Core/Solver/FDM/FDMLinearSystemSolver3.h>

namespace CubbyFlow
{
	//! \brief 3-D finite difference-type linear system solver using conjugate gradient.
	class FDMCGSolver3 final : public FDMLinearSystemSolver3
	{
	public:
		//! Constructs the solver with given parameters.
		FDMCGSolver3(unsigned int maxNumberOfIterations, double tolerance);

		//! Solves the given linear system.
		bool Solve(FDMLinearSystem3* system) override;

		//! Solves the given compressed linear system.
		bool SolveCompressed(FDMCompressedLinearSystem3* system) override;

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
		FDMVector3 m_r;
		FDMVector3 m_d;
		FDMVector3 m_q;
		FDMVector3 m_s;

		// Compressed vectors
		VectorND m_rComp;
		VectorND m_dComp;
		VectorND m_qComp;
		VectorND m_sComp;

		void ClearUncompressedVectors();
		void ClearCompressedVectors();
	};

	//! Shared pointer type for the FDMCGSolver3.
	using FDMCGSolver3Ptr = std::shared_ptr<FDMCGSolver3>;
}

#endif