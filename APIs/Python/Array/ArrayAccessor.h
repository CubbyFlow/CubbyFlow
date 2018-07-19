/*************************************************************************
> File Name: ArrayAccessor.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: ArrayAccessor functions for CubbyFlow Python API.
> Created Time: 2018/01/26
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_ARRAY_ACCESSOR_H
#define CUBBYFLOW_PYTHON_ARRAY_ACCESSOR_H

#include <pybind11/pybind11.h>

void AddArrayAccessor1(pybind11::module& m);
void AddArrayAccessor2(pybind11::module& m);
void AddArrayAccessor3(pybind11::module& m);

#endif