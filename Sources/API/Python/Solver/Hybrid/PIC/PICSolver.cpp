// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Solver/Hybrid/PIC/PICSolver.hpp>
#include <API/Python/Utils/pybind11Utils.hpp>
#include <Core/Solver/Hybrid/PIC/PICSolver2.hpp>
#include <Core/Solver/Hybrid/PIC/PICSolver3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddPICSolver2(pybind11::module& m)
{
    pybind11::class_<PICSolver2, PICSolver2Ptr, GridFluidSolver2>(m,
                                                                  "PICSolver2")
        .def(
            "__init__",
            [](PICSolver2& instance, pybind11::args args,
               pybind11::kwargs kwargs) {
                Size2 resolution{ 1, 1 };
                Vector2D gridSpacing{ 1, 1 };
                Vector2D gridOrigin{ 0, 0 };

                ParseGridResizeParams(args, kwargs, resolution, gridSpacing,
                                      gridOrigin);

                new (&instance) PICSolver2(resolution, gridSpacing, gridOrigin);
            },
            R"pbdoc(
			Constructs PICSolver2

			This method constructs PICSolver2 with resolution, gridSpacing,
			and gridOrigin.

			Parameters
			----------
			- `*args` : resolution, gridSpacing, and gridOrigin arguments.
			- `**kwargs`
				- resolution : Grid resolution.
				- gridSpacing : Grid spacing.
				- gridOrigin : Origin point ot the grid.
				- domainSizeX : Domain size in x-direction.
		)pbdoc")
        .def_property_readonly("particleSystemData",
                               &PICSolver2::GetParticleSystemData,
                               R"pbdoc(
			Returns particleSystemData.
		)pbdoc")
        .def_property("particleEmitter", &PICSolver2::GetParticleEmitter,
                      &PICSolver2::SetParticleEmitter,
                      R"pbdoc(
			Particle emitter property.
		)pbdoc");
}

void AddPICSolver3(pybind11::module& m)
{
    pybind11::class_<PICSolver3, PICSolver3Ptr, GridFluidSolver3>(m,
                                                                  "PICSolver3")
        .def(
            "__init__",
            [](PICSolver3& instance, pybind11::args args,
               pybind11::kwargs kwargs) {
                Size3 resolution{ 1, 1, 1 };
                Vector3D gridSpacing{ 1, 1, 1 };
                Vector3D gridOrigin{ 0, 0, 0 };

                ParseGridResizeParams(args, kwargs, resolution, gridSpacing,
                                      gridOrigin);

                new (&instance) PICSolver3(resolution, gridSpacing, gridOrigin);
            },
            R"pbdoc(
			Constructs PICSolver3

			This method constructs PICSolver3 with resolution, gridSpacing,
			and gridOrigin.

			Parameters
			----------
			- `*args` : resolution, gridSpacing, and gridOrigin arguments.
			- `**kwargs`
				- resolution : Grid resolution.
				- gridSpacing : Grid spacing.
				- gridOrigin : Origin point ot the grid.
				- domainSizeX : Domain size in x-direction.
		)pbdoc")
        .def_property_readonly("particleSystemData",
                               &PICSolver3::GetParticleSystemData,
                               R"pbdoc(
			Returns particleSystemData.
		)pbdoc")
        .def_property("particleEmitter", &PICSolver3::GetParticleEmitter,
                      &PICSolver3::SetParticleEmitter,
                      R"pbdoc(
			Particle emitter property.
		)pbdoc");
}