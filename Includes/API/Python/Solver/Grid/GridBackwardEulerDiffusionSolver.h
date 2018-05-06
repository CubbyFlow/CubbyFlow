/*************************************************************************
> File Name: GridBackwardEulerDiffusionSolver.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: GridBackwardEulerDiffusionSolver functions for CubbyFlow Python API.
> Created Time: 2018/02/15
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_GRID_BACKWARD_EULER_DIFFUSION_SOLVER_H
#define CUBBYFLOW_PYTHON_GRID_BACKWARD_EULER_DIFFUSION_SOLVER_H

#include <pybind11/pybind11.h>

void AddGridBackwardEulerDiffusionSolver2(pybind11::module& m);
void AddGridBackwardEulerDiffusionSolver3(pybind11::module& m);

#endif