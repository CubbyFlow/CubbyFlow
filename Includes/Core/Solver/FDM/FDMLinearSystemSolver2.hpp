// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_FDM_LINEAR_SYSTEM_SOLVER2_HPP
#define CUBBYFLOW_FDM_LINEAR_SYSTEM_SOLVER2_HPP

#include <Core/FDM/FDMLinearSystem2.hpp>

namespace CubbyFlow
{
//! Abstract base class for 2-D finite difference-type linear system solver.
class FDMLinearSystemSolver2
{
 public:
    virtual ~FDMLinearSystemSolver2() = default;

    //! Solves the given linear system.
    virtual bool Solve(FDMLinearSystem2* system) = 0;

    //! Solves the given compressed linear system.
    virtual bool SolveCompressed(FDMCompressedLinearSystem2*)
    {
        return false;
    }
};

//! Shared pointer type for the FDMLinearSystemSolver2.
using FDMLinearSystemSolver2Ptr = std::shared_ptr<FDMLinearSystemSolver2>;
}  // namespace CubbyFlow

#endif