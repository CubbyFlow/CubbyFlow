/*************************************************************************
> File Name: ScalarField.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: ScalarField functions for CubbyFlow Python API.
> Created Time: 2018/02/03
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_SCALAR_FIELD_H
#define CUBBYFLOW_PYTHON_SCALAR_FIELD_H

#include <pybind11/pybind11.h>

void AddScalarField2(pybind11::module& m);
void AddScalarField3(pybind11::module& m);

#endif