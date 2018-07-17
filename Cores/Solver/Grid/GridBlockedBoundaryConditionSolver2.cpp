/*************************************************************************
> File Name: GridBlockedBoundaryConditionSolver2.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Blocked 2-D boundary condition solver for grids.
> Created Time: 2017/08/10
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <LevelSet/LevelSetUtils.h>
#include <Solver/Grid/GridBlockedBoundaryConditionSolver2.h>

namespace CubbyFlow
{
	static const char FLUID = 1;
	static const char COLLIDER = 0;

	GridBlockedBoundaryConditionSolver2::GridBlockedBoundaryConditionSolver2()
	{
		// Do nothing
	}

	void GridBlockedBoundaryConditionSolver2::ConstrainVelocity(
		FaceCenteredGrid2* velocity,
		unsigned int extrapolationDepth)
	{
		GridFractionalBoundaryConditionSolver2::ConstrainVelocity(velocity, extrapolationDepth);

		// No-flux: project the velocity at the marker interface
		Size2 size = velocity->Resolution();
		auto u = velocity->GetUAccessor();
		auto v = velocity->GetVAccessor();
		auto uPos = velocity->GetUPosition();
		auto vPos = velocity->GetVPosition();

		m_marker.ForEachIndex([&](size_t i, size_t j)
		{
			if (m_marker(i, j) == COLLIDER)
			{
				if (i > 0 && m_marker(i - 1, j) == FLUID)
				{
					Vector2D colliderVel = GetCollider()->VelocityAt(uPos(i, j));
					u(i, j) = colliderVel.x;
				}
				if (i < size.x - 1 && m_marker(i + 1, j) == FLUID)
				{
					Vector2D colliderVel = GetCollider()->VelocityAt(uPos(i + 1, j));
					u(i + 1, j) = colliderVel.x;
				}
				if (j > 0 && m_marker(i, j - 1) == FLUID)
				{
					Vector2D colliderVel = GetCollider()->VelocityAt(vPos(i, j));
					v(i, j) = colliderVel.y;
				}
				if (j < size.y - 1 && m_marker(i, j + 1) == FLUID)
				{
					Vector2D colliderVel = GetCollider()->VelocityAt(vPos(i, j + 1));
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
		const Size2& gridSize,
		const Vector2D& gridSpacing,
		const Vector2D& gridOrigin)
	{
		GridFractionalBoundaryConditionSolver2::OnColliderUpdated(gridSize, gridSpacing, gridOrigin);

		const auto sdf = std::dynamic_pointer_cast<CellCenteredScalarGrid2>(GetColliderSDF());

		m_marker.Resize(gridSize);
		m_marker.ParallelForEachIndex([&](size_t i, size_t j)
		{
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
}