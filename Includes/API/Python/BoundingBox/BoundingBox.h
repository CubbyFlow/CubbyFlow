/*************************************************************************
> File Name: BoundingBox.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: BoundingBox functions for CubbyFlow Python API.
> Created Time: 2018/01/30
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_BOUNDING_BOX_H
#define CUBBYFLOW_PYTHON_BOUNDING_BOX_H

#include <pybind11/pybind11.h>

void AddBoundingBox2F(pybind11::module& m);
void AddBoundingBox2D(pybind11::module& m);
void AddBoundingBox3F(pybind11::module& m);
void AddBoundingBox3D(pybind11::module& m);

#endif