/*************************************************************************
> File Name: Quaternion.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Quaternion functions for CubbyFlow Python API.
> Created Time: 2018/01/30
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_QUATERNION_H
#define CUBBYFLOW_PYTHON_QUATERNION_H

#include <pybind11/pybind11.h>

void AddQuaternionF(pybind11::module& m);
void AddQuaternionD(pybind11::module& m);

#endif