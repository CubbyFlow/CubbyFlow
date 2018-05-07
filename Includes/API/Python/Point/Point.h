/*************************************************************************
> File Name: Point.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Point functions for CubbyFlow Python API.
> Created Time: 2018/01/29
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_POINT_H
#define CUBBYFLOW_PYTHON_POINT_H

#include <pybind11/pybind11.h>

void AddPoint2UI(pybind11::module& m);
void AddPoint3UI(pybind11::module& m);

#endif