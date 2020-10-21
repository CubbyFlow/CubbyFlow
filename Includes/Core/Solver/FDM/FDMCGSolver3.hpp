// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_FDM_CG_SOLVER3_HPP
#define CUBBYFLOW_FDM_CG_SOLVER3_HPP

#include <Core/Solver/FDM/FDMLinearSystemSolver3.hpp>

namespace CubbyFlow
{
//! \brief 3-D finite difference-type linear system solver using conjugate
//! gradient.
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
    [[nodiscard]] unsigned int GetMaxNumberOfIterations() const;

    //! Returns the last number of Jacobi iterations the solver made.
    [[nodiscard]] unsigned int GetLastNumberOfIterations() const;

    //! Returns the max residual tolerance for the Jacobi method.
    [[nodiscard]] double GetTolerance() const;

    //! Returns the last residual after the Jacobi iterations.
    [[nodiscard]] double GetLastResidual() const;

 private:
    void ClearUncompressedVectors();
    void ClearCompressedVectors();

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

    unsigned int m_maxNumberOfIterations;
    unsigned int m_lastNumberOfIterations;
    double m_tolerance;
    double m_lastResidual;
};

//! Shared pointer type for the FDMCGSolver3.
using FDMCGSolver3Ptr = std::shared_ptr<FDMCGSolver3>;
}  // namespace CubbyFlow

#endif