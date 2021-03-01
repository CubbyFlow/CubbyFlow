// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Solver/Grid/GridBoundaryConditionSolver2.hpp>

namespace CubbyFlow
{
const Collider2Ptr& GridBoundaryConditionSolver2::GetCollider() const
{
    return m_collider;
}

void GridBoundaryConditionSolver2::UpdateCollider(
    const Collider2Ptr& newCollider, const Vector2UZ& gridSize,
    const Vector2D& gridSpacing, const Vector2D& gridOrigin)
{
    m_collider = newCollider;
    m_gridSize = gridSize;
    m_gridSpacing = gridSpacing;
    m_gridOrigin = gridOrigin;

    OnColliderUpdated(gridSize, gridSpacing, gridOrigin);
}

int GridBoundaryConditionSolver2::GetClosedDomainBoundaryFlag() const
{
    return m_closedDomainBoundaryFlag;
}

void GridBoundaryConditionSolver2::SetClosedDomainBoundaryFlag(int flag)
{
    m_closedDomainBoundaryFlag = flag;
}

const Vector2UZ& GridBoundaryConditionSolver2::GetGridSize() const
{
    return m_gridSize;
}

const Vector2D& GridBoundaryConditionSolver2::GetGridSpacing() const
{
    return m_gridSpacing;
}

const Vector2D& GridBoundaryConditionSolver2::GetGridOrigin() const
{
    return m_gridOrigin;
}
}  // namespace CubbyFlow