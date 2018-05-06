/*************************************************************************
> File Name: PICSolver.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: PICSolver functions for CubbyFlow Python API.
> Created Time: 2018/02/17
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_PIC_SOLVER_H
#define CUBBYFLOW_PYTHON_PIC_SOLVER_H

#include <pybind11/pybind11.h>

void AddPICSolver2(pybind11::module& m);
void AddPICSolver3(pybind11::module& m);

#endif