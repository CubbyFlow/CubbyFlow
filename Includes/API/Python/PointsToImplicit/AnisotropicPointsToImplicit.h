/*************************************************************************
> File Name: AnisotropicPointsToImplicit.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: AnisotropicPointsToImplicit functions for CubbyFlow Python API.
> Created Time: 2018/02/11
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_ANISOTROPIC_POINTS_TO_IMPLICIT_H
#define CUBBYFLOW_PYTHON_ANISOTROPIC_POINTS_TO_IMPLICIT_H

#include <pybind11/pybind11.h>

void AddAnisotropicPointsToImplicit2(pybind11::module& m);
void AddAnisotropicPointsToImplicit3(pybind11::module& m);

#endif