/*************************************************************************
> File Name: GridFractionalSinglePhasePressureSolver3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D fractional single-phase pressure solver.
> Created Time: 2017/08/14
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FRACTIONAL_SINGLE_PHASE_PRESSURE_SOLVER3_H
#define CUBBYFLOW_FRACTIONAL_SINGLE_PHASE_PRESSURE_SOLVER3_H

#include <FDM/FDMMGLinearSystem3.h>
#include <Solver/FDM/FDMLinearSystemSolver3.h>
#include <Solver/FDM/FDMMGSolver3.h>
#include <Solver/Grid/GridPressureSolver3.h>

namespace CubbyFlow
{
	//!
	//! \brief 3-D fractional single-phase pressure solver.
	//!
	//! This class implements 3-D fractional (or variational) single-phase pressure
	//! solver. It is called fractional because the solver encodes the boundaries
	//! to the grid cells like anti-aliased pixels, meaning that a grid cell will
	//! record the partially overlapping boundary as a fractional number.
	//! Alternative approach is to represent boundaries like Lego blocks which is
	//! the case for GridSinglePhasePressureSolver2.
	//! In addition, this class solves single-phase flow, solving the pressure for
	//! selected fluid region only and treat other area as an atmosphere region.
	//! Thus, the pressure outside the fluid will be set to a constant value and
	//! velocity field won't be altered. This solver also computes the fluid
	//! boundary in fractional manner, meaning that the solver tries to capture the
	//! sub-grid structures. This class uses ghost fluid method for such calculation.
	//!
	//! \see Batty, Christopher, Florence Bertails, and Robert Bridson.
	//!     "A fast variational framework for accurate solid-fluid coupling."
	//!     ACM Transactions on Graphics (TOG). Vol. 26. No. 3. ACM, 2007.
	//! \see Enright, Doug, et al. "Using the particle level set method and
	//!     a second order accurate pressure boundary condition for free surface
	//!     flows." ASME/JSME 2003 4th Joint Fluids Summer Engineering Conference.
	//!     American Society of Mechanical Engineers, 2003.
	//!
	class GridFractionalSinglePhasePressureSolver3 : public GridPressureSolver3
	{
	public:
		GridFractionalSinglePhasePressureSolver3();

		virtual ~GridFractionalSinglePhasePressureSolver3();

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
			const FaceCenteredGrid3& input,
			double timeIntervalInSeconds,
			FaceCenteredGrid3* output,
			const ScalarField3& boundarySDF = ConstantScalarField3(std::numeric_limits<double>::max()),
			const VectorField3& boundaryVelocity = ConstantVectorField3({ 0, 0, 0 }),
			const ScalarField3& fluidSDF = ConstantScalarField3(-std::numeric_limits<double>::max()),
			bool useCompressed = false) override;

		//!
		//! \brief Returns the best boundary condition solver for this solver.
		//!
		//! This function returns the best boundary condition solver that works well
		//! with this pressure solver. Depending on the pressure solver
		//! implementation, different boundary condition solver might be used. For
		//! this particular class, an instance of GridFractionalBoundaryConditionSolver3
		//! will be returned.
		//!
		GridBoundaryConditionSolver3Ptr SuggestedBoundaryConditionSolver() const override;

		//! Returns the linear system solver.
		const FDMLinearSystemSolver3Ptr& GetLinearSystemSolver() const;

		//! Sets the linear system solver.
		void SetLinearSystemSolver(const FDMLinearSystemSolver3Ptr& solver);

		//! Returns the pressure field.
		const FDMVector3& GetPressure() const;

	private:
		FDMLinearSystem3 m_system;
		FDMCompressedLinearSystem3 m_compSystem;
		FDMLinearSystemSolver3Ptr m_systemSolver;

		FDMMGLinearSystem3 m_mgSystem;
		FDMMGSolver3Ptr m_mgSystemSolver;

		std::vector<Array3<float>> m_uWeights;
		std::vector<Array3<float>> m_vWeights;
		std::vector<Array3<float>> m_wWeights;
		std::vector<Array3<float>> m_fluidSDF;

		std::function<Vector3D(const Vector3D&)> m_boundaryVel;

		void BuildWeights(
			const FaceCenteredGrid3& input,
			const ScalarField3& boundarySDF,
			const VectorField3& boundaryVelocity,
			const ScalarField3& fluidSDF);

		void DecompressSolution();

		virtual void BuildSystem(const FaceCenteredGrid3& input, bool useCompressed);

		virtual void ApplyPressureGradient(const FaceCenteredGrid3& input, FaceCenteredGrid3* output);
	};

	//! Shared pointer type for the GridFractionalSinglePhasePressureSolver3.
	using GridFractionalSinglePhasePressureSolver3Ptr = std::shared_ptr<GridFractionalSinglePhasePressureSolver3>;
}

#endif