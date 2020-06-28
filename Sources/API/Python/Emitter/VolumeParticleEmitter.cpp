// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Emitter/VolumeParticleEmitter.hpp>
#include <API/Python/Utils/pybind11Utils.hpp>
#include <Core/Emitter/VolumeParticleEmitter2.hpp>
#include <Core/Emitter/VolumeParticleEmitter3.hpp>
#include <Core/Surface/SurfaceToImplicit2.hpp>
#include <Core/Surface/SurfaceToImplicit3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddVolumeParticleEmitter2(pybind11::module& m)
{
    pybind11::class_<VolumeParticleEmitter2, VolumeParticleEmitter2Ptr,
                     ParticleEmitter2>(m, "VolumeParticleEmitter2")
        .def(
            "__init__",
            [](VolumeParticleEmitter2& instance, pybind11::args args,
               pybind11::kwargs kwargs) {
                ImplicitSurface2Ptr implicitSurface;
                BoundingBox2D bounds;
                double spacing = 0.1;
                Vector2D initialVel;
                size_t maxNumberOfParticles =
                    std::numeric_limits<size_t>::max();
                double jitter = 0.0;
                bool isOneShot = true;
                bool allowOverlapping = false;
                uint32_t seed = 0;

                const auto parseImplicitSurface = [&](pybind11::object arg) {
                    if (pybind11::isinstance<ImplicitSurface2>(arg))
                    {
                        implicitSurface = arg.cast<ImplicitSurface2Ptr>();
                        bounds = implicitSurface->BoundingBox();
                    }
                    else if (pybind11::isinstance<Surface2>(arg))
                    {
                        auto surface = arg.cast<Surface2Ptr>();
                        implicitSurface =
                            std::make_shared<SurfaceToImplicit2>(surface);
                        bounds = surface->BoundingBox();
                    }
                    else
                    {
                        throw std::invalid_argument(
                            "Unknown type for ImplicitSurface.");
                    }
                };

                if (args.size() >= 3 && args.size() <= 9)
                {
                    parseImplicitSurface(args[0]);

                    bounds = args[1].cast<BoundingBox2D>();
                    spacing = args[2].cast<double>();

                    if (args.size() > 3)
                    {
                        initialVel =
                            ObjectToVector2D(pybind11::object(args[3]));
                    }
                    if (args.size() > 4)
                    {
                        maxNumberOfParticles = args[4].cast<size_t>();
                    }
                    if (args.size() > 5)
                    {
                        jitter = args[5].cast<double>();
                    }
                    if (args.size() > 6)
                    {
                        isOneShot = args[6].cast<bool>();
                    }
                    if (args.size() > 7)
                    {
                        allowOverlapping = args[7].cast<bool>();
                    }
                    if (args.size() > 8)
                    {
                        seed = args[8].cast<uint32_t>();
                    }
                }
                else if (args.size() > 0)
                {
                    throw std::invalid_argument("Too few/many arguments.");
                }

                if (kwargs.contains("implicitSurface"))
                {
                    parseImplicitSurface(kwargs["implicitSurface"]);
                }
                if (kwargs.contains("bounds"))
                {
                    bounds = kwargs["bounds"].cast<BoundingBox2D>();
                }
                if (kwargs.contains("spacing"))
                {
                    spacing = kwargs["spacing"].cast<double>();
                }
                if (kwargs.contains("initialVelocity"))
                {
                    initialVel = ObjectToVector2D(kwargs["initialVelocity"]);
                }
                if (kwargs.contains("maxNumberOfParticles"))
                {
                    maxNumberOfParticles =
                        kwargs["maxNumberOfParticles"].cast<size_t>();
                }
                if (kwargs.contains("jitter"))
                {
                    jitter = kwargs["jitter"].cast<double>();
                }
                if (kwargs.contains("isOneShot"))
                {
                    isOneShot = kwargs["isOneShot"].cast<bool>();
                }
                if (kwargs.contains("allowOverlapping"))
                {
                    allowOverlapping = kwargs["allowOverlapping"].cast<bool>();
                }
                if (kwargs.contains("seed"))
                {
                    seed = kwargs["seed"].cast<uint32_t>();
                }

                new (&instance) VolumeParticleEmitter2(
                    implicitSurface, bounds, spacing, initialVel,
                    maxNumberOfParticles, jitter, isOneShot, allowOverlapping,
                    seed);
            },
            R"pbdoc(
			Constructs VolumeParticleEmitter2

			This method constructs VolumeParticleEmitter2 with implicit
			surface, bounding box, particle spacing, initial velocity
			(optional), max number of particles (optional), jitter
			(optional), whether it's one shot or not (optional), whether it
			should allow overlapping or not (optional), and random seed
			(optional).
		)pbdoc")
        .def_property("jitter", &VolumeParticleEmitter2::GetJitter,
                      &VolumeParticleEmitter2::SetJitter, R"pbdoc(
             Jitter amount between 0 and 1.
             )pbdoc")
        .def_property("isOneShot", &VolumeParticleEmitter2::GetIsOneShot,
                      &VolumeParticleEmitter2::SetIsOneShot, R"pbdoc(
             True if particles should be emitted just once.
             )pbdoc")
        .def_property("allowOverlapping",
                      &VolumeParticleEmitter2::GetAllowOverlapping,
                      &VolumeParticleEmitter2::SetAllowOverlapping, R"pbdoc(
             True if particles can be overlapped.
             )pbdoc")
        .def_property("maxNumberOfParticles",
                      &VolumeParticleEmitter2::GetMaxNumberOfParticles,
                      &VolumeParticleEmitter2::SetMaxNumberOfParticles, R"pbdoc(
             Max number of particles to be emitted.
             )pbdoc")
        .def_property("spacing", &VolumeParticleEmitter2::GetSpacing,
                      &VolumeParticleEmitter2::SetSpacing, R"pbdoc(
             The spacing between particles.
             )pbdoc")
        .def_property(
            "initialVelocity", &VolumeParticleEmitter2::GetInitialVelocity,
            [](VolumeParticleEmitter2& instance,
               pybind11::object newInitialVel) {
                instance.SetInitialVelocity(ObjectToVector2D(newInitialVel));
            },
            R"pbdoc(
             The initial velocity of the particles.
             )pbdoc");
}

void AddVolumeParticleEmitter3(pybind11::module& m)
{
    pybind11::class_<VolumeParticleEmitter3, VolumeParticleEmitter3Ptr,
                     ParticleEmitter3>(m, "VolumeParticleEmitter3")
        .def(
            "__init__",
            [](VolumeParticleEmitter3& instance, pybind11::args args,
               pybind11::kwargs kwargs) {
                ImplicitSurface3Ptr implicitSurface;
                BoundingBox3D bounds;
                double spacing = 0.1;
                Vector3D initialVel;
                size_t maxNumberOfParticles =
                    std::numeric_limits<size_t>::max();
                double jitter = 0.0;
                bool isOneShot = true;
                bool allowOverlapping = false;
                uint32_t seed = 0;

                const auto parseImplicitSurface = [&](pybind11::object arg) {
                    if (pybind11::isinstance<ImplicitSurface3>(arg))
                    {
                        implicitSurface = arg.cast<ImplicitSurface3Ptr>();
                        bounds = implicitSurface->BoundingBox();
                    }
                    else if (pybind11::isinstance<Surface3>(arg))
                    {
                        auto surface = arg.cast<Surface3Ptr>();
                        implicitSurface =
                            std::make_shared<SurfaceToImplicit3>(surface);
                        bounds = surface->BoundingBox();
                    }
                    else
                    {
                        throw std::invalid_argument(
                            "Unknown type for ImplicitSurface.");
                    }
                };

                if (args.size() >= 3 && args.size() <= 9)
                {
                    parseImplicitSurface(args[0]);

                    bounds = args[1].cast<BoundingBox3D>();
                    spacing = args[2].cast<double>();

                    if (args.size() > 3)
                    {
                        initialVel =
                            ObjectToVector3D(pybind11::object(args[3]));
                    }
                    if (args.size() > 4)
                    {
                        maxNumberOfParticles = args[4].cast<size_t>();
                    }
                    if (args.size() > 5)
                    {
                        jitter = args[5].cast<double>();
                    }
                    if (args.size() > 6)
                    {
                        isOneShot = args[6].cast<bool>();
                    }
                    if (args.size() > 7)
                    {
                        allowOverlapping = args[7].cast<bool>();
                    }
                    if (args.size() > 8)
                    {
                        seed = args[8].cast<uint32_t>();
                    }
                }
                else if (args.size() > 0)
                {
                    throw std::invalid_argument("Too few/many arguments.");
                }

                if (kwargs.contains("implicitSurface"))
                {
                    parseImplicitSurface(kwargs["implicitSurface"]);
                }
                if (kwargs.contains("bounds"))
                {
                    bounds = kwargs["bounds"].cast<BoundingBox3D>();
                }
                if (kwargs.contains("spacing"))
                {
                    spacing = kwargs["spacing"].cast<double>();
                }
                if (kwargs.contains("initialVelocity"))
                {
                    initialVel = ObjectToVector3D(kwargs["initialVelocity"]);
                }
                if (kwargs.contains("maxNumberOfParticles"))
                {
                    maxNumberOfParticles =
                        kwargs["maxNumberOfParticles"].cast<size_t>();
                }
                if (kwargs.contains("jitter"))
                {
                    jitter = kwargs["jitter"].cast<double>();
                }
                if (kwargs.contains("isOneShot"))
                {
                    isOneShot = kwargs["isOneShot"].cast<bool>();
                }
                if (kwargs.contains("allowOverlapping"))
                {
                    allowOverlapping = kwargs["allowOverlapping"].cast<bool>();
                }
                if (kwargs.contains("seed"))
                {
                    seed = kwargs["seed"].cast<uint32_t>();
                }

                new (&instance) VolumeParticleEmitter3(
                    implicitSurface, bounds, spacing, initialVel,
                    maxNumberOfParticles, jitter, isOneShot, allowOverlapping,
                    seed);
            },
            R"pbdoc(
			Constructs VolumeParticleEmitter3

			This method constructs VolumeParticleEmitter3 with implicit
			surface, bounding box, particle spacing, initial velocity
			(optional), max number of particles (optional), jitter
			(optional), whether it's one shot or not (optional), whether it
			should allow overlapping or not (optional), and random seed
			(optional).
		)pbdoc")
        .def_property("jitter", &VolumeParticleEmitter3::GetJitter,
                      &VolumeParticleEmitter3::SetJitter, R"pbdoc(
             Jitter amount between 0 and 1.
             )pbdoc")
        .def_property("isOneShot", &VolumeParticleEmitter3::GetIsOneShot,
                      &VolumeParticleEmitter3::SetIsOneShot, R"pbdoc(
             True if particles should be emitted just once.
             )pbdoc")
        .def_property("allowOverlapping",
                      &VolumeParticleEmitter3::GetAllowOverlapping,
                      &VolumeParticleEmitter3::SetAllowOverlapping, R"pbdoc(
             True if particles can be overlapped.
             )pbdoc")
        .def_property("maxNumberOfParticles",
                      &VolumeParticleEmitter3::GetMaxNumberOfParticles,
                      &VolumeParticleEmitter3::SetMaxNumberOfParticles, R"pbdoc(
             Max number of particles to be emitted.
             )pbdoc")
        .def_property("spacing", &VolumeParticleEmitter3::GetSpacing,
                      &VolumeParticleEmitter3::SetSpacing, R"pbdoc(
             The spacing between particles.
             )pbdoc")
        .def_property(
            "initialVelocity", &VolumeParticleEmitter3::GetInitialVelocity,
            [](VolumeParticleEmitter3& instance,
               pybind11::object newInitialVel) {
                instance.SetInitialVelocity(ObjectToVector3D(newInitialVel));
            },
            R"pbdoc(
             The initial velocity of the particles.
             )pbdoc");
}