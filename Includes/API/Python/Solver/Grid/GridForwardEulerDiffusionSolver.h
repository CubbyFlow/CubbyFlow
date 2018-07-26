/*************************************************************************
> File Name: GridForwardEulerDiffusionSolver.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: GridForwardEulerDiffusionSolver functions for CubbyFlow Python API.
> Created Time: 2018/02/15
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_GRID_FORWARD_EULER_DIFFUSION_SOLVER_H
#define CUBBYFLOW_PYTHON_GRID_FORWARD_EULER_DIFFUSION_SOLVER_H

#include <pybind11/pybind11.h>

void AddGridForwardEulerDiffusionSolver2(pybind11::module& m);
void AddGridForwardEulerDiffusionSolver3(pybind11::module& m);

#endif