/*************************************************************************
> File Name: Field.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Field functions for CubbyFlow Python API.
> Created Time: 2018/02/03
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Field/Field.h>
#include <Core/Field/Field2.h>
#include <Core/Field/Field3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddField2(pybind11::module& m)
{
	pybind11::class_<Field2, Field2Ptr>(static_cast<pybind11::handle>(m), "Field2",
		R"pbdoc(
			Abstract base class for 2-D fields.
		)pbdoc");
}

void AddField3(pybind11::module& m)
{
	pybind11::class_<Field3, Field3Ptr>(static_cast<pybind11::handle>(m), "Field3",
		R"pbdoc(
			Abstract base class for 3-D fields.
		)pbdoc");
}