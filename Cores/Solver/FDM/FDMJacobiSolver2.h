/*************************************************************************
> File Name: FDMJacobiSolver2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D finite difference-type linear system solver using Jacobi method.
> Created Time: 2017/08/17
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FDM_JACOBI_SOLVER2_H
#define CUBBYFLOW_FDM_JACOBI_SOLVER2_H

#include <Solver/FDM/FDMLinearSystemSolver2.h>

namespace CubbyFlow
{
	//! \brief 2-D finite difference-type linear system solver using Jacobi method.
	class FDMJacobiSolver2 final : public FDMLinearSystemSolver2
	{
	public:
		//! Constructs the solver with given parameters.
		FDMJacobiSolver2(
			unsigned int maxNumberOfIterations,
			unsigned int residualCheckInterval,
			double tolerance);

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

		//! Performs single Jacobi relaxation step.
		static void Relax(const FDMMatrix2& A, const FDMVector2& b, FDMVector2* x, FDMVector2* xTemp);

		//! Performs single Jacobi relaxation step for compressed sys.
		static void Relax(const MatrixCSRD& A, const VectorND& b, VectorND* x, VectorND* xTemp);

	private:
		unsigned int m_maxNumberOfIterations;
		unsigned int m_lastNumberOfIterations;
		unsigned int m_residualCheckInterval;
		double m_tolerance;
		double m_lastResidual;

		// Uncompressed vectors
		FDMVector2 m_xTemp;
		FDMVector2 m_residual;

		// Compressed vectors
		VectorND m_xTempComp;
		VectorND m_residualComp;

		void ClearUncompressedVectors();
		void ClearCompressedVectors();
	};

	//! Shared pointer type for the FDMJacobiSolver2.
	using FDMJacobiSolver2Ptr = std::shared_ptr<FDMJacobiSolver2>;
}

#endif