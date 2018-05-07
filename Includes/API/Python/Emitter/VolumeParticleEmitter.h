/*************************************************************************
> File Name: VolumeParticleEmitter.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: VolumeParticleEmitter functions for CubbyFlow Python API.
> Created Time: 2018/02/11
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_VOLUME_PARTICLE_EMITTER_H
#define CUBBYFLOW_PYTHON_VOLUME_PARTICLE_EMITTER_H

#include <pybind11/pybind11.h>

void AddVolumeParticleEmitter2(pybind11::module& m);
void AddVolumeParticleEmitter3(pybind11::module& m);

#endif