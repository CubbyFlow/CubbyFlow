/*************************************************************************
> File Name: TriangleMesh.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: TriangleMesh functions for CubbyFlow Python API.
> Created Time: 2018/01/28
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_TRIANGLE_MESH_H
#define CUBBYFLOW_PYTHON_TRIANGLE_MESH_H

#include <pybind11/pybind11.h>

void AddTriangleMesh3(pybind11::module& m);

#endif