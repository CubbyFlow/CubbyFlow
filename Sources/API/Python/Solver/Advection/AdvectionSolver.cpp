// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Solver/Advection/AdvectionSolver.hpp>
#include <Core/Solver/Advection/AdvectionSolver2.hpp>
#include <Core/Solver/Advection/AdvectionSolver3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddAdvectionSolver2(pybind11::module& m)
{
    pybind11::class_<AdvectionSolver2, AdvectionSolver2Ptr>(
        static_cast<pybind11::handle>(m), "AdvectionSolver2",
        R"pbdoc(
			Abstract based class for 2-D grid-based advection solver.

			The implementation of this abstract base class should solve 2-D advection
			equation for scalar and vector fields.
		)pbdoc");
}

void AddAdvectionSolver3(pybind11::module& m)
{
    pybind11::class_<AdvectionSolver3, AdvectionSolver3Ptr>(
        static_cast<pybind11::handle>(m), "AdvectionSolver3",
        R"pbdoc(
			Abstract based class for 3-D grid-based advection solver.

			The implementation of this abstract base class should solve 3-D advection
			equation for scalar and vector fields.
		)pbdoc");
}