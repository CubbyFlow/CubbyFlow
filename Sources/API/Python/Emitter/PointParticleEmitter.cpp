// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Emitter/PointParticleEmitter.hpp>
#include <API/Python/Utils/pybind11Utils.hpp>
#include <Core/Emitter/PointParticleEmitter2.hpp>
#include <Core/Emitter/PointParticleEmitter3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddPointParticleEmitter2(pybind11::module& m)
{
    pybind11::class_<PointParticleEmitter2, PointParticleEmitter2Ptr>(
        static_cast<pybind11::handle>(m), "PointParticleEmitter2",
        R"pbdoc(
			2-D point particle emitter.
		)pbdoc")
        .def(
            "__init__",
            [](PointParticleEmitter2& instance, pybind11::object origin,
               pybind11::object direction, double speed,
               double spreadAngleInDegrees, size_t maxNumOfNewParticlesPerSec,
               size_t maxNumOfParticles, uint32_t seed) {
                new (&instance) PointParticleEmitter2(
                    ObjectToVector2D(origin), ObjectToVector2D(direction),
                    speed, spreadAngleInDegrees, maxNumOfNewParticlesPerSec,
                    maxNumOfParticles, seed);
            },
            R"pbdoc(
			Constructs an emitter that spawns particles from given origin,
			direction, speed, spread angle, max number of new particles per second,
			max total number of particles to be emitted, and random seed.
		)pbdoc",
            pybind11::arg("origin"), pybind11::arg("direction"),
            pybind11::arg("speed"), pybind11::arg("spreadAngleInDegrees"),
            pybind11::arg("maxNumOfNewParticlesPerSec"),
            pybind11::arg("maxNumOfParticles"), pybind11::arg("seed"))
        .def_property(
            "maxNumberOfNewParticlesPerSecond",
            &PointParticleEmitter2::GetMaxNumberOfNewParticlesPerSecond,
            &PointParticleEmitter2::SetMaxNumberOfNewParticlesPerSecond,
            R"pbdoc(
			The max number of new particles per second.
		)pbdoc")
        .def_property("maxNumberOfParticles",
                      &PointParticleEmitter2::GetMaxNumberOfParticles,
                      &PointParticleEmitter2::SetMaxNumberOfParticles,
                      R"pbdoc(
			The max number of particles to be emitted.
		)pbdoc");
}

void AddPointParticleEmitter3(pybind11::module& m)
{
    pybind11::class_<PointParticleEmitter3, PointParticleEmitter3Ptr>(
        static_cast<pybind11::handle>(m), "PointParticleEmitter3",
        R"pbdoc(
			3-D point particle emitter.
		)pbdoc")
        .def(
            "__init__",
            [](PointParticleEmitter3& instance, pybind11::object origin,
               pybind11::object direction, double speed,
               double spreadAngleInDegrees, size_t maxNumOfNewParticlesPerSec,
               size_t maxNumOfParticles, uint32_t seed) {
                new (&instance) PointParticleEmitter3(
                    ObjectToVector3D(origin), ObjectToVector3D(direction),
                    speed, spreadAngleInDegrees, maxNumOfNewParticlesPerSec,
                    maxNumOfParticles, seed);
            },
            R"pbdoc(
			Constructs an emitter that spawns particles from given origin,
			direction, speed, spread angle, max number of new particles per second,
			max total number of particles to be emitted, and random seed.
		)pbdoc",
            pybind11::arg("origin"), pybind11::arg("direction"),
            pybind11::arg("speed"), pybind11::arg("spreadAngleInDegrees"),
            pybind11::arg("maxNumOfNewParticlesPerSec"),
            pybind11::arg("maxNumOfParticles"), pybind11::arg("seed"))
        .def_property(
            "maxNumberOfNewParticlesPerSecond",
            &PointParticleEmitter3::GetMaxNumberOfNewParticlesPerSecond,
            &PointParticleEmitter3::SetMaxNumberOfNewParticlesPerSecond,
            R"pbdoc(
			The max number of new particles per second.
		)pbdoc")
        .def_property("maxNumberOfParticles",
                      &PointParticleEmitter3::GetMaxNumberOfParticles,
                      &PointParticleEmitter3::SetMaxNumberOfParticles,
                      R"pbdoc(
			The max number of particles to be emitted.
		)pbdoc");
}