/*************************************************************************
> File Name: Plane.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Plane functions for CubbyFlow Python API.
> Created Time: 2018/01/28
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_PLANE_H
#define CUBBYFLOW_PYTHON_PLANE_H

#include <pybind11/pybind11.h>

void AddPlane2(pybind11::module& m);
void AddPlane3(pybind11::module& m);

#endif