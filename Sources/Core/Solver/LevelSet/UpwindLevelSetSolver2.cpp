// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Math/PDE.hpp>
#include <Core/Solver/LevelSet/UpwindLevelSetSolver2.hpp>

namespace CubbyFlow
{
UpwindLevelSetSolver2::UpwindLevelSetSolver2()
{
    SetMaxCFL(0.5);
}

void UpwindLevelSetSolver2::GetDerivatives(ConstArrayView2<double> grid,
                                           const Vector2D& gridSpacing,
                                           size_t i, size_t j,
                                           std::array<double, 2>* dx,
                                           std::array<double, 2>* dy) const
{
    double d0[3];
    const Vector2UZ size = grid.Size();

    const size_t im1 = (i < 1) ? 0 : i - 1;
    const size_t ip1 = std::min(i + 1, size.x - 1);
    const size_t jm1 = (j < 1) ? 0 : j - 1;
    const size_t jp1 = std::min(j + 1, size.y - 1);

    d0[0] = grid(im1, j);
    d0[1] = grid(i, j);
    d0[2] = grid(ip1, j);
    *dx = Upwind1(d0, gridSpacing.x);

    d0[0] = grid(i, jm1);
    d0[1] = grid(i, j);
    d0[2] = grid(i, jp1);
    *dy = Upwind1(d0, gridSpacing.y);
}
}  // namespace CubbyFlow