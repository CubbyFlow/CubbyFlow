// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Solver/Advection/CubicSemiLagrangian.hpp>
#include <Core/Solver/Advection/CubicSemiLagrangian2.hpp>
#include <Core/Solver/Advection/CubicSemiLagrangian3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddCubicSemiLagrangian2(pybind11::module& m)
{
    pybind11::class_<CubicSemiLagrangian2, CubicSemiLagrangian2Ptr,
                     SemiLagrangian2>(static_cast<pybind11::handle>(m),
                                      "CubicSemiLagrangian2",
                                      R"pbdoc(
			Implementation of 2-D cubic semi-Lagrangian advection solver.

			This class implements 3rd-order cubic 2-D semi-Lagrangian advection solver.
		)pbdoc")
        .def(pybind11::init<>());
}

void AddCubicSemiLagrangian3(pybind11::module& m)
{
    pybind11::class_<CubicSemiLagrangian3, CubicSemiLagrangian3Ptr,
                     SemiLagrangian3>(static_cast<pybind11::handle>(m),
                                      "CubicSemiLagrangian3",
                                      R"pbdoc(
			Implementation of 3-D cubic semi-Lagrangian advection solver.

			This class implements 3rd-order cubic 3-D semi-Lagrangian advection solver.
		)pbdoc")
        .def(pybind11::init<>());
}