/*************************************************************************
> File Name: ParticleEmitterSet.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: ParticleEmitterSet functions for CubbyFlow Python API.
> Created Time: 2018/02/11
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Python/Emitter/ParticleEmitterSet.h>
#include <Emitter/ParticleEmitterSet2.h>
#include <Emitter/ParticleEmitterSet3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddParticleEmitterSet2(pybind11::module& m)
{
	pybind11::class_<ParticleEmitterSet2, ParticleEmitterSet2Ptr, ParticleEmitter2>(m, "ParticleEmitterSet2",
		R"pbdoc(
			2-D particle-based emitter set.
		)pbdoc")
	.def("__init__", [](ParticleEmitterSet2& instance, pybind11::list emitters)
	{
		std::vector<ParticleEmitter2Ptr> emitters_(emitters.size());
		for (size_t i = 0; i < emitters_.size(); ++i)
		{
			emitters_[i] = emitters[i].cast<ParticleEmitter2Ptr>();
		}

		new (&instance) ParticleEmitterSet2(emitters_);
	},
		R"pbdoc(
			Constructs an emitter with sub-emitters.
		)pbdoc",
		pybind11::arg("emitters"))
	.def("AddEmitter", &ParticleEmitterSet2::AddEmitter,
		R"pbdoc(
			Adds sub-emitter.
		)pbdoc",
		pybind11::arg("emitter"));
}

void AddParticleEmitterSet3(pybind11::module& m)
{
	pybind11::class_<ParticleEmitterSet3, ParticleEmitterSet3Ptr, ParticleEmitter3>(m, "ParticleEmitterSet3",
		R"pbdoc(
			3-D particle-based emitter set.
		)pbdoc")
	.def("__init__", [](ParticleEmitterSet3& instance, pybind11::list emitters)
	{
		std::vector<ParticleEmitter3Ptr> emitters_(emitters.size());
		for (size_t i = 0; i < emitters_.size(); ++i)
		{
			emitters_[i] = emitters[i].cast<ParticleEmitter3Ptr>();
		}
		
		new (&instance) ParticleEmitterSet3(emitters_);
	},
		R"pbdoc(
			Constructs an emitter with sub-emitters.
		)pbdoc",
		pybind11::arg("emitters"))
	.def("AddEmitter", &ParticleEmitterSet3::AddEmitter,
		R"pbdoc(
			Adds sub-emitter.
		)pbdoc",
		pybind11::arg("emitter"));
}