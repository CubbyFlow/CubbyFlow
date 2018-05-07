/*************************************************************************
> File Name: GridSinglePhasePressureSolver.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: GridSinglePhasePressureSolver functions for CubbyFlow Python API.
> Created Time: 2018/02/15
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Solver/Grid/GridSinglePhasePressureSolver.h>
#include <Core/Solver/Grid/GridSinglePhasePressureSolver2.h>
#include <Core/Solver/Grid/GridSinglePhasePressureSolver3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddGridSinglePhasePressureSolver2(pybind11::module& m)
{
	pybind11::class_<GridSinglePhasePressureSolver2, GridSinglePhasePressureSolver2Ptr, GridPressureSolver2>(static_cast<pybind11::handle>(m), "GridSinglePhasePressureSolver2",
		R"pbdoc(
			2-D single-phase pressure solver.

			This class implements 2-D single-phase pressure solver. This solver encodes
			the boundaries like Lego blocks -- if a grid cell center is inside or
			outside the boundaries, it is either marked as occupied or not.
			In addition, this class solves single-phase flow, solving the pressure for
			selected fluid region only and treat other area as an atmosphere region.
			Thus, the pressure outside the fluid will be set to a constant value and
			velocity field won't be altered. This solver also computes the fluid
			boundary in block-like manner; If a grid cell is inside or outside the
			fluid, it is marked as either fluid or atmosphere. Thus, this solver in
			general, does not compute subgrid structure.
		)pbdoc")
	.def(pybind11::init<>())
	.def_property("linearSystemSolver", &GridSinglePhasePressureSolver2::GetLinearSystemSolver, &GridSinglePhasePressureSolver2::SetLinearSystemSolver,
		R"pbdoc(
			"The linear system solver."
		)pbdoc");
}

void AddGridSinglePhasePressureSolver3(pybind11::module& m)
{
	pybind11::class_<GridSinglePhasePressureSolver3, GridSinglePhasePressureSolver3Ptr, GridPressureSolver3>(static_cast<pybind11::handle>(m), "GridSinglePhasePressureSolver3",
		R"pbdoc(
			3-D single-phase pressure solver.

			This class implements 3-D single-phase pressure solver. This solver encodes
			the boundaries like Lego blocks -- if a grid cell center is inside or
			outside the boundaries, it is either marked as occupied or not.	
			In addition, this class solves single-phase flow, solving the pressure for
			selected fluid region only and treat other area as an atmosphere region.
			Thus, the pressure outside the fluid will be set to a constant value and
			velocity field won't be altered. This solver also computes the fluid
			boundary in block-like manner; If a grid cell is inside or outside the
			fluid, it is marked as either fluid or atmosphere. Thus, this solver in
			general, does not compute subgrid structure.
		)pbdoc")
	.def(pybind11::init<>())
	.def_property("linearSystemSolver", &GridSinglePhasePressureSolver3::GetLinearSystemSolver, &GridSinglePhasePressureSolver3::SetLinearSystemSolver,
		R"pbdoc(
			"The linear system solver."
		)pbdoc");
}