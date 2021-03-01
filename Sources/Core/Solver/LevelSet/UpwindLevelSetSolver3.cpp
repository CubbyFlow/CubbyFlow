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
#include <Core/Solver/LevelSet/UpwindLevelSetSolver3.hpp>

namespace CubbyFlow
{
UpwindLevelSetSolver3::UpwindLevelSetSolver3()
{
    SetMaxCFL(0.5);
}

void UpwindLevelSetSolver3::GetDerivatives(ConstArrayView3<double> grid,
                                           const Vector3D& gridSpacing,
                                           size_t i, size_t j, size_t k,
                                           std::array<double, 2>* dx,
                                           std::array<double, 2>* dy,
                                           std::array<double, 2>* dz) const
{
    double d0[3];
    const Vector3UZ size = grid.Size();

    const size_t im1 = (i < 1) ? 0 : i - 1;
    const size_t ip1 = std::min(i + 1, size.x - 1);
    const size_t jm1 = (j < 1) ? 0 : j - 1;
    const size_t jp1 = std::min(j + 1, size.y - 1);
    const size_t km1 = (k < 1) ? 0 : k - 1;
    const size_t kp1 = std::min(k + 1, size.z - 1);

    d0[0] = grid(im1, j, k);
    d0[1] = grid(i, j, k);
    d0[2] = grid(ip1, j, k);
    *dx = Upwind1(d0, gridSpacing.x);

    d0[0] = grid(i, jm1, k);
    d0[1] = grid(i, j, k);
    d0[2] = grid(i, jp1, k);
    *dy = Upwind1(d0, gridSpacing.y);

    d0[0] = grid(i, j, km1);
    d0[1] = grid(i, j, k);
    d0[2] = grid(i, j, kp1);
    *dz = Upwind1(d0, gridSpacing.z);
}
}  // namespace CubbyFlow