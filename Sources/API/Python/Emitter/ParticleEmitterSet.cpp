// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Emitter/ParticleEmitterSet.hpp>
#include <Core/Emitter/ParticleEmitterSet2.hpp>
#include <Core/Emitter/ParticleEmitterSet3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddParticleEmitterSet2(pybind11::module& m)
{
    pybind11::class_<ParticleEmitterSet2, ParticleEmitterSet2Ptr,
                     ParticleEmitter2>(m, "ParticleEmitterSet2",
                                       R"pbdoc(
			2-D particle-based emitter set.
		)pbdoc")
        .def(
            "__init__",
            [](ParticleEmitterSet2& instance, pybind11::list emitters) {
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
    pybind11::class_<ParticleEmitterSet3, ParticleEmitterSet3Ptr,
                     ParticleEmitter3>(m, "ParticleEmitterSet3",
                                       R"pbdoc(
			3-D particle-based emitter set.
		)pbdoc")
        .def(
            "__init__",
            [](ParticleEmitterSet3& instance, pybind11::list emitters) {
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