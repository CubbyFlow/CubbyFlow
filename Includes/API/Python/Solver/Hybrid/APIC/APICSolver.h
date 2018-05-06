/*************************************************************************
> File Name: APICSolver.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: APICSolver functions for CubbyFlow Python API.
> Created Time: 2018/02/17
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_APIC_SOLVER_H
#define CUBBYFLOW_PYTHON_APIC_SOLVER_H

#include <pybind11/pybind11.h>

void AddAPICSolver2(pybind11::module& m);
void AddAPICSolver3(pybind11::module& m);

#endif