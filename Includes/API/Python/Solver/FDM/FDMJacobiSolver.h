/*************************************************************************
> File Name: FDMJacobiSolver.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: FDMJacobiSolver functions for CubbyFlow Python API.
> Created Time: 2018/02/13
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_FDM_JACOBI_SOLVER_H
#define CUBBYFLOW_PYTHON_FDM_JACOBI_SOLVER_H

#include <pybind11/pybind11.h>

void AddFDMJacobiSolver2(pybind11::module& m);
void AddFDMJacobiSolver3(pybind11::module& m);

#endif