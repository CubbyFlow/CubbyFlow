// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_FDM_JACOBI_SOLVER2_HPP
#define CUBBYFLOW_FDM_JACOBI_SOLVER2_HPP

#include <Core/Solver/FDM/FDMLinearSystemSolver2.hpp>

namespace CubbyFlow
{
//! \brief 2-D finite difference-type linear system solver using Jacobi method.
class FDMJacobiSolver2 final : public FDMLinearSystemSolver2
{
 public:
    //! Constructs the solver with given parameters.
    FDMJacobiSolver2(unsigned int maxNumberOfIterations,
                     unsigned int residualCheckInterval, double tolerance);

    //! Solves the given linear system.
    bool Solve(FDMLinearSystem2* system) override;

    //! Solves the given compressed linear system.
    bool SolveCompressed(FDMCompressedLinearSystem2* system) override;

    //! Returns the max number of Jacobi iterations.
    [[nodiscard]] unsigned int GetMaxNumberOfIterations() const;

    //! Returns the last number of Jacobi iterations the solver made.
    [[nodiscard]] unsigned int GetLastNumberOfIterations() const;

    //! Returns the max residual tolerance for the Jacobi method.
    [[nodiscard]] double GetTolerance() const;

    //! Returns the last residual after the Jacobi iterations.
    [[nodiscard]] double GetLastResidual() const;

    //! Performs single Jacobi relaxation step.
    static void Relax(const FDMMatrix2& A, const FDMVector2& b, FDMVector2* x,
                      FDMVector2* xTemp);

    //! Performs single Jacobi relaxation step for compressed sys.
    static void Relax(const MatrixCSRD& A, const VectorND& b, VectorND* x,
                      VectorND* xTemp);

 private:
    void ClearUncompressedVectors();
    void ClearCompressedVectors();

    // Uncompressed vectors
    FDMVector2 m_xTemp;
    FDMVector2 m_residual;

    // Compressed vectors
    VectorND m_xTempComp;
    VectorND m_residualComp;

    unsigned int m_maxNumberOfIterations;
    unsigned int m_lastNumberOfIterations;
    unsigned int m_residualCheckInterval;
    double m_tolerance;
    double m_lastResidual;
};

//! Shared pointer type for the FDMJacobiSolver2.
using FDMJacobiSolver2Ptr = std::shared_ptr<FDMJacobiSolver2>;
}  // namespace CubbyFlow

#endif