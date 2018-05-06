/*************************************************************************
> File Name: GridDiffusionSolver.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: GridDiffusionSolver functions for CubbyFlow Python API.
> Created Time: 2018/02/15
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_GRID_DIFFUSION_SOLVER_H
#define CUBBYFLOW_PYTHON_GRID_DIFFUSION_SOLVER_H

#include <pybind11/pybind11.h>

void AddGridDiffusionSolver2(pybind11::module& m);
void AddGridDiffusionSolver3(pybind11::module& m);

#endif