// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_FDM_GAUSS_SEIDEL_SOLVER2_HPP
#define CUBBYFLOW_FDM_GAUSS_SEIDEL_SOLVER2_HPP

#include <Core/Solver/FDM/FDMLinearSystemSolver2.hpp>

namespace CubbyFlow
{
//! \brief 2-D finite difference-type linear system solver using Gauss-Seidel
//! method.
class FDMGaussSeidelSolver2 final : public FDMLinearSystemSolver2
{
 public:
    //! Constructs the solver with given parameters.
    FDMGaussSeidelSolver2(unsigned int maxNumberOfIterations,
                          unsigned int residualCheckInterval, double tolerance,
                          double sorFactor = 1.0,
                          bool useRedBlackOrdering = false);

    //! Solves the given linear system.
    bool Solve(FDMLinearSystem2* system) override;

    //! Solves the given compressed linear system.
    bool SolveCompressed(FDMCompressedLinearSystem2* system) override;

    //! Returns the max number of Gauss-Seidel iterations.
    [[nodiscard]] unsigned int GetMaxNumberOfIterations() const;

    //! Returns the last number of Gauss-Seidel iterations the solver made.
    [[nodiscard]] unsigned int GetLastNumberOfIterations() const;

    //! Returns the max residual tolerance for the Gauss-Seidel method.
    [[nodiscard]] double GetTolerance() const;

    //! Returns the last residual after the Gauss-Seidel iterations.
    [[nodiscard]] double GetLastResidual() const;

    //! Returns the SOR (Successive Over Relaxation) factor.
    [[nodiscard]] double GetSORFactor() const;

    //! Returns true if red-black ordering is enabled.
    [[nodiscard]] bool GetUseRedBlackOrdering() const;

    //! Performs single natural Gauss-Seidel relaxation step.
    static void Relax(const FDMMatrix2& A, const FDMVector2& b,
                      double sorFactor, FDMVector2* x);

    //! Performs single natural Gauss-Seidel relaxation step for compressed sys.
    static void Relax(const MatrixCSRD& A, const VectorND& b, double sorFactor,
                      VectorND* x);

    //! Performs single Red-Black Gauss-Seidel relaxation step.
    static void RelaxRedBlack(const FDMMatrix2& A, const FDMVector2& b,
                              double sorFactor, FDMVector2* x);

 private:
    void ClearUncompressedVectors();
    void ClearCompressedVectors();

    // Uncompressed vectors
    FDMVector2 m_residual;

    // Compressed vectors
    VectorND m_residualComp;

    unsigned int m_maxNumberOfIterations;
    unsigned int m_lastNumberOfIterations;
    unsigned int m_residualCheckInterval;
    double m_tolerance;
    double m_lastResidual;
    double m_sorFactor;
    bool m_useRedBlackOrdering;
};

//! Shared pointer type for the FDMGaussSeidelSolver2.
using FDMGaussSeidelSolver2Ptr = std::shared_ptr<FDMGaussSeidelSolver2>;
}  // namespace CubbyFlow

#endif