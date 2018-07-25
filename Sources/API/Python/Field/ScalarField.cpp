/*************************************************************************
> File Name: ScalarField.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: ScalarField functions for CubbyFlow Python API.
> Created Time: 2018/02/03
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Field/ScalarField.h>
#include <Core/Field/ScalarField2.h>
#include <Core/Field/ScalarField3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddScalarField2(pybind11::module& m)
{
	pybind11::class_<ScalarField2, ScalarField2Ptr, Field2>(static_cast<pybind11::handle>(m), "ScalarField2",
		R"pbdoc(
			Abstract base class for 2-D scalar field.
		)pbdoc");
}

void AddScalarField3(pybind11::module& m)
{
	pybind11::class_<ScalarField3, ScalarField3Ptr, Field3>(static_cast<pybind11::handle>(m), "ScalarField3",
		R"pbdoc(
			Abstract base class for 3-D scalar field.
		)pbdoc");
}