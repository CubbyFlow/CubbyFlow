/*************************************************************************
> File Name: VertexCenteredScalarGrid.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: VertexCenteredScalarGrid functions for CubbyFlow Python API.
> Created Time: 2018/02/07
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_VERTEX_CENTERED_SCALAR_GRID_H
#define CUBBYFLOW_PYTHON_VERTEX_CENTERED_SCALAR_GRID_H

#include <pybind11/pybind11.h>

void AddVertexCenteredScalarGrid2(pybind11::module& m);
void AddVertexCenteredScalarGrid3(pybind11::module& m);

#endif