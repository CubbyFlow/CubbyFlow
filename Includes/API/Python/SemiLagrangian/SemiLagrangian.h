/*************************************************************************
> File Name: SemiLagrangian.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: SemiLagrangian functions for CubbyFlow Python API.
> Created Time: 2018/02/10
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_SEMI_LAGRANGIAN_H
#define CUBBYFLOW_PYTHON_SEMI_LAGRANGIAN_H

#include <pybind11/pybind11.h>

void AddSemiLagrangian2(pybind11::module& m);
void AddSemiLagrangian3(pybind11::module& m);

#endif