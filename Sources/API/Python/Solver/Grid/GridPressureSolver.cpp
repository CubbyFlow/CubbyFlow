// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Solver/Grid/GridPressureSolver.hpp>
#include <Core/Solver/Grid/GridPressureSolver2.hpp>
#include <Core/Solver/Grid/GridPressureSolver3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddGridPressureSolver2(pybind11::module& m)
{
    pybind11::class_<GridPressureSolver2, GridPressureSolver2Ptr>(
        static_cast<pybind11::handle>(m), "GridPressureSolver2",
        R"pbdoc(
			Abstract base class for 2-D grid-based pressure solver.

			This class represents a 2-D grid-based pressure solver interface which can
			be used as a sub-step of GridFluidSolver2.
		)pbdoc");
}

void AddGridPressureSolver3(pybind11::module& m)
{
    pybind11::class_<GridPressureSolver3, GridPressureSolver3Ptr>(
        static_cast<pybind11::handle>(m), "GridPressureSolver3",
        R"pbdoc(
			Abstract base class for 3-D grid-based pressure solver.

			This class represents a 3-D grid-based pressure solver interface which can
			be used as a sub-step of GridFluidSolver3.
		)pbdoc");
}