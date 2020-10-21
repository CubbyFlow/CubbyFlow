// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_FDM_LINEAR_SYSTEM_SOLVER3_HPP
#define CUBBYFLOW_FDM_LINEAR_SYSTEM_SOLVER3_HPP

#include <Core/FDM/FDMLinearSystem3.hpp>

namespace CubbyFlow
{
//! Abstract base class for 3-D finite difference-type linear system solver.
class FDMLinearSystemSolver3
{
 public:
    //! Default constructor.
    FDMLinearSystemSolver3() = default;

    //! Deleted copy constructor.
    FDMLinearSystemSolver3(const FDMLinearSystemSolver3&) = delete;

    //! Deleted move constructor.
    FDMLinearSystemSolver3(FDMLinearSystemSolver3&&) noexcept = delete;

    //! Default virtual destructor.
    virtual ~FDMLinearSystemSolver3() = default;

    //! Deleted copy assignment operator.
    FDMLinearSystemSolver3& operator=(const FDMLinearSystemSolver3&) = delete;

    //! Deleted move assignment operator.
    FDMLinearSystemSolver3& operator=(FDMLinearSystemSolver3&&) noexcept =
        delete;

    //! Solves the given linear system.
    virtual bool Solve(FDMLinearSystem3* system) = 0;

    //! Solves the given compressed linear system.
    virtual bool SolveCompressed(FDMCompressedLinearSystem3*)
    {
        return false;
    }
};

//! Shared pointer type for the FDMLinearSystemSolver3.
using FDMLinearSystemSolver3Ptr = std::shared_ptr<FDMLinearSystemSolver3>;
}  // namespace CubbyFlow

#endif