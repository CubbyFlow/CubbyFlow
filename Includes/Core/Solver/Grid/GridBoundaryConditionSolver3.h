/*************************************************************************
> File Name: GridBoundaryConditionSolver3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for 3-D boundary condition solver for grids.
> Created Time: 2017/08/09
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_GRID_BOUNDARY_CONDITION_SOLVER3_H
#define CUBBYFLOW_GRID_BOUNDARY_CONDITION_SOLVER3_H

#include <Core/Collider/Collider3.hpp>
#include <Core/Field/ScalarField3.hpp>
#include <Core/Grid/FaceCenteredGrid3.hpp>
#include <Core/Size/Size3.h>

namespace CubbyFlow
{
	//!
	//! \brief Abstract base class for 3-D boundary condition solver for grids.
	//!
	//! This is a helper class to constrain the 3-D velocity field with given
	//! collider object. It also determines whether to open any domain boundaries.
	//! To control the friction level, tune the collider parameter.
	//!
	class GridBoundaryConditionSolver3
	{
	public:
		//! Default constructor.
		GridBoundaryConditionSolver3();

		//! Default destructor.
		virtual ~GridBoundaryConditionSolver3();

		//! Returns associated collider.
		const Collider3Ptr& GetCollider() const;

		//!
		//! \brief Applies new collider and build the internals.
		//!
		//! This function is called to apply new collider and build the internal
		//! cache. To provide a hint to the cache, info for the expected velocity
		//! grid that will be constrained is provided.
		//!
		//! \param newCollider New collider to apply.
		//! \param gridSize Size of the velocity grid to be constrained.
		//! \param gridSpacing Grid spacing of the velocity grid to be constrained.
		//! \param gridOrigin Origin of the velocity grid to be constrained.
		//!
		void UpdateCollider(
			const Collider3Ptr& newCollider,
			const Size3& gridSize,
			const Vector3D& gridSpacing,
			const Vector3D& gridOrigin);

		//! Returns the closed domain boundary flag.
		int GetClosedDomainBoundaryFlag() const;

		//! Sets the closed domain boundary flag.
		void SetClosedDomainBoundaryFlag(int flag);

		//!
		//! Constrains the velocity field to conform the collider boundary.
		//!
		//! \param velocity Input and output velocity grid.
		//! \param extrapolationDepth Number of inner-collider grid cells that
		//!     velocity will get extrapolated.
		//!
		virtual void ConstrainVelocity(
			FaceCenteredGrid3* velocity,
			unsigned int extrapolationDepth = 5) = 0;

		//! Returns the signed distance field of the collider.
		virtual ScalarField3Ptr GetColliderSDF() const = 0;

		//! Returns the velocity field of the collider.
		virtual VectorField3Ptr GetColliderVelocityField() const = 0;

	protected:
		//! Invoked when a new collider is set.
		virtual void OnColliderUpdated(
			const Size3& gridSize,
			const Vector3D& gridSpacing,
			const Vector3D& gridOrigin) = 0;

		//! Returns the size of the velocity grid to be constrained.
		const Size3& GetGridSize() const;

		//! Returns the spacing of the velocity grid to be constrained.
		const Vector3D& GetGridSpacing() const;

		//! Returns the origin of the velocity grid to be constrained.
		const Vector3D& GetGridOrigin() const;

	private:
		Collider3Ptr m_collider;
		Size3 m_gridSize;
		Vector3D m_gridSpacing;
		Vector3D m_gridOrigin;
		int m_closedDomainBoundaryFlag = DIRECTION_ALL;
	};

	//! Shared pointer type for the GridBoundaryConditionSolver3.
	using GridBoundaryConditionSolver3Ptr = std::shared_ptr<GridBoundaryConditionSolver3>;
}

#endif