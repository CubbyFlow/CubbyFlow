/*************************************************************************
> File Name: FDMJacobiSolver.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: FDMJacobiSolver functions for CubbyFlow Python API.
> Created Time: 2018/02/13
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Solver/FDM/FDMJacobiSolver.h>
#include <Core/Solver/FDM/FDMJacobiSolver2.h>
#include <Core/Solver/FDM/FDMJacobiSolver3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddFDMJacobiSolver2(pybind11::module& m)
{
	pybind11::class_<FDMJacobiSolver2, FDMJacobiSolver2Ptr, FDMLinearSystemSolver2>(static_cast<pybind11::handle>(m), "FDMJacobiSolver2",
		R"pbdoc(
			2-D finite difference-type linear system solver using conjugate gradient.
		)pbdoc")
	.def(pybind11::init<uint32_t, uint32_t, double>(),
		pybind11::arg("maxNumberOfIterations"),
		pybind11::arg("residualCheckInterval"),
		pybind11::arg("tolerance"))
	.def_property_readonly("maxNumberOfIterations", &FDMJacobiSolver2::GetMaxNumberOfIterations,
		R"pbdoc(
			Max number of CG iterations.
		)pbdoc")
	.def_property_readonly("lastNumberOfIterations", &FDMJacobiSolver2::GetLastNumberOfIterations,
		R"pbdoc(
			The last number of CG iterations the solver made.
		)pbdoc")
	.def_property_readonly("tolerance", &FDMJacobiSolver2::GetTolerance,
		R"pbdoc(
			The max residual tolerance for the CG method.
		)pbdoc")
	.def_property_readonly("lastResidual", &FDMJacobiSolver2::GetLastResidual,
		R"pbdoc(
			The last residual after the CG iterations.
		)pbdoc");
}

void AddFDMJacobiSolver3(pybind11::module& m)
{
	pybind11::class_<FDMJacobiSolver3, FDMJacobiSolver3Ptr, FDMLinearSystemSolver3>(static_cast<pybind11::handle>(m), "FDMJacobiSolver3",
		R"pbdoc(
			3-D finite difference-type linear system solver using conjugate gradient.
		)pbdoc")
	.def(pybind11::init<uint32_t, uint32_t, double>(),
		pybind11::arg("maxNumberOfIterations"),
		pybind11::arg("residualCheckInterval"),
		pybind11::arg("tolerance"))
	.def_property_readonly("maxNumberOfIterations", &FDMJacobiSolver3::GetMaxNumberOfIterations,
		R"pbdoc(
			Max number of CG iterations.
		)pbdoc")
	.def_property_readonly("lastNumberOfIterations", &FDMJacobiSolver3::GetLastNumberOfIterations,
		R"pbdoc(
			The last number of CG iterations the solver made.
		)pbdoc")
	.def_property_readonly("tolerance", &FDMJacobiSolver3::GetTolerance,
		R"pbdoc(
			The max residual tolerance for the CG method.
		)pbdoc")
	.def_property_readonly("lastResidual", &FDMJacobiSolver3::GetLastResidual,
		R"pbdoc(
			The last residual after the CG iterations.
		)pbdoc");
}