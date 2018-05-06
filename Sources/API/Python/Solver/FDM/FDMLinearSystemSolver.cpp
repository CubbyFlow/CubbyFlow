/*************************************************************************
> File Name: FDMLinearSystemSolver.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: FDMLinearSystemSolver functions for CubbyFlow Python API.
> Created Time: 2018/02/13
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Solver/FDM/FDMLinearSystemSolver.h>
#include <Core/Solver/FDM/FDMLinearSystemSolver2.h>
#include <Core/Solver/FDM/FDMLinearSystemSolver3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddFDMLinearSystemSolver2(pybind11::module& m)
{
	pybind11::class_<FDMLinearSystemSolver2, FDMLinearSystemSolver2Ptr>(static_cast<pybind11::handle>(m), "FDMLinearSystemSolver2",
		R"pbdoc(
			Abstract base class for 2-D finite difference-type linear system solver.
		)pbdoc");
}

void AddFDMLinearSystemSolver3(pybind11::module& m)
{
	pybind11::class_<FDMLinearSystemSolver3, FDMLinearSystemSolver3Ptr>(static_cast<pybind11::handle>(m), "FDMLinearSystemSolver3",
		R"pbdoc(
			Abstract base class for 3-D finite difference-type linear system solver.
		)pbdoc");
}