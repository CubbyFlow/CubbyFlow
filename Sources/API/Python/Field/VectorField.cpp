/*************************************************************************
> File Name: VectorField.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: VectorField functions for CubbyFlow Python API.
> Created Time: 2018/02/03
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Field/VectorField.h>
#include <Core/Field/VectorField2.h>
#include <Core/Field/VectorField3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddVectorField2(pybind11::module& m)
{
	pybind11::class_<VectorField2, VectorField2Ptr, Field2>(static_cast<pybind11::handle>(m), "VectorField2",
		R"pbdoc(
			Abstract base class for 2-D vector field.
		)pbdoc");
}

void AddVectorField3(pybind11::module& m)
{
	pybind11::class_<VectorField3, VectorField3Ptr, Field3>(static_cast<pybind11::handle>(m), "VectorField3",
		R"pbdoc(
			Abstract base class for 3-D vector field.
		)pbdoc");
}