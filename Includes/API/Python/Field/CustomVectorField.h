/*************************************************************************
> File Name: CustomVectorField.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: CustomVectorField functions for CubbyFlow Python API.
> Created Time: 2018/02/04
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_CUSTOM_VECTOR_FIELD_H
#define CUBBYFLOW_PYTHON_CUSTOM_VECTOR_FIELD_H

#include <pybind11/pybind11.h>

void AddCustomVectorField2(pybind11::module& m);
void AddCustomVectorField3(pybind11::module& m);

#endif