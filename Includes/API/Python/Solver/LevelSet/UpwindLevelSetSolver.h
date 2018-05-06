/*************************************************************************
> File Name: UpwindLevelSetSolver.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: UpwindLevelSetSolver functions for CubbyFlow Python API.
> Created Time: 2018/02/16
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_UPWIND_LEVEL_SET_SOLVER_H
#define CUBBYFLOW_PYTHON_UPWIND_LEVEL_SET_SOLVER_H

#include <pybind11/pybind11.h>

void AddUpwindLevelSetSolver2(pybind11::module& m);
void AddUpwindLevelSetSolver3(pybind11::module& m);

#endif