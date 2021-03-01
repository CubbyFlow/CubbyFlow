// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Solver/Grid/GridBoundaryConditionSolver3.hpp>

namespace CubbyFlow
{
const Collider3Ptr& GridBoundaryConditionSolver3::GetCollider() const
{
    return m_collider;
}

void GridBoundaryConditionSolver3::UpdateCollider(
    const Collider3Ptr& newCollider, const Vector3UZ& gridSize,
    const Vector3D& gridSpacing, const Vector3D& gridOrigin)
{
    m_collider = newCollider;
    m_gridSize = gridSize;
    m_gridSpacing = gridSpacing;
    m_gridOrigin = gridOrigin;

    OnColliderUpdated(gridSize, gridSpacing, gridOrigin);
}

int GridBoundaryConditionSolver3::GetClosedDomainBoundaryFlag() const
{
    return m_closedDomainBoundaryFlag;
}

void GridBoundaryConditionSolver3::SetClosedDomainBoundaryFlag(int flag)
{
    m_closedDomainBoundaryFlag = flag;
}

const Vector3UZ& GridBoundaryConditionSolver3::GetGridSize() const
{
    return m_gridSize;
}

const Vector3D& GridBoundaryConditionSolver3::GetGridSpacing() const
{
    return m_gridSpacing;
}

const Vector3D& GridBoundaryConditionSolver3::GetGridOrigin() const
{
    return m_gridOrigin;
}
}  // namespace CubbyFlow