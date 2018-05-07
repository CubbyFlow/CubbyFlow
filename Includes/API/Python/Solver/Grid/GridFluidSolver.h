/*************************************************************************
> File Name: GridFluidSolver.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: GridFluidSolver functions for CubbyFlow Python API.
> Created Time: 2018/02/17
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_GRID_FLUID_SOLVER_H
#define CUBBYFLOW_PYTHON_GRID_FLUID_SOLVER_H

#include <pybind11/pybind11.h>

void AddGridFluidSolver2(pybind11::module& m);
void AddGridFluidSolver3(pybind11::module& m);

#endif