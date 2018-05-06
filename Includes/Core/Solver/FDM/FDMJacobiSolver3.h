/*************************************************************************
> File Name: FDMJacobiSolver3.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 3-D finite difference-type linear system solver using Jacobi method.
> Created Time: 2017/08/17
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FDM_JACOBI_SOLVER3_H
#define CUBBYFLOW_FDM_JACOBI_SOLVER3_H

#include <Core/Solver/FDM/FDMLinearSystemSolver3.h>

namespace CubbyFlow
{
	//! \brief 3-D finite difference-type linear system solver using Jacobi method.
	class FDMJacobiSolver3 final : public FDMLinearSystemSolver3
	{
	public:
		//! Constructs the solver with given parameters.
		FDMJacobiSolver3(
			unsigned int maxNumberOfIterations,
			unsigned int residualCheckInterval,
			double tolerance);

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

		//! Performs single Jacobi relaxation step.
		static void Relax(const FDMMatrix3& A, const FDMVector3& b, FDMVector3* x, FDMVector3* xTemp);

		//! Performs single Jacobi relaxation step for compressed sys.
		static void Relax(const MatrixCSRD& A, const VectorND& b, VectorND* x, VectorND* xTemp);

	private:
		unsigned int m_maxNumberOfIterations;
		unsigned int m_lastNumberOfIterations;
		unsigned int m_residualCheckInterval;
		double m_tolerance;
		double m_lastResidual;

		// Uncompressed vectors
		FDMVector3 m_xTemp;
		FDMVector3 m_residual;

		// Compressed vectors
		VectorND m_xTempComp;
		VectorND m_residualComp;

		void ClearUncompressedVectors();
		void ClearCompressedVectors();
	};

	//! Shared pointer type for the FDMJacobiSolver3.
	using FDMJacobiSolver3Ptr = std::shared_ptr<FDMJacobiSolver3>;
}

#endif