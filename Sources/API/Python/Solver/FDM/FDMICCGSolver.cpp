// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Solver/FDM/FDMICCGSolver.hpp>
#include <Core/Solver/FDM/FDMICCGSolver2.hpp>
#include <Core/Solver/FDM/FDMICCGSolver3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddFDMICCGSolver2(pybind11::module& m)
{
    pybind11::class_<FDMICCGSolver2, FDMICCGSolver2Ptr, FDMLinearSystemSolver2>(
        m, "FDMICCGSolver2",
        R"pbdoc(
			2-D finite difference-type linear system solver using conjugate gradient.
		)pbdoc")
        .def(pybind11::init<uint32_t, double>(),
             pybind11::arg("maxNumberOfIterations"), pybind11::arg("tolerance"))
        .def_property_readonly("maxNumberOfIterations",
                               &FDMICCGSolver2::GetMaxNumberOfIterations,
                               R"pbdoc(
			Max number of ICCG iterations.
		)pbdoc")
        .def_property_readonly("lastNumberOfIterations",
                               &FDMICCGSolver2::GetLastNumberOfIterations,
                               R"pbdoc(
			The last number of ICCG iterations the solver made.
		)pbdoc")
        .def_property_readonly("tolerance", &FDMICCGSolver2::GetTolerance,
                               R"pbdoc(
			The max residual tolerance for the ICCG method.
		)pbdoc")
        .def_property_readonly("lastResidual", &FDMICCGSolver2::GetLastResidual,
                               R"pbdoc(
			The last residual after the ICCG iterations.
		)pbdoc");
}

void AddFDMICCGSolver3(pybind11::module& m)
{
    pybind11::class_<FDMICCGSolver3, FDMICCGSolver3Ptr, FDMLinearSystemSolver3>(
        m, "FDMICCGSolver3",
        R"pbdoc(
			3-D finite difference-type linear system solver using conjugate gradient.
		)pbdoc")
        .def(pybind11::init<uint32_t, double>(),
             pybind11::arg("maxNumberOfIterations"), pybind11::arg("tolerance"))
        .def_property_readonly("maxNumberOfIterations",
                               &FDMICCGSolver3::GetMaxNumberOfIterations,
                               R"pbdoc(
			Max number of ICCG iterations.
		)pbdoc")
        .def_property_readonly("lastNumberOfIterations",
                               &FDMICCGSolver3::GetLastNumberOfIterations,
                               R"pbdoc(
			The last number of ICCG iterations the solver made.
		)pbdoc")
        .def_property_readonly("tolerance", &FDMICCGSolver3::GetTolerance,
                               R"pbdoc(
			The max residual tolerance for the ICCG method.
		)pbdoc")
        .def_property_readonly("lastResidual", &FDMICCGSolver3::GetLastResidual,
                               R"pbdoc(
			The last residual after the ICCG iterations.
		)pbdoc");
}