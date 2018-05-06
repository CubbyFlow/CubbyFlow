/*************************************************************************
> File Name: CubicSemiLagrangian.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: CubicSemiLagrangian functions for CubbyFlow Python API.
> Created Time: 2018/02/10
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_CUBIC_SEMI_LAGRANGIAN_H
#define CUBBYFLOW_PYTHON_CUBIC_SEMI_LAGRANGIAN_H

#include <pybind11/pybind11.h>

void AddCubicSemiLagrangian2(pybind11::module& m);
void AddCubicSemiLagrangian3(pybind11::module& m);

#endif