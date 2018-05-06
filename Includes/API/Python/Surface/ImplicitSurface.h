/*************************************************************************
> File Name: ImplicitSurface.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: ImplicitSurface functions for CubbyFlow Python API.
> Created Time: 2018/02/07
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_IMPLICIT_SURFACE_H
#define CUBBYFLOW_PYTHON_IMPLICIT_SURFACE_H

#include <pybind11/pybind11.h>

void AddImplicitSurface2(pybind11::module& m);
void AddImplicitSurface3(pybind11::module& m);

#endif