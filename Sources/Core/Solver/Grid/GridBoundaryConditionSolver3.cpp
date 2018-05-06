/*************************************************************************
> File Name: GridBoundaryConditionSolver3.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Abstract base class for 3-D boundary condition solver for grids.
> Created Time: 2017/08/09
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Solver/Grid/GridBoundaryConditionSolver3.h>

namespace CubbyFlow
{
	GridBoundaryConditionSolver3::GridBoundaryConditionSolver3()
	{
		// Do nothing
	}

	GridBoundaryConditionSolver3::~GridBoundaryConditionSolver3()
	{
		// Do nothing
	}

	const Collider3Ptr& GridBoundaryConditionSolver3::GetCollider() const
	{
		return m_collider;
	}

	void GridBoundaryConditionSolver3::UpdateCollider(
		const Collider3Ptr& newCollider,
		const Size3& gridSize,
		const Vector3D& gridSpacing,
		const Vector3D& gridOrigin)
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

	const Size3& GridBoundaryConditionSolver3::GetGridSize() const
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
}