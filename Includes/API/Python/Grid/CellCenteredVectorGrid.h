/*************************************************************************
> File Name: CellCenteredVectorGrid.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: CellCenteredVectorGrid functions for CubbyFlow Python API.
> Created Time: 2018/02/06
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_CELL_CENTERED_VECTOR_GRID_H
#define CUBBYFLOW_PYTHON_CELL_CENTERED_VECTOR_GRID_H

#include <pybind11/pybind11.h>

void AddCellCenteredVectorGrid2(pybind11::module& m);
void AddCellCenteredVectorGrid3(pybind11::module& m);

#endif