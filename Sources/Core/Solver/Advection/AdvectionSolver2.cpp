// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Solver/Advection/AdvectionSolver2.hpp>

namespace CubbyFlow
{
AdvectionSolver2::AdvectionSolver2()
{
    // Do nothing
}

AdvectionSolver2::~AdvectionSolver2()
{
    // Do nothing
}

void AdvectionSolver2::Advect(const CollocatedVectorGrid2& source,
                              const VectorField2& flow, double dt,
                              CollocatedVectorGrid2* target,
                              const ScalarField2& boundarySDF)
{
    UNUSED_VARIABLE(source);
    UNUSED_VARIABLE(flow);
    UNUSED_VARIABLE(dt);
    UNUSED_VARIABLE(target);
    UNUSED_VARIABLE(boundarySDF);
}

void AdvectionSolver2::Advect(const FaceCenteredGrid2& source,
                              const VectorField2& flow, double dt,
                              FaceCenteredGrid2* target,
                              const ScalarField2& boundarySDF)
{
    UNUSED_VARIABLE(source);
    UNUSED_VARIABLE(flow);
    UNUSED_VARIABLE(dt);
    UNUSED_VARIABLE(target);
    UNUSED_VARIABLE(boundarySDF);
}
}  // namespace CubbyFlow