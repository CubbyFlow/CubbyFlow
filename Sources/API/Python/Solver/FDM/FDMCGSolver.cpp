// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Solver/FDM/FDMCGSolver.hpp>
#include <Core/Solver/FDM/FDMCGSolver2.hpp>
#include <Core/Solver/FDM/FDMCGSolver3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddFDMCGSolver2(pybind11::module& m)
{
    pybind11::class_<FDMCGSolver2, FDMCGSolver2Ptr, FDMLinearSystemSolver2>(
        m, "FDMCGSolver2",
        R"pbdoc(
			2-D finite difference-type linear system solver using conjugate gradient.
		)pbdoc")
        .def(pybind11::init<uint32_t, double>(),
             pybind11::arg("maxNumberOfIterations"), pybind11::arg("tolerance"))
        .def_property_readonly("maxNumberOfIterations",
                               &FDMCGSolver2::GetMaxNumberOfIterations,
                               R"pbdoc(
			Max number of CG iterations.
		)pbdoc")
        .def_property_readonly("lastNumberOfIterations",
                               &FDMCGSolver2::GetLastNumberOfIterations,
                               R"pbdoc(
			The last number of CG iterations the solver made.
		)pbdoc")
        .def_property_readonly("tolerance", &FDMCGSolver2::GetTolerance,
                               R"pbdoc(
			The max residual tolerance for the CG method.
		)pbdoc")
        .def_property_readonly("lastResidual", &FDMCGSolver2::GetLastResidual,
                               R"pbdoc(
			The last residual after the CG iterations.
		)pbdoc");
}

void AddFDMCGSolver3(pybind11::module& m)
{
    pybind11::class_<FDMCGSolver3, FDMCGSolver3Ptr, FDMLinearSystemSolver3>(
        m, "FDMCGSolver3",
        R"pbdoc(
			3-D finite difference-type linear system solver using conjugate gradient.
		)pbdoc")
        .def(pybind11::init<uint32_t, double>(),
             pybind11::arg("maxNumberOfIterations"), pybind11::arg("tolerance"))
        .def_property_readonly("maxNumberOfIterations",
                               &FDMCGSolver3::GetMaxNumberOfIterations,
                               R"pbdoc(
			Max number of CG iterations.
		)pbdoc")
        .def_property_readonly("lastNumberOfIterations",
                               &FDMCGSolver3::GetLastNumberOfIterations,
                               R"pbdoc(
			The last number of CG iterations the solver made.
		)pbdoc")
        .def_property_readonly("tolerance", &FDMCGSolver3::GetTolerance,
                               R"pbdoc(
			The max residual tolerance for the CG method.
		)pbdoc")
        .def_property_readonly("lastResidual", &FDMCGSolver3::GetLastResidual,
                               R"pbdoc(
			The last residual after the CG iterations.
		)pbdoc");
}