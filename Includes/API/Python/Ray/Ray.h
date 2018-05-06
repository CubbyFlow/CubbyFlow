/*************************************************************************
> File Name: Ray.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Ray functions for CubbyFlow Python API.
> Created Time: 2018/01/29
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_RAY_H
#define CUBBYFLOW_PYTHON_RAY_H

#include <pybind11/pybind11.h>

void AddRay2F(pybind11::module& m);
void AddRay2D(pybind11::module& m);
void AddRay3F(pybind11::module& m);
void AddRay3D(pybind11::module& m);

#endif