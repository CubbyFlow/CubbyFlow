/*************************************************************************
> File Name: Sphere.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Sphere functions for CubbyFlow Python API.
> Created Time: 2018/01/28
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_SPHERE_H
#define CUBBYFLOW_PYTHON_SPHERE_H

#include <pybind11/pybind11.h>

void AddSphere2(pybind11::module& m);
void AddSphere3(pybind11::module& m);

#endif