/*************************************************************************
> File Name: GridPressureSolver.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: GridPressureSolver functions for CubbyFlow Python API.
> Created Time: 2018/02/15
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_GRID_PRESSURE_SOLVER_H
#define CUBBYFLOW_PYTHON_GRID_PRESSURE_SOLVER_H

#include <pybind11/pybind11.h>

void AddGridPressureSolver2(pybind11::module& m);
void AddGridPressureSolver3(pybind11::module& m);

#endif