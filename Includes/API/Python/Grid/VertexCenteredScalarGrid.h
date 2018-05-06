/*************************************************************************
> File Name: VertexCenteredScalarGrid.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
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