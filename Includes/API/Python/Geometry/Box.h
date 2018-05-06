/*************************************************************************
> File Name: Box.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Box functions for CubbyFlow Python API.
> Created Time: 2018/01/28
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_BOX_H
#define CUBBYFLOW_PYTHON_BOX_H

#include <pybind11/pybind11.h>

void AddBox2(pybind11::module& m);
void AddBox3(pybind11::module& m);

#endif