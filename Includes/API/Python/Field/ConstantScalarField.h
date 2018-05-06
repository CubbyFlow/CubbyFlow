/*************************************************************************
> File Name: ConstantScalarField.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: ConstantScalarField functions for CubbyFlow Python API.
> Created Time: 2018/02/04
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_CONSTANT_SCALAR_FIELD_H
#define CUBBYFLOW_PYTHON_CONSTANT_SCALAR_FIELD_H

#include <pybind11/pybind11.h>

void AddConstantScalarField2(pybind11::module& m);
void AddConstantScalarField3(pybind11::module& m);

#endif