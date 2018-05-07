/*************************************************************************
> File Name: PICSolver.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
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