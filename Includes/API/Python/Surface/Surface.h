/*************************************************************************
> File Name: Surface.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Surface functions for CubbyFlow Python API.
> Created Time: 2018/01/29
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_SURFACE_H
#define CUBBYFLOW_PYTHON_SURFACE_H

#include <pybind11/pybind11.h>

void AddSurface2(pybind11::module& m);
void AddSurface3(pybind11::module& m);

#endif