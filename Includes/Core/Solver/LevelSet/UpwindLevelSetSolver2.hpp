// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_UPWIND_LEVEL_SET_SOLVER2_HPP
#define CUBBYFLOW_UPWIND_LEVEL_SET_SOLVER2_HPP

#include <Core/Solver/LevelSet/IterativeLevelSetSolver2.hpp>

namespace CubbyFlow
{
class UpwindLevelSetSolver2 final : public IterativeLevelSetSolver2
{
 public:
    //! Default constructor.
    UpwindLevelSetSolver2();

 protected:
    //! Computes the derivatives for given grid point.
    void GetDerivatives(ConstArrayView2<double> grid,
                        const Vector2D& gridSpacing, size_t i, size_t j,
                        std::array<double, 2>* dx,
                        std::array<double, 2>* dy) const override;
};

using UpwindLevelSetSolver2Ptr = std::shared_ptr<UpwindLevelSetSolver2>;
}  // namespace CubbyFlow

#endif