// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_ENO_LEVEL_SET_SOLVER3_HPP
#define CUBBYFLOW_ENO_LEVEL_SET_SOLVER3_HPP

#include <Core/Solver/LevelSet/IterativeLevelSetSolver3.hpp>

namespace CubbyFlow
{
//! Three-dimensional third-order ENO-based iterative level set solver.
class ENOLevelSetSolver3 final : public IterativeLevelSetSolver3
{
 public:
    //! Default constructor.
    ENOLevelSetSolver3();

 protected:
    //! Computes the derivatives for given grid point.
    void GetDerivatives(ConstArrayAccessor3<double> grid,
                        const Vector3D& gridSpacing, size_t i, size_t j,
                        size_t k, std::array<double, 2>* dx,
                        std::array<double, 2>* dy,
                        std::array<double, 2>* dz) const override;
};

using ENOLevelSetSolver3Ptr = std::shared_ptr<ENOLevelSetSolver3>;
}  // namespace CubbyFlow

#endif