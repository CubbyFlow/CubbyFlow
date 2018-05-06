/*************************************************************************
> File Name: FaceCenteredGrid.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: FaceCenteredGrid functions for CubbyFlow Python API.
> Created Time: 2018/02/07
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_FACE_CENTERED_GRID_H
#define CUBBYFLOW_PYTHON_FACE_CENTERED_GRID_H

#include <pybind11/pybind11.h>

void AddFaceCenteredGrid2(pybind11::module& m);
void AddFaceCenteredGrid3(pybind11::module& m);

#endif