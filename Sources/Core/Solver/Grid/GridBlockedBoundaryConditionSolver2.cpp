// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Solver/Grid/GridBlockedBoundaryConditionSolver2.hpp>
#include <Core/Utils/LevelSetUtils.hpp>

namespace CubbyFlow
{
static const char FLUID = 1;
static const char COLLIDER = 0;

void GridBlockedBoundaryConditionSolver2::ConstrainVelocity(
    FaceCenteredGrid2* velocity, unsigned int extrapolationDepth)
{
    GridFractionalBoundaryConditionSolver2::ConstrainVelocity(
        velocity, extrapolationDepth);

    // No-flux: project the velocity at the marker interface
    Size2 size = velocity->Resolution();
    ArrayAccessor2<double> u = velocity->GetUAccessor();
    ArrayAccessor2<double> v = velocity->GetVAccessor();
    auto uPos = velocity->GetUPosition();
    auto vPos = velocity->GetVPosition();

    m_marker.ForEachIndex([&](size_t i, size_t j) {
        if (m_marker(i, j) == COLLIDER)
        {
            if (i > 0 && m_marker(i - 1, j) == FLUID)
            {
                const Vector2D colliderVel =
                    GetCollider()->VelocityAt(uPos(i, j));
                u(i, j) = colliderVel.x;
            }
            if (i < size.x - 1 && m_marker(i + 1, j) == FLUID)
            {
                const Vector2D colliderVel =
                    GetCollider()->VelocityAt(uPos(i + 1, j));
                u(i + 1, j) = colliderVel.x;
            }
            if (j > 0 && m_marker(i, j - 1) == FLUID)
            {
                const Vector2D colliderVel =
                    GetCollider()->VelocityAt(vPos(i, j));
                v(i, j) = colliderVel.y;
            }
            if (j < size.y - 1 && m_marker(i, j + 1) == FLUID)
            {
                const Vector2D colliderVel =
                    GetCollider()->VelocityAt(vPos(i, j + 1));
                v(i, j + 1) = colliderVel.y;
            }
        }
    });
}

const Array2<char>& GridBlockedBoundaryConditionSolver2::GetMarker() const
{
    return m_marker;
}

void GridBlockedBoundaryConditionSolver2::OnColliderUpdated(
    const Size2& gridSize, const Vector2D& gridSpacing,
    const Vector2D& gridOrigin)
{
    GridFractionalBoundaryConditionSolver2::OnColliderUpdated(
        gridSize, gridSpacing, gridOrigin);

    const auto sdf =
        std::dynamic_pointer_cast<CellCenteredScalarGrid2>(GetColliderSDF());

    m_marker.Resize(gridSize);
    m_marker.ParallelForEachIndex([&](size_t i, size_t j) {
        if (IsInsideSDF((*sdf)(i, j)))
        {
            m_marker(i, j) = COLLIDER;
        }
        else
        {
            m_marker(i, j) = FLUID;
        }
    });
}
}  // namespace CubbyFlow