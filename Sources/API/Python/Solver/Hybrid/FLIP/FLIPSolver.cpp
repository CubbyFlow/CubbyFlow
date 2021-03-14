// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Solver/Hybrid/FLIP/FLIPSolver.hpp>
#include <API/Python/Utils/pybind11Utils.hpp>
#include <Core/Solver/Hybrid/FLIP/FLIPSolver2.hpp>
#include <Core/Solver/Hybrid/FLIP/FLIPSolver3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddFLIPSolver2(pybind11::module& m)
{
    pybind11::class_<FLIPSolver2, FLIPSolver2Ptr, PICSolver2>(m, "FLIPSolver2",
                                                              R"pbdoc(
			2-D Fluid-Implicit Particle (FLIP) implementation.

			This class implements 2-D Fluid-Implicit Particle (FLIP) solver from the
			SIGGRAPH paper, Zhu and Bridson 2005. By transfering delta-velocity field
			from grid to particles, the FLIP solver achieves less viscous fluid flow
			compared to the original PIC method.

			See: Zhu, Yongning, and Robert Bridson. "Animating sand as a fluid."
			ACM Transactions on Graphics (TOG). Vol. 24. No. 2. ACM, 2005.
		)pbdoc")
        .def(
            "__init__",
            [](FLIPSolver2& instance, pybind11::args args,
               pybind11::kwargs kwargs) {
                Vector2UZ resolution{ 1, 1 };
                Vector2D gridSpacing{ 1, 1 };
                Vector2D gridOrigin{ 0, 0 };

                ParseGridResizeParams(args, kwargs, resolution, gridSpacing,
                                      gridOrigin);

                new (&instance)
                    FLIPSolver2(resolution, gridSpacing, gridOrigin);
            },
            R"pbdoc(
			Constructs FLIPSolver2

			This method constructs FLIPSolver2 with resolution, gridSpacing,
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
        .def_property("picBlendingFactor", &FLIPSolver2::GetPICBlendingFactor,
                      &FLIPSolver2::SetPICBlendingFactor,
                      R"pbdoc(
			The PIC blending factor.

			This property specifies the PIC blending factor which mixes FLIP and PIC
			results when transferring velocity from grids to particles in order to
			reduce the noise. The factor can be a value between 0 and 1, where 0
			means no blending and 1 means full PIC. Default is 0.
		)pbdoc");
}

void AddFLIPSolver3(pybind11::module& m)
{
    pybind11::class_<FLIPSolver3, FLIPSolver3Ptr, PICSolver3>(m, "FLIPSolver3",
                                                              R"pbdoc(
			3-D Fluid-Implicit Particle (FLIP) implementation.

			This class implements 3-D Fluid-Implicit Particle (FLIP) solver from the
			SIGGRAPH paper, Zhu and Bridson 2005. By transfering delta-velocity field
			from grid to particles, the FLIP solver achieves less viscous fluid flow
			compared to the original PIC method.

			See: Zhu, Yongning, and Robert Bridson. "Animating sand as a fluid."
			ACM Transactions on Graphics (TOG). Vol. 24. No. 3. ACM, 2005.
		)pbdoc")
        .def(
            "__init__",
            [](FLIPSolver3& instance, pybind11::args args,
               pybind11::kwargs kwargs) {
                Vector3UZ resolution{ 1, 1, 1 };
                Vector3D gridSpacing{ 1, 1, 1 };
                Vector3D gridOrigin{ 0, 0, 0 };

                ParseGridResizeParams(args, kwargs, resolution, gridSpacing,
                                      gridOrigin);

                new (&instance)
                    FLIPSolver3(resolution, gridSpacing, gridOrigin);
            },
            R"pbdoc(
			Constructs FLIPSolver3

			This method constructs FLIPSolver3 with resolution, gridSpacing,
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
        .def_property("picBlendingFactor", &FLIPSolver3::GetPICBlendingFactor,
                      &FLIPSolver3::SetPICBlendingFactor,
                      R"pbdoc(
			The PIC blending factor.

			This property specifies the PIC blending factor which mixes FLIP and PIC
			results when transferring velocity from grids to particles in order to
			reduce the noise. The factor can be a value between 0 and 1, where 0
			means no blending and 1 means full PIC. Default is 0.
		)pbdoc");
}