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
#include <Core/Solver/LevelSet/ENOLevelSetSolver3.hpp>

namespace CubbyFlow
{
ENOLevelSetSolver3::ENOLevelSetSolver3()
{
    SetMaxCFL(0.25);
}

void ENOLevelSetSolver3::GetDerivatives(ConstArrayView3<double> grid,
                                        const Vector3D& gridSpacing, size_t i,
                                        size_t j, size_t k,
                                        std::array<double, 2>* dx,
                                        std::array<double, 2>* dy,
                                        std::array<double, 2>* dz) const
{
    double d0[7];
    const Vector3UZ size = grid.Size();

    const size_t im3 = (i < 3) ? 0 : i - 3;
    const size_t im2 = (i < 2) ? 0 : i - 2;
    const size_t im1 = (i < 1) ? 0 : i - 1;
    const size_t ip1 = std::min(i + 1, size.x - 1);
    const size_t ip2 = std::min(i + 2, size.x - 1);
    const size_t ip3 = std::min(i + 3, size.x - 1);
    const size_t jm3 = (j < 3) ? 0 : j - 3;
    const size_t jm2 = (j < 2) ? 0 : j - 2;
    const size_t jm1 = (j < 1) ? 0 : j - 1;
    const size_t jp1 = std::min(j + 1, size.y - 1);
    const size_t jp2 = std::min(j + 2, size.y - 1);
    const size_t jp3 = std::min(j + 3, size.y - 1);
    const size_t km3 = (k < 3) ? 0 : k - 3;
    const size_t km2 = (k < 2) ? 0 : k - 2;
    const size_t km1 = (k < 1) ? 0 : k - 1;
    const size_t kp1 = std::min(k + 1, size.z - 1);
    const size_t kp2 = std::min(k + 2, size.z - 1);
    const size_t kp3 = std::min(k + 3, size.z - 1);

    // 3rd-order ENO differencing
    d0[0] = grid(im3, j, k);
    d0[1] = grid(im2, j, k);
    d0[2] = grid(im1, j, k);
    d0[3] = grid(i, j, k);
    d0[4] = grid(ip1, j, k);
    d0[5] = grid(ip2, j, k);
    d0[6] = grid(ip3, j, k);
    *dx = ENO3(d0, gridSpacing.x);

    d0[0] = grid(i, jm3, k);
    d0[1] = grid(i, jm2, k);
    d0[2] = grid(i, jm1, k);
    d0[3] = grid(i, j, k);
    d0[4] = grid(i, jp1, k);
    d0[5] = grid(i, jp2, k);
    d0[6] = grid(i, jp3, k);
    *dy = ENO3(d0, gridSpacing.y);

    d0[0] = grid(i, j, km3);
    d0[1] = grid(i, j, km2);
    d0[2] = grid(i, j, km1);
    d0[3] = grid(i, j, k);
    d0[4] = grid(i, j, kp1);
    d0[5] = grid(i, j, kp2);
    d0[6] = grid(i, j, kp3);
    *dz = ENO3(d0, gridSpacing.z);
}
}  // namespace CubbyFlow