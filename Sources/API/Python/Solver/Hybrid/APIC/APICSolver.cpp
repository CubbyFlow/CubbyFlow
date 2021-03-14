// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Solver/Hybrid/APIC/APICSolver.hpp>
#include <API/Python/Utils/pybind11Utils.hpp>
#include <Core/Solver/Hybrid/APIC/APICSolver2.hpp>
#include <Core/Solver/Hybrid/APIC/APICSolver3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddAPICSolver2(pybind11::module& m)
{
    pybind11::class_<APICSolver2, APICSolver2Ptr, PICSolver2>(m, "APICSolver2",
                                                              R"pbdoc(
			2-D Affine Particle-in-Cell (APIC) implementation

			This class implements 2-D Affine Particle-in-Cell (APIC) solver from the
			SIGGRAPH paper, Jiang 2015.

			See: Jiang, Chenfanfu, et al. "The affine particle-in-cell method."
			ACM Transactions on Graphics (TOG) 34.4 (2015): 51.
		)pbdoc")
        .def(
            "__init__",
            [](APICSolver2& instance, pybind11::args args,
               pybind11::kwargs kwargs) {
                Vector2UZ resolution{ 1, 1 };
                Vector2D gridSpacing{ 1, 1 };
                Vector2D gridOrigin{ 0, 0 };

                ParseGridResizeParams(args, kwargs, resolution, gridSpacing,
                                      gridOrigin);

                new (&instance)
                    APICSolver2(resolution, gridSpacing, gridOrigin);
            },
            R"pbdoc(
			Constructs APICSolver2

			This method constructs APICSolver2 with resolution, gridSpacing,
			and gridOrigin.

			Parameters
			----------
			- `*args` : resolution, gridSpacing, and gridOrigin arguments.
			- `**kwargs`
				- resolution : Grid resolution.
				- gridSpacing : Grid spacing.
				- gridOrigin : Origin point at the grid.
				- domainSizeX : Domain size in x-direction.
		)pbdoc");
}

void AddAPICSolver3(pybind11::module& m)
{
    pybind11::class_<APICSolver3, APICSolver3Ptr, PICSolver3>(m, "APICSolver3",
                                                              R"pbdoc(
			3-D Affine Particle-in-Cell (APIC) implementation

			This class implements 3-D Affine Particle-in-Cell (APIC) solver from the
			SIGGRAPH paper, Jiang 2015.

			See: Jiang, Chenfanfu, et al. "The affine particle-in-cell method."
			ACM Transactions on Graphics (TOG) 34.4 (2015): 51.
		)pbdoc")
        .def(
            "__init__",
            [](APICSolver3& instance, pybind11::args args,
               pybind11::kwargs kwargs) {
                Vector3UZ resolution{ 1, 1, 1 };
                Vector3D gridSpacing{ 1, 1, 1 };
                Vector3D gridOrigin{ 0, 0, 0 };

                ParseGridResizeParams(args, kwargs, resolution, gridSpacing,
                                      gridOrigin);

                new (&instance)
                    APICSolver3(resolution, gridSpacing, gridOrigin);
            },
            R"pbdoc(
			Constructs APICSolver3

			This method constructs APICSolver3 with resolution, gridSpacing,
			and gridOrigin.

			Parameters
			----------
			- `*args` : resolution, gridSpacing, and gridOrigin arguments.
			- `**kwargs`
				- resolution : Grid resolution.
				- gridSpacing : Grid spacing.
				- gridOrigin : Origin point at the grid.
				- domainSizeX : Domain size in x-direction.
		)pbdoc");
}