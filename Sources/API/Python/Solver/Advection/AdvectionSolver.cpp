/*************************************************************************
> File Name: AdvectionSolver.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: AdvectionSolver functions for CubbyFlow Python API.
> Created Time: 2018/02/13
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Solver/Advection/AdvectionSolver.h>
#include <Core/Solver/Advection/AdvectionSolver2.h>
#include <Core/Solver/Advection/AdvectionSolver3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddAdvectionSolver2(pybind11::module& m)
{
	pybind11::class_<AdvectionSolver2, AdvectionSolver2Ptr>(static_cast<pybind11::handle>(m), "AdvectionSolver2",
		R"pbdoc(
			Abstract based class for 2-D grid-based advection solver.

			The implementation of this abstract base class should solve 2-D advection
			equation for scalar and vector fields.
		)pbdoc");
}

void AddAdvectionSolver3(pybind11::module& m)
{
	pybind11::class_<AdvectionSolver3, AdvectionSolver3Ptr>(static_cast<pybind11::handle>(m), "AdvectionSolver3",
		R"pbdoc(
			Abstract based class for 3-D grid-based advection solver.

			The implementation of this abstract base class should solve 3-D advection
			equation for scalar and vector fields.
		)pbdoc");
}