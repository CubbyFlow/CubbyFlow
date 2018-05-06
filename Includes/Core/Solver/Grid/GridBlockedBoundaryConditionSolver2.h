/*************************************************************************
> File Name: GridBlockedBoundaryConditionSolver2.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Blocked 2-D boundary condition solver for grids.
> Created Time: 2017/08/10
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_GRID_BLOCKED_BOUNDARY_CONDITION_SOLVER2_H
#define CUBBYFLOW_GRID_BLOCKED_BOUNDARY_CONDITION_SOLVER2_H

#include <Core/Solver/Grid/GridFractionalBoundaryConditionSolver2.h>

namespace CubbyFlow
{
	//!
	//! \brief Blocked 2-D boundary condition solver for grids.
	//!
	//! This class constrains the velocity field by projecting the flow to the
	//! blocked representation of the collider. A collider is rasterized into voxels
	//! and each face of the collider voxels projects the velocity field onto its
	//! face. This implementation should pair up with GridSinglePhasePressureSolver3
	//! since the pressure solver assumes blocked boundary representation as well.
	//!
	class GridBlockedBoundaryConditionSolver2 final : public GridFractionalBoundaryConditionSolver2
	{
	public:
		//! Default constructor.
		GridBlockedBoundaryConditionSolver2();

		//!
		//! Constrains the velocity field to conform the collider boundary.
		//!
		//! \param velocity Input and output velocity grid.
		//! \param extrapolationDepth Number of inner-collider grid cells that
		//!     velocity will get extrapolated.
		//!
		void ConstrainVelocity(
			FaceCenteredGrid2* velocity,
			unsigned int extrapolationDepth = 5) override;

		//! Returns the marker which is 1 if occupied by the collider.
		const Array2<char>& GetMarker() const;

	protected:
		//! Invoked when a new collider is set.
		void OnColliderUpdated(
			const Size2& gridSize,
			const Vector2D& gridSpacing,
			const Vector2D& gridOrigin) override;

	private:
		Array2<char> m_marker;
	};

	//! Shared pointer type for the GridBlockedBoundaryConditionSolver2.
	using GridBlockedBoundaryConditionSolver2Ptr = std::shared_ptr<GridBlockedBoundaryConditionSolver2>;
}

#endif