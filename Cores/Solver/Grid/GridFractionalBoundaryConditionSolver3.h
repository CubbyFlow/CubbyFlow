/*************************************************************************
> File Name: GridFractionalBoundaryConditionSolver3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Fractional 3-D boundary condition solver for grids.
> Created Time: 2017/08/09
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_GRID_FRACTIONAL_BOUNDARY_CONDITION_SOLVER3_H
#define CUBBYFLOW_GRID_FRACTIONAL_BOUNDARY_CONDITION_SOLVER3_H

#include <Field/CustomVectorField3.h>
#include <Grid/CellCenteredScalarGrid3.h>
#include <Solver/Grid/GridBoundaryConditionSolver3.h>

namespace CubbyFlow
{
	//!
	//! \brief Fractional 3-D boundary condition solver for grids.
	//!
	//! This class constrains the velocity field by projecting the flow to the
	//! signed-distance field representation of the collider. This implementation
	//! should pair up with GridFractionalSinglePhasePressureSolver3 to provide
	//! sub-grid resolution velocity projection.
	//!
	class GridFractionalBoundaryConditionSolver3 : public GridBoundaryConditionSolver3
	{
	public:
		//! Default constructor.
		GridFractionalBoundaryConditionSolver3();

		//! Default destructor.
		virtual ~GridFractionalBoundaryConditionSolver3();

		//!
		//! Constrains the velocity field to conform the collider boundary.
		//!
		//! \param velocity Input and output velocity grid.
		//! \param extrapolationDepth Number of inner-collider grid cells that
		//!     velocity will get extrapolated.
		//!
		void ConstrainVelocity(
			FaceCenteredGrid3* velocity,
			unsigned int extrapolationDepth = 5) override;

		//! Returns the signed distance field of the collider.
		ScalarField3Ptr GetColliderSDF() const override;

		//! Returns the velocity field of the collider.
		VectorField3Ptr GetColliderVelocityField() const override;

	protected:
		//! Invoked when a new collider is set.
		void OnColliderUpdated(
			const Size3& gridSize,
			const Vector3D& gridSpacing,
			const Vector3D& gridOrigin) override;

	private:
		CellCenteredScalarGrid3Ptr m_colliderSDF;
		CustomVectorField3Ptr m_colliderVel;
	};

	//! Shared pointer type for the GridFractionalBoundaryConditionSolver3.
	using GridFractionalBoundaryConditionSolver3Ptr = std::shared_ptr<GridFractionalBoundaryConditionSolver3>;
}

#endif