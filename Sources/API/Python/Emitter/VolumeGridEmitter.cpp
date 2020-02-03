/*************************************************************************
> File Name: VolumeGridEmitter.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: VolumeGridEmitter functions for CubbyFlow Python API.
> Created Time: 2018/02/11
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Emitter/VolumeGridEmitter.h>
#include <Core/Emitter/VolumeGridEmitter2.hpp>
#include <Core/Emitter/VolumeGridEmitter3.hpp>
#include <Core/Surface/SurfaceToImplicit2.h>
#include <Core/Surface/SurfaceToImplicit3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddVolumeGridEmitter2(pybind11::module& m)
{
	pybind11::class_<VolumeGridEmitter2, VolumeGridEmitter2Ptr, GridEmitter2>(m, "VolumeGridEmitter2",
		R"pbdoc(
			2-D grid-based volumetric emitter.
		)pbdoc")
	.def("__init__", [](VolumeGridEmitter2& instance, const Surface2Ptr& sourceRegion, bool isOneShot)
	{
		ImplicitSurface3Ptr sourceRegion_;
		auto implicit = std::dynamic_pointer_cast<ImplicitSurface2>(sourceRegion);
		
		if (implicit != nullptr)
		{
			new (&instance) VolumeGridEmitter2(implicit, isOneShot);
		}
		else
		{
			new (&instance) VolumeGridEmitter2(std::make_shared<SurfaceToImplicit2>(sourceRegion), isOneShot);
		}
	},
		R"pbdoc(
			Constructs an emitter with a source and is-one-shot flag.
		)pbdoc",
		pybind11::arg("sourceRegion"),
		pybind11::arg("isOneShot") = true)
	.def("AddSignedDistanceTarget", &VolumeGridEmitter2::AddSignedDistanceTarget,
		R"pbdoc(
			Adds signed-distance target to the scalar grid.
		)pbdoc",
		pybind11::arg("scalarGridTarget"))
	.def("AddStepFunctionTarget", &VolumeGridEmitter2::AddStepFunctionTarget,
		R"pbdoc(
			Adds step function target to the scalar grid.

			Parameters
			----------
			- scalarGridTarget : The scalar grid target.
			- minValue : The minimum value of the step function.
			- maxValue : The maximum value of the step function.
		)pbdoc",
		pybind11::arg("scalarGridTarget"),
		pybind11::arg("minValue"),
		pybind11::arg("maxValue"))
	.def("AddTarget", [](VolumeGridEmitter2& instance, pybind11::object obj, pybind11::function mapper)
	{
		if (pybind11::isinstance<ScalarGrid2Ptr>(obj))
		{
			instance.AddTarget(obj.cast<ScalarGrid2Ptr>(),
				[mapper](double ds, const Vector2D& l, double old) -> double
			{
				return mapper(ds, l, old).cast<double>();
			});
		}
		else if (pybind11::isinstance<VectorGrid2Ptr>(obj))
		{
			instance.AddTarget(obj.cast<VectorGrid2Ptr>(),
				[mapper](double ds, const Vector2D& l, const Vector2D& old) -> Vector2D
			{
				return mapper(ds, l, old).cast<Vector2D>();
			});
		}
		else
		{
			throw std::invalid_argument("Unknown grid type.");
		}
	},
		R"pbdoc(
			Adds a scalar/vector grid target.

			This function adds a custom target to the emitter. The first parameter
			defines which grid should it write to. The second parameter,
			`customMapper`, defines how to map signed-distance field from the
			volume geometry and location of the point to the final value that
			is going to be written to the target grid. The third parameter defines
			how to blend the old value from the target grid and the new value from
			the mapper function.

			Parameters
			----------
			- gridTarget : The scalar/vector grid target.
			- customMapper : The custom mapper.
		)pbdoc",
		pybind11::arg("scalarGridTarget"),
		pybind11::arg("customMapper"))
	.def_property_readonly("sourceRegion", &VolumeGridEmitter2::GetSourceRegion,
		R"pbdoc(
			Implicit surface which defines the source region.
		)pbdoc")
	.def_property_readonly("isOneShot", &VolumeGridEmitter2::GetIsOneShot,
		R"pbdoc(
			True if this emits only once.
		)pbdoc");
}

void AddVolumeGridEmitter3(pybind11::module& m)
{
	pybind11::class_<VolumeGridEmitter3, VolumeGridEmitter3Ptr, GridEmitter3>(m, "VolumeGridEmitter3",
		R"pbdoc(
			3-D grid-based volumetric emitter.
		)pbdoc")
	.def("__init__", [](VolumeGridEmitter3& instance, const Surface3Ptr& sourceRegion, bool isOneShot)
	{
		ImplicitSurface3Ptr sourceRegion_;
		auto implicit = std::dynamic_pointer_cast<ImplicitSurface3>(sourceRegion);
		
		if (implicit != nullptr)
		{
			new (&instance) VolumeGridEmitter3(implicit, isOneShot);
		}
		else
		{
			new (&instance) VolumeGridEmitter3(std::make_shared<SurfaceToImplicit3>(sourceRegion), isOneShot);
		}
	},
		R"pbdoc(
			Constructs an emitter with a source and is-one-shot flag.
		)pbdoc",
		pybind11::arg("sourceRegion"),
		pybind11::arg("isOneShot") = true)
	.def("AddSignedDistanceTarget", &VolumeGridEmitter3::AddSignedDistanceTarget,
		R"pbdoc(
			Adds signed-distance target to the scalar grid.
		)pbdoc",
		pybind11::arg("scalarGridTarget"))
	.def("AddStepFunctionTarget", &VolumeGridEmitter3::AddStepFunctionTarget,
		R"pbdoc(
			Adds step function target to the scalar grid.

			Parameters
			----------
			- scalarGridTarget : The scalar grid target.
			- minValue : The minimum value of the step function.
			- maxValue : The maximum value of the step function.
		)pbdoc",
		pybind11::arg("scalarGridTarget"),
		pybind11::arg("minValue"),
		pybind11::arg("maxValue"))
	.def("AddTarget", [](VolumeGridEmitter3& instance, pybind11::object obj, pybind11::function mapper)
	{
		if (pybind11::isinstance<ScalarGrid3Ptr>(obj))
		{
			instance.AddTarget(obj.cast<ScalarGrid3Ptr>(),
				[mapper](double ds, const Vector3D& l, double old) -> double
			{
				return mapper(ds, l, old).cast<double>();
			});
		}
		else if (pybind11::isinstance<VectorGrid3Ptr>(obj))
		{
			instance.AddTarget(obj.cast<VectorGrid3Ptr>(),
				[mapper](double ds, const Vector3D& l, const Vector3D& old) -> Vector3D
			{
				return mapper(ds, l, old).cast<Vector3D>();
			});
		}
		else
		{
			throw std::invalid_argument("Unknown grid type.");
		}
	},
		R"pbdoc(
			Adds a scalar/vector grid target.

			This function adds a custom target to the emitter. The first parameter
			defines which grid should it write to. The second parameter,
			customMapper`, defines how to map signed-distance field from the
			volume geometry and location of the point to the final value that
			is going to be written to the target grid. The third parameter defines
			how to blend the old value from the target grid and the new value from
			the mapper function.

			Parameters
			----------
			- gridTarget : The scalar/vector grid target.
			- customMapper : The custom mapper.
		)pbdoc",
		pybind11::arg("scalarGridTarget"),
		pybind11::arg("customMapper"))
	.def_property_readonly("sourceRegion", &VolumeGridEmitter3::GetSourceRegion,
		R"pbdoc(
			Implicit surface which defines the source region.
		)pbdoc")
	.def_property_readonly("isOneShot", &VolumeGridEmitter3::GetIsOneShot,
		R"pbdoc(
			True if this emits only once.
		)pbdoc");
}