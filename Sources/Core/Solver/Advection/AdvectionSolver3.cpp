// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Solver/Advection/AdvectionSolver3.hpp>

namespace CubbyFlow
{
void AdvectionSolver3::Advect(const CollocatedVectorGrid3& input,
                              const VectorField3& flow, double dt,
                              CollocatedVectorGrid3* output,
                              const ScalarField3& boundarySDF)
{
    UNUSED_VARIABLE(input);
    UNUSED_VARIABLE(flow);
    UNUSED_VARIABLE(dt);
    UNUSED_VARIABLE(output);
    UNUSED_VARIABLE(boundarySDF);
}

void AdvectionSolver3::Advect(const FaceCenteredGrid3& input,
                              const VectorField3& flow, double dt,
                              FaceCenteredGrid3* output,
                              const ScalarField3& boundarySDF)
{
    UNUSED_VARIABLE(input);
    UNUSED_VARIABLE(flow);
    UNUSED_VARIABLE(dt);
    UNUSED_VARIABLE(output);
    UNUSED_VARIABLE(boundarySDF);
}
}  // namespace CubbyFlow