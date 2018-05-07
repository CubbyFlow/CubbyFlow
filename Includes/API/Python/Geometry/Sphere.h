/*************************************************************************
> File Name: Sphere.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Sphere functions for CubbyFlow Python API.
> Created Time: 2018/01/28
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_SPHERE_H
#define CUBBYFLOW_PYTHON_SPHERE_H

#include <pybind11/pybind11.h>

void AddSphere2(pybind11::module& m);
void AddSphere3(pybind11::module& m);

#endif