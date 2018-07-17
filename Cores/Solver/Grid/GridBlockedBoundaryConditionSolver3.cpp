/*************************************************************************
> File Name: GridBlockedBoundaryConditionSolver3.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Blocked 3-D boundary condition solver for grids.
> Created Time: 2017/08/10
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <LevelSet/LevelSetUtils.h>
#include <Solver/Grid/GridBlockedBoundaryConditionSolver3.h>

namespace CubbyFlow
{
	static const char FLUID = 1;
	static const char COLLIDER = 0;

	GridBlockedBoundaryConditionSolver3::GridBlockedBoundaryConditionSolver3()
	{
		// Do nothing
	}

	void GridBlockedBoundaryConditionSolver3::ConstrainVelocity(
		FaceCenteredGrid3* velocity,
		unsigned int extrapolationDepth)
	{
		GridFractionalBoundaryConditionSolver3::ConstrainVelocity(velocity, extrapolationDepth);

		// No-flux: project the velocity at the marker interface
		Size3 size = velocity->Resolution();
		auto u = velocity->GetUAccessor();
		auto v = velocity->GetVAccessor();
		auto w = velocity->GetWAccessor();
		auto uPos = velocity->GetUPosition();
		auto vPos = velocity->GetVPosition();
		auto wPos = velocity->GetWPosition();

		m_marker.ForEachIndex([&](size_t i, size_t j, size_t k)
		{
			if (m_marker(i, j, k) == COLLIDER)
			{
				if (i > 0 && m_marker(i - 1, j, k) == FLUID)
				{
					Vector3D colliderVel = GetCollider()->VelocityAt(uPos(i, j, k));
					u(i, j, k) = colliderVel.x;
				}
				if (i < size.x - 1 && m_marker(i + 1, j, k) == FLUID)
				{
					Vector3D colliderVel = GetCollider()->VelocityAt(uPos(i + 1, j, k));
					u(i + 1, j, k) = colliderVel.x;
				}
				if (j > 0 && m_marker(i, j - 1, k) == FLUID)
				{
					Vector3D colliderVel = GetCollider()->VelocityAt(vPos(i, j, k));
					v(i, j, k) = colliderVel.y;
				}
				if (j < size.y - 1 && m_marker(i, j + 1, k) == FLUID)
				{
					Vector3D colliderVel = GetCollider()->VelocityAt(vPos(i, j + 1, k));
					v(i, j + 1, k) = colliderVel.y;
				}
				if (k > 0 && m_marker(i, j, k - 1) == FLUID)
				{
					Vector3D colliderVel = GetCollider()->VelocityAt(wPos(i, j, k));
					w(i, j, k) = colliderVel.z;
				}
				if (k < size.z - 1 && m_marker(i, j, k + 1) == FLUID)
				{
					Vector3D colliderVel = GetCollider()->VelocityAt(wPos(i, j, k + 1));
					w(i, j, k + 1) = colliderVel.z;
				}
			}
		});
	}

	const Array3<char>& GridBlockedBoundaryConditionSolver3::GetMarker() const
	{
		return m_marker;
	}

	void GridBlockedBoundaryConditionSolver3::OnColliderUpdated(
		const Size3& gridSize,
		const Vector3D& gridSpacing,
		const Vector3D& gridOrigin)
	{
		GridFractionalBoundaryConditionSolver3::OnColliderUpdated(gridSize, gridSpacing, gridOrigin);

		const auto sdf = std::dynamic_pointer_cast<CellCenteredScalarGrid3>(GetColliderSDF());

		m_marker.Resize(gridSize);
		m_marker.ParallelForEachIndex([&](size_t i, size_t j, size_t k)
		{
			if (IsInsideSDF((*sdf)(i, j, k)))
			{
				m_marker(i, j, k) = COLLIDER;
			}
			else
			{
				m_marker(i, j, k) = FLUID;
			}
		});
	}
}