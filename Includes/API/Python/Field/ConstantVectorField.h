/*************************************************************************
> File Name: ConstantVectorField.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: ConstantVectorField functions for CubbyFlow Python API.
> Created Time: 2018/02/04
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_CONSTANT_VECTOR_FIELD_H
#define CUBBYFLOW_PYTHON_CONSTANT_VECTOR_FIELD_H

#include <pybind11/pybind11.h>

void AddConstantVectorField2(pybind11::module& m);
void AddConstantVectorField3(pybind11::module& m);

#endif