/*************************************************************************
> File Name: LevelSetLiquidSolver.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: LevelSetLiquidSolver functions for CubbyFlow Python API.
> Created Time: 2018/02/17
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Solver/LevelSet/LevelSetLiquidSolver.h>
#include <API/Python/Utils/pybind11Utils.h>
#include <Core/Solver/LevelSet/LevelSetLiquidSolver2.h>
#include <Core/Solver/LevelSet/LevelSetLiquidSolver3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddLevelSetLiquidSolver2(pybind11::module& m)
{
	pybind11::class_<LevelSetLiquidSolver2, LevelSetLiquidSolver2Ptr, GridFluidSolver2>(m, "LevelSetLiquidSolver2",
		R"pbdoc(
			Level set based 2-D liquid solver.

			This class implements level set-based 2-D liquid solver. It defines the
			surface of the liquid using signed-distance field and use stable fluids
			framework to compute the forces.

			- See Enright, Douglas, Stephen Marschner, and Ronald Fedkiw.
			"Animation and rendering of complex water surfaces." ACM Transactions on
			Graphics (TOG). Vol. 21. No. 3. ACM, 2002.
		)pbdoc")
	.def("__init__", [](LevelSetLiquidSolver2& instance, pybind11::args args, pybind11::kwargs kwargs)
	{
		Size2 resolution{ 1, 1, 1 };
		Vector2D gridSpacing{ 1, 1, 1 };
		Vector2D gridOrigin{ 0, 0, 0 };

		ParseGridResizeParams(args, kwargs, resolution, gridSpacing, gridOrigin);

		new (&instance) LevelSetLiquidSolver2(resolution, gridSpacing, gridOrigin);
	},
		R"pbdoc(
			Constructs LevelSetLiquidSolver2

			This method constructs LevelSetLiquidSolver2 with resolution, gridSpacing,
			and gridOrigin.

			Parameters
			----------
			- `*args` : resolution, gridSpacing, and gridOrigin arguments.
			- `**kwargs`
				- resolution : Grid resolution.
				- gridSpacing : Grid spacing.
				- gridOrigin : Origin point at the grid.
				- domainSizeX : Domain size in x-direction.
		)pbdoc")
	.def_property_readonly("signedDistanceField", &LevelSetLiquidSolver2::GetSignedDistanceField,
		R"pbdoc(
			The signed-distance field.
		)pbdoc")
	.def_property("levelSetSolver", &LevelSetLiquidSolver2::GetLevelSetSolver, &LevelSetLiquidSolver2::SetLevelSetSolver,
		R"pbdoc(
			The level set solver.
		)pbdoc")
	.def("SetMinReinitializeDistance", &LevelSetLiquidSolver2::SetMinReinitializeDistance,
		R"pbdoc(
			Sets minimum reinitialization distance.
		)pbdoc")
	.def("SetIsGlobalCompensationEnabled", &LevelSetLiquidSolver2::SetIsGlobalCompensationEnabled,
		R"pbdoc(
			Enables (or disables) global compensation feature flag.

			When `isEnabled` is true, the global compensation feature is enabled.
			The global compensation measures the volume at the beginning and the end
			of the time-step and adds the volume change back to the level-set field
			by globally shifting the front.
			\see Song, Oh-Young, Hyuncheol Shin, and Hyeong-Seok Ko.
			"Stable but nondissipative water." ACM Transactions on Graphics (TOG)
			24, no. 1 (2005): 81-97.
		)pbdoc",
		pybind11::arg("isEnabled"))
	.def("ComputeVolume", &LevelSetLiquidSolver2::ComputeVolume,
		R"pbdoc(
			Returns liquid volume measured by smeared Heaviside function.

			This function measures the liquid volume using smeared Heaviside
			function. Thus, the estimated volume is an approximated quantity.
		)pbdoc");
}

void AddLevelSetLiquidSolver3(pybind11::module& m)
{
	pybind11::class_<LevelSetLiquidSolver3, LevelSetLiquidSolver3Ptr, GridFluidSolver3>(m, "LevelSetLiquidSolver3",
		R"pbdoc(
			Level set based 3-D liquid solver.

			This class implements level set-based 3-D liquid solver. It defines the
			surface of the liquid using signed-distance field and use stable fluids
			framework to compute the forces.

			- See Enright, Douglas, Stephen Marschner, and Ronald Fedkiw.
			"Animation and rendering of complex water surfaces." ACM Transactions on
			 Graphics (TOG). Vol. 21. No. 3. ACM, 2002.
		)pbdoc")
	.def("__init__", [](LevelSetLiquidSolver3& instance, pybind11::args args, pybind11::kwargs kwargs) 
	{
		Size3 resolution{ 1, 1, 1 };
		Vector3D gridSpacing{ 1, 1, 1 };
		Vector3D gridOrigin{ 0, 0, 0 };

		ParseGridResizeParams(args, kwargs, resolution, gridSpacing, gridOrigin);

		new (&instance) LevelSetLiquidSolver3(resolution, gridSpacing, gridOrigin);
	},
		R"pbdoc(
			Constructs LevelSetLiquidSolver3

			This method constructs LevelSetLiquidSolver3 with resolution, gridSpacing,
			and gridOrigin.

			Parameters
			----------
			- `*args` : resolution, gridSpacing, and gridOrigin arguments.
			- `**kwargs`
				- resolution : Grid resolution.
				- gridSpacing : Grid spacing.
				- gridOrigin : Origin point at the grid.
				- domainSizeX : Domain size in x-direction.
		)pbdoc")
	.def_property_readonly("signedDistanceField", &LevelSetLiquidSolver3::GetSignedDistanceField,
		R"pbdoc(
			The signed-distance field.
		)pbdoc")
	.def_property("levelSetSolver", &LevelSetLiquidSolver3::GetLevelSetSolver, &LevelSetLiquidSolver3::SetLevelSetSolver,
		R"pbdoc(
			The level set solver.
		)pbdoc")
	.def("SetMinReinitializeDistance", &LevelSetLiquidSolver3::SetMinReinitializeDistance,
		R"pbdoc(
			Sets minimum reinitialization distance.
		)pbdoc")
	.def("SetIsGlobalCompensationEnabled", &LevelSetLiquidSolver3::SetIsGlobalCompensationEnabled,
		R"pbdoc(
			Enables (or disables) global compensation feature flag.

			When `isEnabled` is true, the global compensation feature is enabled.
			The global compensation measures the volume at the beginning and the end
			of the time-step and adds the volume change back to the level-set field
			by globally shifting the front.
			\see Song, Oh-Young, Hyuncheol Shin, and Hyeong-Seok Ko.
			"Stable but nondissipative water." ACM Transactions on Graphics (TOG)
			24, no. 1 (2005): 81-97.
		)pbdoc",
		pybind11::arg("isEnabled"))
	.def("ComputeVolume", &LevelSetLiquidSolver3::ComputeVolume,
		R"pbdoc(
			Returns liquid volume measured by smeared Heaviside function.

			This function measures the liquid volume using smeared Heaviside
			function. Thus, the estimated volume is an approximated quantity.
		)pbdoc");
}