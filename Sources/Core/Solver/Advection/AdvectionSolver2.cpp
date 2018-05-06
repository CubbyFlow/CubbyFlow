/*************************************************************************
> File Name: AdvectionSolver2.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Abstract based class for 2-D grid-based advection solver.
> Created Time: 2017/08/07
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Solver/Advection/AdvectionSolver2.h>

namespace CubbyFlow
{
	AdvectionSolver2::AdvectionSolver2()
	{
		// Do nothing
	}

	AdvectionSolver2::~AdvectionSolver2()
	{
		// Do nothing
	}

	void AdvectionSolver2::Advect(
		const CollocatedVectorGrid2& source,
		const VectorField2& flow,
		double dt,
		CollocatedVectorGrid2* target,
		const ScalarField2& boundarySDF)
	{
		UNUSED_VARIABLE(source);
		UNUSED_VARIABLE(flow);
		UNUSED_VARIABLE(dt);
		UNUSED_VARIABLE(target);
		UNUSED_VARIABLE(boundarySDF);
	}

	void AdvectionSolver2::Advect(
		const FaceCenteredGrid2& source,
		const VectorField2& flow,
		double dt,
		FaceCenteredGrid2* target,
		const ScalarField2& boundarySDF)
	{
		UNUSED_VARIABLE(source);
		UNUSED_VARIABLE(flow);
		UNUSED_VARIABLE(dt);
		UNUSED_VARIABLE(target);
		UNUSED_VARIABLE(boundarySDF);
	}
}