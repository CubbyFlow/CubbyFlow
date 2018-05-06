/*************************************************************************
> File Name: GridSmokeSolver.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: GridSmokeSolver functions for CubbyFlow Python API.
> Created Time: 2018/02/17
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_GRID_SMOKE_SOLVER_H
#define CUBBYFLOW_PYTHON_GRID_SMOKE_SOLVER_H

#include <pybind11/pybind11.h>

void AddGridSmokeSolver2(pybind11::module& m);
void AddGridSmokeSolver3(pybind11::module& m);

#endif