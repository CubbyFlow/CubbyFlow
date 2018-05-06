/*************************************************************************
> File Name: SurfaceSet.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: SurfaceSet functions for CubbyFlow Python API.
> Created Time: 2018/01/29
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_SURFACE_SET_H
#define CUBBYFLOW_PYTHON_SURFACE_SET_H

#include <pybind11/pybind11.h>

void AddSurfaceSet2(pybind11::module& m);
void AddSurfaceSet3(pybind11::module& m);

#endif