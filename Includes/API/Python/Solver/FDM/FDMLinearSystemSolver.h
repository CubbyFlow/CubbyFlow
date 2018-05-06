/*************************************************************************
> File Name: FDMLinearSystemSolver.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: FDMLinearSystemSolver functions for CubbyFlow Python API.
> Created Time: 2018/02/13
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_FDM_LINEAR_SYSTEM_SOLVER_H
#define CUBBYFLOW_PYTHON_FDM_LINEAR_SYSTEM_SOLVER_H

#include <pybind11/pybind11.h>

void AddFDMLinearSystemSolver2(pybind11::module& m);
void AddFDMLinearSystemSolver3(pybind11::module& m);

#endif