/*************************************************************************
> File Name: FDMCGSolver.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: FDMCGSolver functions for CubbyFlow Python API.
> Created Time: 2018/02/14
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_FDM_CG_SOLVER_H
#define CUBBYFLOW_PYTHON_FDM_CG_SOLVER_H

#include <pybind11/pybind11.h>

void AddFDMCGSolver2(pybind11::module& m);
void AddFDMCGSolver3(pybind11::module& m);

#endif