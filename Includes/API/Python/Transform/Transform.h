/*************************************************************************
> File Name: Transform.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Transform functions for CubbyFlow Python API.
> Created Time: 2018/01/29
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_TRANSFORM_H
#define CUBBYFLOW_PYTHON_TRANSFORM_H

#include <pybind11/pybind11.h>

void AddTransform2(pybind11::module& m);
void AddTransform3(pybind11::module& m);

#endif