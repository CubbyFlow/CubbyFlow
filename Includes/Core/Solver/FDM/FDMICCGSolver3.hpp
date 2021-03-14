// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_FDM_ICCG_SOLVER3_HPP
#define CUBBYFLOW_FDM_ICCG_SOLVER3_HPP

#include <Core/Array/ArrayView.hpp>
#include <Core/Solver/FDM/FDMLinearSystemSolver3.hpp>

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
    [[nodiscard]] unsigned int GetMaxNumberOfIterations() const;

    //! Returns the last number of Jacobi iterations the solver made.
    [[nodiscard]] unsigned int GetLastNumberOfIterations() const;

    //! Returns the max residual tolerance for the Jacobi method.
    [[nodiscard]] double GetTolerance() const;

    //! Returns the last residual after the Jacobi iterations.
    [[nodiscard]] double GetLastResidual() const;

 private:
    struct Preconditioner final
    {
        void Build(const FDMMatrix3& matrix);

        void Solve(const FDMVector3& b, FDMVector3* x);

        ConstArrayView3<FDMMatrixRow3> A;
        FDMVector3 d;
        FDMVector3 y;
    };

    struct PreconditionerCompressed final
    {
        void Build(const MatrixCSRD& matrix);

        void Solve(const VectorND& b, VectorND* x);

        const MatrixCSRD* A = nullptr;
        VectorND d;
        VectorND y;
    };

    void ClearUncompressedVectors();
    void ClearCompressedVectors();

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

    unsigned int m_maxNumberOfIterations;
    unsigned int m_lastNumberOfIterations;
    double m_tolerance;
    double m_lastResidualNorm;
};

//! Shared pointer type for the FDMICCGSolver3.
using FDMICCGSolver3Ptr = std::shared_ptr<FDMICCGSolver3>;
}  // namespace CubbyFlow

#endif