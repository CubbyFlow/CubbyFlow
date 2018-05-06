/*************************************************************************
> File Name: VolumeGridEmitter.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: VolumeGridEmitter functions for CubbyFlow Python API.
> Created Time: 2018/02/11
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_VOLUME_GRID_EMITTER_H
#define CUBBYFLOW_PYTHON_VOLUME_GRID_EMITTER_H

#include <pybind11/pybind11.h>

void AddVolumeGridEmitter2(pybind11::module& m);
void AddVolumeGridEmitter3(pybind11::module& m);

#endif