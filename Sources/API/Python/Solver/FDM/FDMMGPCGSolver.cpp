/*************************************************************************
> File Name: FDMMGPCGSolver.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: FDMMGPCGSolver functions for CubbyFlow Python API.
> Created Time: 2018/02/14
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Solver/FDM/FDMMGPCGSolver.h>
#include <Core/Solver/FDM/FDMMGPCGSolver2.h>
#include <Core/Solver/FDM/FDMMGPCGSolver3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddFDMMGPCGSolver2(pybind11::module& m)
{
	pybind11::class_<FDMMGPCGSolver2, FDMMGPCGSolver2Ptr, FDMLinearSystemSolver2>(m, "FDMMGPCGSolver2",
		R"pbdoc(
			2-D finite difference-type linear system solver using MGPCG.
		)pbdoc")
	.def(pybind11::init<uint32_t, size_t, uint32_t, uint32_t, uint32_t, uint32_t, double, double, bool>(),
		pybind11::arg("numberOfCGIter"),
		pybind11::arg("maxNumberOfLevels"),
		pybind11::arg("numberOfRestrictionIter") = 5,
		pybind11::arg("numberOfCorrectionIter") = 5,
		pybind11::arg("numberOfCoarsestIter") = 20,
		pybind11::arg("numberOfFinalIter") = 20,
		pybind11::arg("maxTolerance") = 1e-9,
		pybind11::arg("sorFactor") = 1.5,
		pybind11::arg("useRedBlackOrdering") = false)
	.def_property_readonly("maxNumberOfIterations", &FDMMGPCGSolver2::GetMaxNumberOfIterations,
		R"pbdoc(
			Max number of MGPCG iterations.
		)pbdoc")
	.def_property_readonly("lastNumberOfIterations", &FDMMGPCGSolver2::GetLastNumberOfIterations,
		R"pbdoc(
			The last number of MGPCG iterations the solver made.
		)pbdoc")
	.def_property_readonly("tolerance", &FDMMGPCGSolver2::GetTolerance,
		R"pbdoc(
			The max residual tolerance for the MGPCG method.
		)pbdoc")
	.def_property_readonly("lastResidual", &FDMMGPCGSolver2::GetLastResidual,
		R"pbdoc(
			The last residual after the MGPCG iterations.
		)pbdoc")
	.def_property_readonly("sorFactor", &FDMMGPCGSolver2::GetSORFactor,
		R"pbdoc(
			Returns the SOR (Successive Over Relaxation) factor.
		)pbdoc")
	.def_property_readonly("useRedBlackOrdering", &FDMMGPCGSolver2::GetUseRedBlackOrdering,
		R"pbdoc(
			Returns true if red-black ordering is enabled.
		)pbdoc");
}

void AddFDMMGPCGSolver3(pybind11::module& m)
{
	pybind11::class_<FDMMGPCGSolver3, FDMMGPCGSolver3Ptr, FDMLinearSystemSolver3>(m, "FDMMGPCGSolver3",
		R"pbdoc(
			3-D finite difference-type linear system solver using MGPCG.
		)pbdoc")
	.def(pybind11::init<uint32_t, size_t, uint32_t, uint32_t, uint32_t, uint32_t, double, double, bool>(),
		pybind11::arg("numberOfCGIter"),
		pybind11::arg("maxNumberOfLevels"),
		pybind11::arg("numberOfRestrictionIter") = 5,
		pybind11::arg("numberOfCorrectionIter") = 5,
		pybind11::arg("numberOfCoarsestIter") = 20,
		pybind11::arg("numberOfFinalIter") = 20,
		pybind11::arg("maxTolerance") = 1e-9,
		pybind11::arg("sorFactor") = 1.5,
		pybind11::arg("useRedBlackOrdering") = false)
	.def_property_readonly("maxNumberOfIterations", &FDMMGPCGSolver3::GetMaxNumberOfIterations,
		R"pbdoc(
			Max number of MGPCG iterations.
		)pbdoc")
	.def_property_readonly("lastNumberOfIterations", &FDMMGPCGSolver3::GetLastNumberOfIterations,
		R"pbdoc(
			The last number of MGPCG iterations the solver made.
		)pbdoc")
	.def_property_readonly("tolerance", &FDMMGPCGSolver3::GetTolerance,
		R"pbdoc(
			The max residual tolerance for the MGPCG method.
		)pbdoc")
	.def_property_readonly("lastResidual", &FDMMGPCGSolver3::GetLastResidual,
		R"pbdoc(
			The last residual after the MGPCG iterations.
		)pbdoc")
	.def_property_readonly("sorFactor", &FDMMGPCGSolver3::GetSORFactor,
		R"pbdoc(
			Returns the SOR (Successive Over Relaxation) factor.
		)pbdoc")
	.def_property_readonly("useRedBlackOrdering", &FDMMGPCGSolver3::GetUseRedBlackOrdering,
		R"pbdoc(
			Returns true if red-black ordering is enabled.
		)pbdoc");
}