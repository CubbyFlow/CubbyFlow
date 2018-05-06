/*************************************************************************
> File Name: GridSinglePhasePressureSolver2.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 2-D single-phase pressure solver.
> Created Time: 2017/08/14
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_SINGLE_PHASE_PRESSURE_SOLVER2_H
#define CUBBYFLOW_SINGLE_PHASE_PRESSURE_SOLVER2_H

#include <Core/FDM/FDMMGLinearSystem2.h>
#include <Core/Solver/FDM/FDMLinearSystemSolver2.h>
#include <Core/Solver/FDM/FDMMGSolver2.h>
#include <Core/Solver/Grid/GridPressureSolver2.h>

namespace CubbyFlow
{
	//!
	//! \brief 2-D single-phase pressure solver.
	//!
	//! This class implements 2-D single-phase pressure solver. This solver encodes
	//! the boundaries like Lego blocks -- if a grid cell center is inside or
	//! outside the boundaries, it is either marked as occupied or not.
	//! In addition, this class solves single-phase flow, solving the pressure for
	//! selected fluid region only and treat other area as an atmosphere region.
	//! Thus, the pressure outside the fluid will be set to a constant value and
	//! velocity field won't be altered. This solver also computes the fluid
	//! boundary in block-like manner; If a grid cell is inside or outside the
	//! fluid, it is marked as either fluid or atmosphere. Thus, this solver in
	//! general, does not compute sub-grid structure.
	//!
	class GridSinglePhasePressureSolver2 : public GridPressureSolver2
	{
	public:
		//! Default constructor.
		GridSinglePhasePressureSolver2();

		//! Default destructor.
		virtual ~GridSinglePhasePressureSolver2();

		//!
		//! \brief Solves the pressure term and apply it to the velocity field.
		//!
		//! This function takes input velocity field and outputs pressure-applied
		//! velocity field. It also accepts extra arguments such as \p boundarySDF
		//! and \p fluidSDF that represent signed-distance representation of the
		//! boundary and fluid area. The negative region of \p boundarySDF means
		//! it is occupied by solid object. Also, the positive / negative area of
		//! the \p fluidSDF means it is occupied by fluid / atmosphere. If not
		//! specified, constant scalar field with std::numeric_limits<double>::max() will be used for
		//! \p boundarySDF meaning that no boundary at all. Similarly, a constant
		//! field with -std::numeric_limits<double>::max() will be used for \p fluidSDF
		//! which means it's fully occupied with fluid without any atmosphere.
		//!
		//! \param[in]    input                 The input velocity field.
		//! \param[in]    timeIntervalInSeconds The time interval for the sim.
		//! \param[inout] output                The output velocity field.
		//! \param[in]    boundarySDF           The SDF of the boundary.
		//! \param[in]    boundaryVelocity      The velocity of the boundary.
		//! \param[in]    fluidSDF              The SDF of the fluid/atmosphere.
		//! \param[in]    useCompressed         True if it uses compressed system.
		//!
		void Solve(
			const FaceCenteredGrid2& input,
			double timeIntervalInSeconds,
			FaceCenteredGrid2* output,
			const ScalarField2& boundarySDF = ConstantScalarField2(std::numeric_limits<double>::max()),
			const VectorField2& boundaryVelocity = ConstantVectorField2({ 0, 0 }),
			const ScalarField2& fluidSDF = ConstantScalarField2(-std::numeric_limits<double>::max()),
			bool useCompressed = false) override;

		//!
		//! \brief Returns the best boundary condition solver for this solver.
		//!
		//! This function returns the best boundary condition solver that works well
		//! with this pressure solver. Depending on the pressure solver
		//! implementation, different boundary condition solver might be used. For
		//! this particular class, an instance of GridBlockedBoundaryConditionSolver2
		//! will be returned since this pressure solver encodes boundaries
		//! like pixelated Lego blocks.
		//!
		GridBoundaryConditionSolver2Ptr SuggestedBoundaryConditionSolver() const override;

		//! Returns the linear system solver.
		const FDMLinearSystemSolver2Ptr& GetLinearSystemSolver() const;

		//! Sets the linear system solver.
		void SetLinearSystemSolver(const FDMLinearSystemSolver2Ptr& solver);

		//! Returns the pressure field.
		const FDMVector2& GetPressure() const;

	private:
		FDMLinearSystem2 m_system;
		FDMCompressedLinearSystem2 m_compSystem;
		FDMLinearSystemSolver2Ptr m_systemSolver;

		FDMMGLinearSystem2 m_mgSystem;
		FDMMGSolver2Ptr m_mgSystemSolver;

		std::vector<Array2<char>> m_markers;

		void BuildMarkers(
			const Size2& size,
			const std::function<Vector2D(size_t, size_t)>& pos,
			const ScalarField2& boundarySDF,
			const ScalarField2& fluidSDF);

		void DecompressSolution();

		virtual void BuildSystem(const FaceCenteredGrid2& input, bool useCompressed);

		virtual void ApplyPressureGradient(const FaceCenteredGrid2& input, FaceCenteredGrid2* output);
	};

	//! Shared pointer type for the GridSinglePhasePressureSolver2.
	using GridSinglePhasePressureSolver2Ptr = std::shared_ptr<GridSinglePhasePressureSolver2>;
}

#endif