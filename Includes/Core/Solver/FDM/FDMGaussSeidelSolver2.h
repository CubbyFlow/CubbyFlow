/*************************************************************************
> File Name: FDMGaussSeidelSolver2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D finite difference-type linear system solver using Gauss-Seidel method.
> Created Time: 2017/08/17
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FDM_GAUSS_SEIDEL_SOLVER2_H
#define CUBBYFLOW_FDM_GAUSS_SEIDEL_SOLVER2_H

#include <Core/Solver/FDM/FDMLinearSystemSolver2.h>

namespace CubbyFlow
{
	//! \brief 2-D finite difference-type linear system solver using Gauss-Seidel method.
	class FDMGaussSeidelSolver2 final : public FDMLinearSystemSolver2
	{
	public:
		//! Constructs the solver with given parameters.
		FDMGaussSeidelSolver2(
			unsigned int maxNumberOfIterations,
			unsigned int residualCheckInterval,
			double tolerance,
			double sorFactor = 1.0,
			bool useRedBlackOrdering = false);

		//! Solves the given linear system.
		bool Solve(FDMLinearSystem2* system) override;

		//! Solves the given compressed linear system.
		bool SolveCompressed(FDMCompressedLinearSystem2* system) override;

		//! Returns the max number of Gauss-Seidel iterations.
		unsigned int GetMaxNumberOfIterations() const;

		//! Returns the last number of Gauss-Seidel iterations the solver made.
		unsigned int GetLastNumberOfIterations() const;

		//! Returns the max residual tolerance for the Gauss-Seidel method.
		double GetTolerance() const;

		//! Returns the last residual after the Gauss-Seidel iterations.
		double GetLastResidual() const;

		//! Returns the SOR (Successive Over Relaxation) factor.
		double GetSORFactor() const;

		//! Returns true if red-black ordering is enabled.
		bool GetUseRedBlackOrdering() const;

		//! Performs single natural Gauss-Seidel relaxation step.
		static void Relax(const FDMMatrix2& A, const FDMVector2& b, double sorFactor, FDMVector2* x);

		//! Performs single natural Gauss-Seidel relaxation step for compressed sys.
		static void Relax(const MatrixCSRD& A, const VectorND& b, double sorFactor, VectorND* x);

		//! Performs single Red-Black Gauss-Seidel relaxation step.
		static void RelaxRedBlack(const FDMMatrix2& A, const FDMVector2& b, double sorFactor, FDMVector2* x);

	private:
		unsigned int m_maxNumberOfIterations;
		unsigned int m_lastNumberOfIterations;
		unsigned int m_residualCheckInterval;
		double m_tolerance;
		double m_lastResidual;
		double m_sorFactor;
		double m_useRedBlackOrdering;

		// Uncompressed vectors
		FDMVector2 m_residual;

		// Compressed vectors
		VectorND m_residualComp;

		void ClearUncompressedVectors();
		void ClearCompressedVectors();
	};

	//! Shared pointer type for the FDMGaussSeidelSolver2.
	using FDMGaussSeidelSolver2Ptr = std::shared_ptr<FDMGaussSeidelSolver2>;
}

#endif