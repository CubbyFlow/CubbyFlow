/*************************************************************************
> File Name: Serializable.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Serializable functions for CubbyFlow Python API.
> Created Time: 2018/02/01
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_SERIALIZABLE_H
#define CUBBYFLOW_PYTHON_SERIALIZABLE_H

#include <pybind11/pybind11.h>

void AddSerializable(pybind11::module& m);

#endif