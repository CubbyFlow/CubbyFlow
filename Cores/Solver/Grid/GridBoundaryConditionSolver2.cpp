/*************************************************************************
> File Name: GridBoundaryConditionSolver2.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for 2-D boundary condition solver for grids.
> Created Time: 2017/08/09
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Solver/Grid/GridBoundaryConditionSolver2.h>

namespace CubbyFlow
{
	GridBoundaryConditionSolver2::GridBoundaryConditionSolver2()
	{
		// Do nothing
	}

	GridBoundaryConditionSolver2::~GridBoundaryConditionSolver2()
	{
		// Do nothing
	}

	const Collider2Ptr& GridBoundaryConditionSolver2::GetCollider() const
	{
		return m_collider;
	}

	void GridBoundaryConditionSolver2::UpdateCollider(
		const Collider2Ptr& newCollider,
		const Size2& gridSize,
		const Vector2D& gridSpacing,
		const Vector2D& gridOrigin)
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

	const Size2& GridBoundaryConditionSolver2::GetGridSize() const
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
}