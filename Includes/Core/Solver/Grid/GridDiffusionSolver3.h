/*************************************************************************
> File Name: GridDiffusionSolver3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for 3-D grid-based diffusion equation solver.
> Created Time: 2017/08/10
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_GRID_DIFFUSION_SOLVER3_H
#define CUBBYFLOW_GRID_DIFFUSION_SOLVER3_H

#include <Core/Field/ConstantScalarField3.hpp>
#include <Core/Grid/CollocatedVectorGrid3.h>
#include <Core/Grid/FaceCenteredGrid3.h>
#include <Core/Grid/ScalarGrid3.h>

namespace CubbyFlow
{
	//!
	//! \brief Abstract base class for 3-D grid-based diffusion equation solver.
	//!
	//! This class provides functions to solve the diffusion equation for different
	//! types of fields. The target equation can be written as
	//! \f$\frac{\partial f}{\partial t} = \mu\nabla^2 f\f$ where \f$\mu\f$ is
	//! the diffusion coefficient. The field \f$f\f$ can be either scalar or vector
	//! field.
	//!
	class GridDiffusionSolver3
	{
	public:
		//! Default constructor.
		GridDiffusionSolver3();

		//! Default destructor.
		virtual ~GridDiffusionSolver3();

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
		virtual void Solve(
			const ScalarGrid3& source,
			double diffusionCoefficient,
			double timeIntervalInSeconds,
			ScalarGrid3* dest,
			const ScalarField3& boundarySDF = ConstantScalarField3(std::numeric_limits<double>::max()),
			const ScalarField3& fluidSDF = ConstantScalarField3(-std::numeric_limits<double>::max())) = 0;

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
		virtual void Solve(
			const CollocatedVectorGrid3& source,
			double diffusionCoefficient,
			double timeIntervalInSeconds,
			CollocatedVectorGrid3* dest,
			const ScalarField3& boundarySDF = ConstantScalarField3(std::numeric_limits<double>::max()),
			const ScalarField3& fluidSDF = ConstantScalarField3(-std::numeric_limits<double>::max())) = 0;

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
		virtual void Solve(
			const FaceCenteredGrid3& source,
			double diffusionCoefficient,
			double timeIntervalInSeconds,
			FaceCenteredGrid3* dest,
			const ScalarField3& boundarySDF = ConstantScalarField3(std::numeric_limits<double>::max()),
			const ScalarField3& fluidSDF = ConstantScalarField3(-std::numeric_limits<double>::max())) = 0;
	};

	//! Shared pointer type for the GridDiffusionSolver3.
	using GridDiffusionSolver3Ptr = std::shared_ptr<GridDiffusionSolver3>;
}

#endif