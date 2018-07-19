/*************************************************************************
> File Name: CubicSemiLagrangian.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: CubicSemiLagrangian functions for CubbyFlow Python API.
> Created Time: 2018/02/10
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Python/SemiLagrangian/CubicSemiLagrangian.h>
#include <SemiLagrangian/CubicSemiLagrangian2.h>
#include <SemiLagrangian/CubicSemiLagrangian3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddCubicSemiLagrangian2(pybind11::module& m)
{
	pybind11::class_<CubicSemiLagrangian2, CubicSemiLagrangian2Ptr, SemiLagrangian2>(static_cast<pybind11::handle>(m), "CubicSemiLagrangian2",
		R"pbdoc(
			Implementation of 2-D cubic semi-Lagrangian advection solver.

			This class implements 3rd-order cubic 2-D semi-Lagrangian advection solver.
		)pbdoc")
	.def(pybind11::init<>());
}

void AddCubicSemiLagrangian3(pybind11::module& m){
	pybind11::class_<CubicSemiLagrangian3, CubicSemiLagrangian3Ptr, SemiLagrangian3>(static_cast<pybind11::handle>(m), "CubicSemiLagrangian3",
		R"pbdoc(
			Implementation of 3-D cubic semi-Lagrangian advection solver.

			This class implements 3rd-order cubic 3-D semi-Lagrangian advection solver.
		)pbdoc")
	.def(pybind11::init<>());
}