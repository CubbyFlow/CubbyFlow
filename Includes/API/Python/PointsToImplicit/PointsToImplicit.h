/*************************************************************************
> File Name: PointsToImplicit.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: PointsToImplicit functions for CubbyFlow Python API.
> Created Time: 2018/02/10
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_POINTS_TO_IMPLICIT_H
#define CUBBYFLOW_PYTHON_POINTS_TO_IMPLICIT_H

#include <pybind11/pybind11.h>

void AddPointsToImplicit2(pybind11::module& m);
void AddPointsToImplicit3(pybind11::module& m);

#endif