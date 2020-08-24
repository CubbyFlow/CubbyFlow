// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Solver/FDM/FDMMGSolver.hpp>
#include <Core/Solver/FDM/FDMMGSolver2.hpp>
#include <Core/Solver/FDM/FDMMGSolver3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddFDMMGSolver2(pybind11::module& m)
{
    pybind11::class_<FDMMGSolver2, FDMMGSolver2Ptr, FDMLinearSystemSolver2>(
        m, "FDMMGSolver2",
        R"pbdoc(
			2-D finite difference-type linear system solver using multigrid.
		)pbdoc")
        .def(pybind11::init<size_t, uint32_t, uint32_t, uint32_t, uint32_t,
                            double, double, bool>(),
             pybind11::arg("maxNumberOfLevels"),
             pybind11::arg("numberOfRestrictionIter") = 5,
             pybind11::arg("numberOfCorrectionIter") = 5,
             pybind11::arg("numberOfCoarsestIter") = 20,
             pybind11::arg("numberOfFinalIter") = 20,
             pybind11::arg("maxTolerance") = 1e-9,
             pybind11::arg("sorFactor") = 1.5,
             pybind11::arg("useRedBlackOrdering") = false)
        .def_property_readonly(
            "maxNumberOfLevels",
            [](const FDMMGSolver2& instance) {
                return instance.GetParams().maxNumberOfLevels;
            },
            R"pbdoc(
			Max number of multigrid levels.
		)pbdoc")
        .def_property_readonly(
            "numberOfRestrictionIter",
            [](const FDMMGSolver2& instance) {
                return instance.GetParams().numberOfRestrictionIter;
            },
            R"pbdoc(
			Number of iteration at restriction step.
		)pbdoc")
        .def_property_readonly(
            "numberOfCorrectionIter",
            [](const FDMMGSolver2& instance) {
                return instance.GetParams().numberOfCorrectionIter;
            },
            R"pbdoc(
			Number of iteration at correction step.
		)pbdoc")
        .def_property_readonly(
            "numberOfCoarsestIter",
            [](const FDMMGSolver2& instance) {
                return instance.GetParams().numberOfCoarsestIter;
            },
            R"pbdoc(
			Number of iteration at coarsest step.
		)pbdoc")
        .def_property_readonly(
            "numberOfFinalIter",
            [](const FDMMGSolver2& instance) {
                return instance.GetParams().numberOfFinalIter;
            },
            R"pbdoc(
			Number of iteration at final step.
		)pbdoc")
        .def_property_readonly("sorFactor", &FDMMGSolver2::GetSORFactor,
                               R"pbdoc(
			Returns the SOR (Successive Over Relaxation) factor.
		)pbdoc")
        .def_property_readonly("useRedBlackOrdering",
                               &FDMMGSolver2::GetUseRedBlackOrdering,
                               R"pbdoc(
			Returns true if red-black ordering is enabled.
		)pbdoc");
}

void AddFDMMGSolver3(pybind11::module& m)
{
    pybind11::class_<FDMMGSolver3, FDMMGSolver3Ptr, FDMLinearSystemSolver3>(
        m, "FDMMGSolver3",
        R"pbdoc(
			3-D finite difference-type linear system solver using multigrid.
		)pbdoc")
        .def(pybind11::init<size_t, uint32_t, uint32_t, uint32_t, uint32_t,
                            double, double, bool>(),
             pybind11::arg("maxNumberOfLevels"),
             pybind11::arg("numberOfRestrictionIter") = 5,
             pybind11::arg("numberOfCorrectionIter") = 5,
             pybind11::arg("numberOfCoarsestIter") = 20,
             pybind11::arg("numberOfFinalIter") = 20,
             pybind11::arg("maxTolerance") = 1e-9,
             pybind11::arg("sorFactor") = 1.5,
             pybind11::arg("useRedBlackOrdering") = false)
        .def_property_readonly(
            "maxNumberOfLevels",
            [](const FDMMGSolver3& instance) {
                return instance.GetParams().maxNumberOfLevels;
            },
            R"pbdoc(
			Max number of multigrid levels.
		)pbdoc")
        .def_property_readonly(
            "numberOfRestrictionIter",
            [](const FDMMGSolver3& instance) {
                return instance.GetParams().numberOfRestrictionIter;
            },
            R"pbdoc(
			Number of iteration at restriction step.
		)pbdoc")
        .def_property_readonly(
            "numberOfCorrectionIter",
            [](const FDMMGSolver3& instance) {
                return instance.GetParams().numberOfCorrectionIter;
            },
            R"pbdoc(
			Number of iteration at correction step.
		)pbdoc")
        .def_property_readonly(
            "numberOfCoarsestIter",
            [](const FDMMGSolver3& instance) {
                return instance.GetParams().numberOfCoarsestIter;
            },
            R"pbdoc(
			Number of iteration at coarsest step.
		)pbdoc")
        .def_property_readonly(
            "numberOfFinalIter",
            [](const FDMMGSolver3& instance) {
                return instance.GetParams().numberOfFinalIter;
            },
            R"pbdoc(
			Number of iteration at final step.
		)pbdoc")
        .def_property_readonly("sorFactor", &FDMMGSolver3::GetSORFactor,
                               R"pbdoc(
			Returns the SOR (Successive Over Relaxation) factor.
		)pbdoc")
        .def_property_readonly("useRedBlackOrdering",
                               &FDMMGSolver3::GetUseRedBlackOrdering,
                               R"pbdoc(
			Returns true if red-black ordering is enabled.
		)pbdoc");
}