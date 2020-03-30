/*************************************************************************
> File Name: GridPressureSolver.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: GridPressureSolver functions for CubbyFlow Python API.
> Created Time: 2018/02/15
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Solver/Grid/GridPressureSolver.h>
#include <Core/Solver/Grid/GridPressureSolver2.hpp>
#include <Core/Solver/Grid/GridPressureSolver3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddGridPressureSolver2(pybind11::module& m)
{
	pybind11::class_<GridPressureSolver2, GridPressureSolver2Ptr>(static_cast<pybind11::handle>(m), "GridPressureSolver2",
		R"pbdoc(
			Abstract base class for 2-D grid-based pressure solver.

			This class represents a 2-D grid-based pressure solver interface which can
			be used as a sub-step of GridFluidSolver2.
		)pbdoc");
}

void AddGridPressureSolver3(pybind11::module& m)
{
	pybind11::class_<GridPressureSolver3, GridPressureSolver3Ptr>(static_cast<pybind11::handle>(m), "GridPressureSolver3",
		R"pbdoc(
			Abstract base class for 3-D grid-based pressure solver.

			This class represents a 3-D grid-based pressure solver interface which can
			be used as a sub-step of GridFluidSolver3.
		)pbdoc");
}