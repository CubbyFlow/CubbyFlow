/*************************************************************************
> File Name: PointParticleEmitter.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: PointParticleEmitter functions for CubbyFlow Python API.
> Created Time: 2018/02/11
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_POINT_PARTICLE_EMITTER_H
#define CUBBYFLOW_PYTHON_POINT_PARTICLE_EMITTER_H

#include <pybind11/pybind11.h>

void AddPointParticleEmitter2(pybind11::module& m);
void AddPointParticleEmitter3(pybind11::module& m);

#endif