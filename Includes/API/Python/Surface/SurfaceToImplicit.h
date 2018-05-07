/*************************************************************************
> File Name: SurfaceToImplicit.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: SurfaceToImplicit functions for CubbyFlow Python API.
> Created Time: 2018/01/29
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_SURFACE_TO_IMPLICIT_H
#define CUBBYFLOW_PYTHON_SURFACE_TO_IMPLICIT_H

#include <pybind11/pybind11.h>

void AddSurfaceToImplicit2(pybind11::module& m);
void AddSurfaceToImplicit3(pybind11::module& m);

#endif