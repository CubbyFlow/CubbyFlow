/*************************************************************************
> File Name: GridBackwardEulerDiffusionSolver3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D grid-based backward Euler diffusion solver.
> Created Time: 2017/08/11
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_GRID_BACKWARD_EULER_DIFFUSION_SOLVER3_H
#define CUBBYFLOW_GRID_BACKWARD_EULER_DIFFUSION_SOLVER3_H

#include <Solver/FDM/FDMLinearSystemSolver3.h>
#include <Solver/Grid/GridDiffusionSolver3.h>

namespace CubbyFlow
{
	//!
	//! \brief 3-D grid-based backward Euler diffusion solver.
	//!
	//! This class implements 3-D grid-based forward Euler diffusion solver using
	//! second-order central differencing spatially. Since the method is following
	//! the implicit time-integration (i.e. backward Euler), larger time interval or
	//! diffusion coefficient can be used without breaking the result. Note, higher
	//! values for those parameters will still impact the accuracy of the result.
	//! To solve the backward Euler method, a linear system solver is used and
	//! incomplete Cholesky conjugate gradient method is used by default.
	//!
	class GridBackwardEulerDiffusionSolver3 final : public GridDiffusionSolver3
	{
	public:
		enum class BoundaryType
		{
			Dirichlet,
			Neumann
		};

		//! Constructs the solver with given boundary type.
		explicit GridBackwardEulerDiffusionSolver3(BoundaryType boundaryType = BoundaryType::Neumann);

		//!
		//! Solves diffusion equation for a scalar field.
		//!
		//! \param source Input scalar field.
		//! \param diffusionCoefficient Amount of diffusion.
		//! \param timeIntervalInSeconds Small time-interval that diffusion occur.
		//! \param dest Output scalar field.
		//! \param boundarySDF Shape of the solid boundary that is empty by default.
		//! \param fluidSDF Shape of the fluid boundary that is full by default.
		//!
		void Solve(
			const ScalarGrid3& source,
			double diffusionCoefficient,
			double timeIntervalInSeconds,
			ScalarGrid3* dest,
			const ScalarField3& boundarySDF = ConstantScalarField3(std::numeric_limits<double>::max()),
			const ScalarField3& fluidSDF = ConstantScalarField3(-std::numeric_limits<double>::max())) override;

		//!
		//! Solves diffusion equation for a collocated vector field.
		//!
		//! \param source Input collocated vector field.
		//! \param diffusionCoefficient Amount of diffusion.
		//! \param timeIntervalInSeconds Small time-interval that diffusion occur.
		//! \param dest Output collocated vector field.
		//! \param boundarySDF Shape of the solid boundary that is empty by default.
		//! \param fluidSDF Shape of the fluid boundary that is full by default.
		//!
		void Solve(
			const CollocatedVectorGrid3& source,
			double diffusionCoefficient,
			double timeIntervalInSeconds,
			CollocatedVectorGrid3* dest,
			const ScalarField3& boundarySDF = ConstantScalarField3(std::numeric_limits<double>::max()),
			const ScalarField3& fluidSDF = ConstantScalarField3(-std::numeric_limits<double>::max())) override;

		//!
		//! Solves diffusion equation for a face-centered vector field.
		//!
		//! \param source Input face-centered vector field.
		//! \param diffusionCoefficient Amount of diffusion.
		//! \param timeIntervalInSeconds Small time-interval that diffusion occur.
		//! \param dest Output face-centered vector field.
		//! \param boundarySDF Shape of the solid boundary that is empty by default.
		//! \param fluidSDF Shape of the fluid boundary that is full by default.
		//!
		void Solve(
			const FaceCenteredGrid3& source,
			double diffusionCoefficient,
			double timeIntervalInSeconds,
			FaceCenteredGrid3* dest,
			const ScalarField3& boundarySDF = ConstantScalarField3(std::numeric_limits<double>::max()),
			const ScalarField3& fluidSDF = ConstantScalarField3(-std::numeric_limits<double>::max())) override;

		//! Sets the linear system solver for this diffusion solver.
		void SetLinearSystemSolver(const FDMLinearSystemSolver3Ptr& solver);

	private:
		BoundaryType m_boundaryType;
		FDMLinearSystem3 m_system;
		FDMLinearSystemSolver3Ptr m_systemSolver;
		Array3<char> m_markers;

		void BuildMarkers(
			const Size3& size,
			const std::function<Vector3D(size_t, size_t, size_t)>& pos,
			const ScalarField3& boundarySDF,
			const ScalarField3& fluidSDF);

		void BuildMatrix(
			const Size3& size,
			const Vector3D& c);

		void BuildVectors(
			const ConstArrayAccessor3<double>& f,
			const Vector3D& c);

		void BuildVectors(
			const ConstArrayAccessor3<Vector3D>& f,
			const Vector3D& c,
			size_t component);
	};

	//! Shared pointer type for the GridBackwardEulerDiffusionSolver3.
	using GridBackwardEulerDiffusionSolver3Ptr = std::shared_ptr<GridBackwardEulerDiffusionSolver3>;
}

#endif