/*************************************************************************
> File Name: AdvectionSolver3.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Abstract based class for 3-D grid-based advection solver.
> Created Time: 2017/08/07
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Solver/Advection/AdvectionSolver3.h>

namespace CubbyFlow
{
	AdvectionSolver3::AdvectionSolver3()
	{
		// Do nothing
	}

	AdvectionSolver3::~AdvectionSolver3()
	{
		// Do nothing
	}

	void AdvectionSolver3::Advect(
		const CollocatedVectorGrid3& source,
		const VectorField3& flow,
		double dt,
		CollocatedVectorGrid3* target,
		const ScalarField3& boundarySDF)
	{
		UNUSED_VARIABLE(source);
		UNUSED_VARIABLE(flow);
		UNUSED_VARIABLE(dt);
		UNUSED_VARIABLE(target);
		UNUSED_VARIABLE(boundarySDF);
	}

	void AdvectionSolver3::Advect(
		const FaceCenteredGrid3& source,
		const VectorField3& flow,
		double dt,
		FaceCenteredGrid3* target,
		const ScalarField3& boundarySDF)
	{
		UNUSED_VARIABLE(source);
		UNUSED_VARIABLE(flow);
		UNUSED_VARIABLE(dt);
		UNUSED_VARIABLE(target);
		UNUSED_VARIABLE(boundarySDF);
	}
}