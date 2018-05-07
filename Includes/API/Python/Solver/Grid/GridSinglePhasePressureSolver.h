/*************************************************************************
> File Name: GridSinglePhasePressureSolver.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: GridSinglePhasePressureSolver functions for CubbyFlow Python API.
> Created Time: 2018/02/15
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_GRID_SINGLE_PHASE_PRESSURE_SOLVER_H
#define CUBBYFLOW_PYTHON_GRID_SINGLE_PHASE_PRESSURE_SOLVER_H

#include <pybind11/pybind11.h>

void AddGridSinglePhasePressureSolver2(pybind11::module& m);
void AddGridSinglePhasePressureSolver3(pybind11::module& m);

#endif