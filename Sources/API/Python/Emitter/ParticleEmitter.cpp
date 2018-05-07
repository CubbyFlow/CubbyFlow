/*************************************************************************
> File Name: ParticleEmitter.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: ParticleEmitter functions for CubbyFlow Python API.
> Created Time: 2018/02/11
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Emitter/ParticleEmitter.h>
#include <Core/Emitter/ParticleEmitter2.h>
#include <Core/Emitter/ParticleEmitter3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddParticleEmitter2(pybind11::module& m)
{
	pybind11::class_<ParticleEmitter2, ParticleEmitter2Ptr>(m, "ParticleEmitter2",
		R"pbdoc(
			Abstract base class for 2-D particle emitter.
		)pbdoc")
	.def("Update", &ParticleEmitter2::Update,
		R"pbdoc(
			Updates the emitter state from `currentTimeInSeconds` to the following
			time-step.
		)pbdoc",
		pybind11::arg("currentTimeInSeconds"),
		pybind11::arg("timeIntervalInSeconds"))
	.def_property("target", &ParticleEmitter2::GetTarget, &ParticleEmitter2::SetTarget,
		R"pbdoc(
			The target particle system to emit.
		)pbdoc")
	.def("SetOnBeginUpdateCallback", &ParticleEmitter2::SetOnBeginUpdateCallback,
		R"pbdoc(
			Sets the callback function to be called when `Update` function is invoked.

			The callback function takes current simulation time in seconds unit. Use
			this callback to track any motion or state changes related to this
			emitter.
		)pbdoc");
}

void AddParticleEmitter3(pybind11::module& m)
{
	pybind11::class_<ParticleEmitter3, ParticleEmitter3Ptr>(m, "ParticleEmitter3",
		R"pbdoc(
			Abstract base class for 3-D particle emitter.
		)pbdoc")
	.def("Update", &ParticleEmitter3::Update,
		R"pbdoc(
			Updates the emitter state from `currentTimeInSeconds` to the following
			time-step.
		)pbdoc",
		pybind11::arg("currentTimeInSeconds"),
		pybind11::arg("timeIntervalInSeconds"))
	.def_property("target", &ParticleEmitter3::GetTarget, &ParticleEmitter3::SetTarget,
		R"pbdoc(
			The target particle system to emit.
		)pbdoc")
	.def("SetOnBeginUpdateCallback", &ParticleEmitter3::SetOnBeginUpdateCallback,
		R"pbdoc(
			Sets the callback function to be called when `update` function is invoked.

			The callback function takes current simulation time in seconds unit. Use
			this callback to track any motion or state changes related to this
			emitter.
		)pbdoc");
}