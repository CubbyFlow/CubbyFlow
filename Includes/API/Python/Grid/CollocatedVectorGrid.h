/*************************************************************************
> File Name: CollocatedVectorGrid.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: CollocatedVectorGrid functions for CubbyFlow Python API.
> Created Time: 2018/02/05
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_COLLOCATED_VECTOR_GRID_H
#define CUBBYFLOW_PYTHON_COLLOCATED_VECTOR_GRID_H

#include <pybind11/pybind11.h>

void AddCollocatedVectorGrid2(pybind11::module& m);
void AddCollocatedVectorGrid3(pybind11::module& m);

#endif