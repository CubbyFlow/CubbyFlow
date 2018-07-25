/*************************************************************************
> File Name: GridEmitter.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: GridEmitter functions for CubbyFlow Python API.
> Created Time: 2018/02/11
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_GRID_EMITTER_H
#define CUBBYFLOW_PYTHON_GRID_EMITTER_H

#include <pybind11/pybind11.h>

void AddGridEmitter2(pybind11::module& m);
void AddGridEmitter3(pybind11::module& m);

#endif