/*************************************************************************
> File Name: Vector.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Vector functions for CubbyFlow Python API.
> Created Time: 2018/01/28
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_VECTOR_H
#define CUBBYFLOW_PYTHON_VECTOR_H

#include <pybind11/pybind11.h>

void AddVector2F(pybind11::module& m);
void AddVector2D(pybind11::module& m);
void AddVector3F(pybind11::module& m);
void AddVector3D(pybind11::module& m);

#endif