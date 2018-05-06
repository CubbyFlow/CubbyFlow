/*************************************************************************
> File Name: ParticleEmitter.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: ParticleEmitter functions for CubbyFlow Python API.
> Created Time: 2018/02/11
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_PARTICLE_EMITTER_H
#define CUBBYFLOW_PYTHON_PARTICLE_EMITTER_H

#include <pybind11/pybind11.h>

void AddParticleEmitter2(pybind11::module& m);
void AddParticleEmitter3(pybind11::module& m);

#endif