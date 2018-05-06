/*************************************************************************
> File Name: GridFluidSolver.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: GridFluidSolver functions for CubbyFlow Python API.
> Created Time: 2018/02/17
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Solver/Grid/GridFluidSolver.h>
#include <API/Python/Utils/pybind11Utils.h>
#include <Core/Solver/Grid/GridFluidSolver2.h>
#include <Core/Solver/Grid/GridFluidSolver3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddGridFluidSolver2(pybind11::module& m)
{
	pybind11::class_<GridFluidSolver2, GridFluidSolver2Ptr, PhysicsAnimation>(m, "GridFluidSolver2",
		R"pbdoc(
			Abstract base class for grid-based 2-D fluid solver.

			This is an abstract base class for grid-based 2-D fluid solver based on
			Jos Stam's famous 1999 paper - "Stable Fluids". This solver takes fractional
			step method as its foundation which is consisted of independent advection,
			diffusion, external forces, and pressure projection steps. Each step is
			configurable so that a custom step can be implemented. For example, if a
			user wants to change the advection solver to her/his own implementation,
			simply call setAdvectionSolver(newSolver).
		)pbdoc")
	.def_property("gravity", &GridFluidSolver2::GetGravity, [](GridFluidSolver2& instance, pybind11::object obj)
	{
		instance.SetGravity(ObjectToVector2D(obj));
	},
		R"pbdoc(
			The gravity vector of the system.
		)pbdoc")
	.def_property("viscosityCoefficient", &GridFluidSolver2::GetViscosityCoefficient, &GridFluidSolver2::SetViscosityCoefficient,
		R"pbdoc(
			The viscosity coefficient.
		)pbdoc")
	.def("GetCFL", &GridFluidSolver2::GetCFL,
		R"pbdoc(
			Returns the CFL number from the current velocity field for given
			time interval.

			Parameters
			----------
			- timeIntervalInSeconds : The time interval in seconds.
		)pbdoc",
		pybind11::arg("timeIntervalInSeconds"))
	.def_property("maxCFL", &GridFluidSolver2::GetMaxCFL, &GridFluidSolver2::SetMaxCFL,
		R"pbdoc(
			The max allowed CFL number.
		)pbdoc")
	.def_property("useCompressedLinearSystem", &GridFluidSolver2::GetUseCompressedLinearSystem, &GridFluidSolver2::SetUseCompressedLinearSystem,
		R"pbdoc(
			True if the solver is using compressed linear system.
		)pbdoc")
	.def_property("advectionSolver", &GridFluidSolver2::GetAdvectionSolver, &GridFluidSolver2::SetAdvectionSolver,
		R"pbdoc(
			The advection solver.
		)pbdoc")
	.def_property("diffusionSolver", &GridFluidSolver2::GetDiffusionSolver, &GridFluidSolver2::SetDiffusionSolver,
		R"pbdoc(
			The diffusion solver instance.
		)pbdoc")
	.def_property("pressureSolver", &GridFluidSolver2::GetPressureSolver, &GridFluidSolver2::SetPressureSolver,
		R"pbdoc(
			The pressure solver instance.
		)pbdoc")
	.def_property("closedDomainBoundaryFlag", &GridFluidSolver2::GetClosedDomainBoundaryFlag, &GridFluidSolver2::SetClosedDomainBoundaryFlag,
		R"pbdoc(
			The closed domain boundary flag.

			This flag is an integer which is bitwise-combination of
			DIRECTION_LEFT, DIRECTION_RIGHT, DIRECTION_DOWN, DIRECTION_UP,
			and DIRECTION_BACK.
		)pbdoc")
	.def("ResizeGrid", [](GridFluidSolver2& instance, pybind11::args args, pybind11::kwargs kwargs)
	{
		Size2 resolution{ 1, 1 };
		Vector2D gridSpacing{ 1, 1 };
		Vector2D gridOrigin{ 0, 0 };

		ParseGridResizeParams(args, kwargs, resolution, gridSpacing, gridOrigin);

		instance.ResizeGrid(resolution, gridSpacing, gridOrigin);
	},
		R"pbdoc(
			Resizes grid system data.

			This function resizes grid system data. You can also resize the grid by
			calling resize function directly from
			GridFluidSolver2::GetGridSystemData(), but this function provides a
			shortcut for the same operation.

			Parameters
			----------
			- `*args` : resolution, gridSpacing, and gridOrigin arguments.
			- `**kwargs`
				- resolution : Grid resolution.
				- gridSpacing : Grid spacing.
				- gridOrigin : Origin point at the grid.
				- domainSizeX : Domain size in x-direction.
		)pbdoc")
	.def_property_readonly("resolution", &GridFluidSolver2::GetResolution,
		R"pbdoc(
			The resolution of the grid system data.

			This function returns the resolution of the grid system data. This is
			equivalent to calling gridSystemData.resolution, but provides a
			shortcut.
		)pbdoc")
	.def_property_readonly("gridSpacing", &GridFluidSolver2::GetGridSpacing,
		R"pbdoc(
			The grid spacing of the grid system data.

			This function returns the resolution of the grid system data. This is
			equivalent to calling gridSystemData.gridSpacing, but provides a
			shortcut.
		)pbdoc")
	.def_property_readonly("gridOrigin", &GridFluidSolver2::GetGridOrigin,
		R"pbdoc(
			The origin of the grid system data.

			This function returns the resolution of the grid system data. This is
			equivalent to calling gridSystemData.origin, but provides a
			shortcut.
		)pbdoc")
	.def_property_readonly("velocity", &GridFluidSolver2::GetVelocity,
		R"pbdoc(
			The velocity field.

			This property returns the velocity field from the grid system data.
			It is just a shortcut to the most commonly accessed data chunk.
		)pbdoc")
	.def_property("collider", [](const GridFluidSolver2& instance)
	{
		return instance.GetCollider();
	},
		[](GridFluidSolver2& instance, const Collider2Ptr& collider)
	{
		instance.SetCollider(collider);
	},
		R"pbdoc(
			The collider.
		)pbdoc")
	.def_property("emitter", [](const GridFluidSolver2& instance)
	{
		return instance.GetEmitter();
	},
		[](GridFluidSolver2& instance, const GridEmitter2Ptr& emitter)
	{
		instance.SetEmitter(emitter);
	},
		R"pbdoc(
			The emitter.
		)pbdoc");
}

void AddGridFluidSolver3(pybind11::module& m)
{
	pybind11::class_<GridFluidSolver3, GridFluidSolver3Ptr, PhysicsAnimation>(m, "GridFluidSolver3",
		R"pbdoc(
			Abstract base class for grid-based 3-D fluid solver.

			This is an abstract base class for grid-based 3-D fluid solver based on
			Jos Stam's famous 1999 paper - "Stable Fluids". This solver takes fractional
			step method as its foundation which is consisted of independent advection,
			diffusion, external forces, and pressure projection steps. Each step is
			configurable so that a custom step can be implemented. For example, if a
			user wants to change the advection solver to her/his own implementation,
			simply call setAdvectionSolver(newSolver).
		)pbdoc")
	.def_property("gravity", &GridFluidSolver3::GetGravity, [](GridFluidSolver3& instance, pybind11::object obj)
	{
		instance.SetGravity(ObjectToVector3D(obj));
	},
		R"pbdoc(
			The gravity vector of the system.
		)pbdoc")
	.def_property("viscosityCoefficient", &GridFluidSolver3::GetViscosityCoefficient, &GridFluidSolver3::SetViscosityCoefficient,
		R"pbdoc(
			The viscosity coefficient.
		)pbdoc")
	.def("GetCFL", &GridFluidSolver3::GetCFL,
		R"pbdoc(
			Returns the CFL number from the current velocity field for given
			time interval.

			Parameters
			----------
			- timeIntervalInSeconds : The time interval in seconds.
		)pbdoc",
		pybind11::arg("timeIntervalInSeconds"))
	.def_property("maxCFL", &GridFluidSolver3::GetMaxCFL, &GridFluidSolver3::SetMaxCFL,
		R"pbdoc(
			The max allowed CFL number.
		)pbdoc")
	.def_property("useCompressedLinearSystem", &GridFluidSolver3::GetUseCompressedLinearSystem, &GridFluidSolver3::SetUseCompressedLinearSystem,
		R"pbdoc(
			True if the solver is using compressed linear system.
		)pbdoc")
	.def_property("advectionSolver", &GridFluidSolver3::GetAdvectionSolver, &GridFluidSolver3::SetAdvectionSolver,
		R"pbdoc(
			The advection solver.
		)pbdoc")
	.def_property("diffusionSolver", &GridFluidSolver3::GetDiffusionSolver, &GridFluidSolver3::SetDiffusionSolver,
		R"pbdoc(
			The diffusion solver instance.
		)pbdoc")
	.def_property("pressureSolver", &GridFluidSolver3::GetPressureSolver, &GridFluidSolver3::SetPressureSolver,
		R"pbdoc(
			The pressure solver instance.
		)pbdoc")
	.def_property("closedDomainBoundaryFlag", &GridFluidSolver3::GetClosedDomainBoundaryFlag, &GridFluidSolver3::SetClosedDomainBoundaryFlag,
		R"pbdoc(
			The closed domain boundary flag.

			This flag is an integer which is bitwise-combination of
			DIRECTION_LEFT, DIRECTION_RIGHT, DIRECTION_DOWN, DIRECTION_UP,
			and DIRECTION_BACK.
		)pbdoc")
	.def("ResizeGrid", [](GridFluidSolver3& instance, pybind11::args args, pybind11::kwargs kwargs)
	{
		Size3 resolution{ 1, 1 };
		Vector3D gridSpacing{ 1, 1 };
		Vector3D gridOrigin{ 0, 0 };

		ParseGridResizeParams(args, kwargs, resolution, gridSpacing, gridOrigin);

		instance.ResizeGrid(resolution, gridSpacing, gridOrigin);
	},
		R"pbdoc(
			Resizes grid system data.

			This function resizes grid system data. You can also resize the grid by
			calling resize function directly from
			GridFluidSolver3::GetGridSystemData(), but this function provides a
			shortcut for the same operation.

			Parameters
			----------
			- `*args` : resolution, gridSpacing, and gridOrigin arguments.
			- `**kwargs`
				- resolution : Grid resolution.
				- gridSpacing : Grid spacing.
				- gridOrigin : Origin point at the grid.
				- domainSizeX : Domain size in x-direction.
		)pbdoc")
	.def_property_readonly("resolution", &GridFluidSolver3::GetResolution,
		R"pbdoc(
			The resolution of the grid system data.

			This function returns the resolution of the grid system data. This is
			equivalent to calling gridSystemData.resolution, but provides a
			shortcut.
		)pbdoc")
	.def_property_readonly("gridSpacing", &GridFluidSolver3::GetGridSpacing,
		R"pbdoc(
			The grid spacing of the grid system data.

			This function returns the resolution of the grid system data. This is
			equivalent to calling gridSystemData.gridSpacing, but provides a
			shortcut.
		)pbdoc")
	.def_property_readonly("gridOrigin", &GridFluidSolver3::GetGridOrigin,
		R"pbdoc(
			The origin of the grid system data.

			This function returns the resolution of the grid system data. This is
			equivalent to calling gridSystemData.origin, but provides a
			shortcut.
		)pbdoc")
	.def_property_readonly("velocity", &GridFluidSolver3::GetVelocity,
		R"pbdoc(
			The velocity field.

			This property returns the velocity field from the grid system data.
			It is just a shortcut to the most commonly accessed data chunk.
		)pbdoc")
	.def_property("collider", [](const GridFluidSolver3& instance)
	{
		return instance.GetCollider();
	},
		[](GridFluidSolver3& instance, const Collider3Ptr& collider)
	{
		instance.SetCollider(collider);
	},
		R"pbdoc(
			The collider.
		)pbdoc")
	.def_property("emitter", [](const GridFluidSolver3& instance)
	{
		return instance.GetEmitter();
	},
		[](GridFluidSolver3& instance, const GridEmitter3Ptr& emitter)
	{
		instance.SetEmitter(emitter);
	},
		R"pbdoc(
			The emitter.
		)pbdoc");
}