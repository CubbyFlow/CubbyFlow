/*************************************************************************
> File Name: AdvectionSolver2.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Abstract based class for 2-D grid-based advection solver.
> Created Time: 2017/08/07
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_ADVECTION_SOLVER2_H
#define CUBBYFLOW_ADVECTION_SOLVER2_H

#include <Core/Field/ConstantScalarField2.h>
#include <Core/Field/VectorField2.h>
#include <Core/Grid/CollocatedVectorGrid2.h>
#include <Core/Grid/FaceCenteredGrid2.h>
#include <Core/Grid/ScalarGrid2.h>

namespace CubbyFlow
{
	//!
	//! \brief Abstract based class for 2-D grid-based advection solver.
	//!
	//! The implementation of this abstract base class should solve 2-D advection
	//! equation for scalar and vector fields.
	//!
	class AdvectionSolver2
	{
	public:
		AdvectionSolver2();

		virtual ~AdvectionSolver2();

		//!
		//! \brief Solves advection equation for given scalar grid.
		//!
		//! The implementation of this virtual function should solve advection
		//! equation for given scalar field \p input and underlying vector field
		//! \p flow that carries the input field. The solution after solving the
		//! equation for given time-step \p dt should be stored in scalar field
		//! \p output. The boundary interface is given by a signed-distance field.
		//! The field is negative inside the boundary. By default, a constant field
		//! with max double value (std::numeric_limits<double>::max()) is used, meaning no boundary.
		//!
		//! \param input Input scalar grid.
		//! \param flow Vector field that advects the input field.
		//! \param dt Time-step for the advection.
		//! \param output Output scalar grid.
		//! \param boundarySDF Boundary interface defined by signed-distance
		//!     field.
		//!
		virtual void Advect(
			const ScalarGrid2& input,
			const VectorField2& flow,
			double dt,
			ScalarGrid2* output,
			const ScalarField2& boundarySDF = ConstantScalarField2(std::numeric_limits<double>::max())) = 0;

		//!
		//! \brief Solves advection equation for given collocated vector grid.
		//!
		//! The implementation of this virtual function should solve advection
		//! equation for given collocated vector grid \p input and underlying vector
		//! field \p flow that carries the input field. The solution after solving
		//! the equation for given time-step \p dt should be stored in vector field
		//! \p output. The boundary interface is given by a signed-distance field.
		//! The field is negative inside the boundary. By default, a constant field
		//! with max double value (std::numeric_limits<double>::max()) is used, meaning no boundary.
		//!
		//! \param input Input vector grid.
		//! \param flow Vector field that advects the input field.
		//! \param dt Time-step for the advection.
		//! \param output Output vector grid.
		//! \param boundarySDF Boundary interface defined by signed-distance
		//!     field.
		//!
		virtual void Advect(
			const CollocatedVectorGrid2& input,
			const VectorField2& flow,
			double dt,
			CollocatedVectorGrid2* output,
			const ScalarField2& boundarySDF = ConstantScalarField2(std::numeric_limits<double>::max()));

		//!
		//! \brief Solves advection equation for given face-centered vector grid.
		//!
		//! The implementation of this virtual function should solve advection
		//! equation for given face-centered vector field \p input and underlying
		//! vector field \p flow that carries the input field. The solution after
		//! solving the equation for given time-step \p dt should be stored in
		//! vector field \p output. The boundary interface is given by a
		//! signed-distance field. The field is negative inside the boundary. By
		//! default, a constant field with max double value (std::numeric_limits<double>::max()) is used, meaning
		//! no boundary.
		//!
		//! \param input Input vector grid.
		//! \param flow Vector field that advects the input field.
		//! \param dt Time-step for the advection.
		//! \param output Output vector grid.
		//! \param boundarySDF Boundary interface defined by signed-distance
		//!     field.
		//!
		virtual void Advect(
			const FaceCenteredGrid2& input,
			const VectorField2& flow,
			double dt,
			FaceCenteredGrid2* output,
			const ScalarField2& boundarySDF = ConstantScalarField2(std::numeric_limits<double>::max()));
	};

	//! Shared pointer type for the 2-D advection solver.
	using AdvectionSolver2Ptr = std::shared_ptr<AdvectionSolver2>;
}

#endif