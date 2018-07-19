/*************************************************************************
> File Name: FDMICCGSolver3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D finite difference-type linear system solver using incomplete
>          Cholesky conjugate gradient (ICCG).
> Created Time: 2017/08/12
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FDM_ICCG_SOLVER3_H
#define CUBBYFLOW_FDM_ICCG_SOLVER3_H

#include <Solver/FDM/FDMLinearSystemSolver3.h>

namespace CubbyFlow
{
	//!
	//! \brief 3-D finite difference-type linear system solver using incomplete
	//!        Cholesky conjugate gradient (ICCG).
	//!
	class FDMICCGSolver3 final : public FDMLinearSystemSolver3
	{
	public:
		//! Constructs the solver with given parameters.
		FDMICCGSolver3(unsigned int maxNumberOfIterations, double tolerance);

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
		struct Preconditioner final
		{
			ConstArrayAccessor3<FDMMatrixRow3> A;
			FDMVector3 d;
			FDMVector3 y;

			void Build(const FDMMatrix3& matrix);

			void Solve(const FDMVector3& b, FDMVector3* x);
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
		FDMVector3 m_r;
		FDMVector3 m_d;
		FDMVector3 m_q;
		FDMVector3 m_s;
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

	//! Shared pointer type for the FDMICCGSolver3.
	using FDMICCGSolver3Ptr = std::shared_ptr<FDMICCGSolver3>;
}

#endif