/*************************************************************************
> File Name: FDMICCGSolver2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D finite difference-type linear system solver using incomplete
>          Cholesky conjugate gradient (ICCG).
> Created Time: 2017/08/12
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FDM_ICCG_SOLVER2_H
#define CUBBYFLOW_FDM_ICCG_SOLVER2_H

#include <Core/Solver/FDM/FDMLinearSystemSolver2.h>

namespace CubbyFlow
{
	//!
	//! \brief 2-D finite difference-type linear system solver using incomplete
	//!        Cholesky conjugate gradient (ICCG).
	//!
	class FDMICCGSolver2 final : public FDMLinearSystemSolver2
	{
	public:
		//! Constructs the solver with given parameters.
		FDMICCGSolver2(unsigned int maxNumberOfIterations, double tolerance);

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
		struct Preconditioner final
		{
			ConstArrayAccessor2<FDMMatrixRow2> A;
			FDMVector2 d;
			FDMVector2 y;

			void Build(const FDMMatrix2& matrix);

			void Solve(const FDMVector2& b, FDMVector2* x);
		};

		struct PreconditionerCompressed final
		{
			const MatrixCSRD* A;
			VectorND d;
			VectorND y;

			void Build(const MatrixCSRD& matrix);

			void Solve(const VectorND& b, VectorND* x);
		};

		unsigned int m_maxNumberOfIterations;
		unsigned int m_lastNumberOfIterations;
		double m_tolerance;
		double m_lastResidualNorm;

		// Uncompressed vectors and preconditioner
		FDMVector2 m_r;
		FDMVector2 m_d;
		FDMVector2 m_q;
		FDMVector2 m_s;
		Preconditioner m_precond;

		// Compressed vectors and preconditioner
		VectorND m_rComp;
		VectorND m_dComp;
		VectorND m_qComp;
		VectorND m_sComp;
		PreconditionerCompressed m_precondComp;

		void ClearUncompressedVectors();
		void ClearCompressedVectors();
	};

	//! Shared pointer type for the FDMICCGSolver2.
	using FDMICCGSolver2Ptr = std::shared_ptr<FDMICCGSolver2>;
}

#endif