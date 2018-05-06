/*************************************************************************
> File Name: CellCenteredVectorGrid.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: CellCenteredVectorGrid functions for CubbyFlow Python API.
> Created Time: 2018/02/06
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Grid/CellCenteredVectorGrid.h>
#include <API/Python/Utils/pybind11Utils.h>
#include <Core/Grid/CellCenteredVectorGrid2.h>
#include <Core/Grid/CellCenteredVectorGrid3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddCellCenteredVectorGrid2(pybind11::module& m)
{
	pybind11::class_<CellCenteredVectorGrid2, CellCenteredVectorGrid2Ptr, CollocatedVectorGrid2>(static_cast<pybind11::handle>(m), "CellCenteredVectorGrid2",
		R"pbdoc(
			2-D Cell-centered vector grid structure.

			This class represents 2-D cell-centered vector grid which extends
			CollocatedVectorGrid2. As its name suggests, the class defines the data
			point at the center of a grid cell. Thus, the dimension of data points are
			equal to the dimension of the cells.
		)pbdoc")
	.def("__init__", [](CellCenteredVectorGrid2& instance, pybind11::args args, pybind11::kwargs kwargs)
	{
		Size2 resolution{ 1, 1 };
		Vector2D gridSpacing{ 1, 1 };
		Vector2D gridOrigin{ 0, 0 };
		
		ParseGridResizeParams(args, kwargs, resolution, gridSpacing, gridOrigin);
		new (&instance) CellCenteredVectorGrid2(resolution, gridSpacing, gridOrigin);
	},
		R"pbdoc(
			Constructs grid.

			Parameters
			----------
			- `*args` : resolution, gridSpacing, and gridOrigin arguments.
			- `**kwargs`
				- resolution : Grid resolution.
				- gridSpacing : Grid spacing.
				- gridOrigin : Origin point at the grid.
				- domainSizeX : Domain size in x-direction.
			 )pbdoc")
	.def_property_readonly("dataSize", &CellCenteredVectorGrid2::GetDataSize,
		R"pbdoc(
			Returns the actual data point size.
		)pbdoc")
	.def_property_readonly("dataOrigin", &CellCenteredVectorGrid2::GetDataOrigin,
		R"pbdoc(
			Returns data position for the grid point at (0, 0).

			Note that this is different from origin() since origin() returns
			the lower corner point of the bounding box.
		)pbdoc")
	.def("Fill", [](CellCenteredVectorGrid2& instance, pybind11::object obj)
	{
		if (pybind11::isinstance<Vector2D>(static_cast<pybind11::handle>(obj)))
		{
			instance.Fill(obj.cast<Vector2D>());
		}
		else if (pybind11::isinstance<pybind11::tuple>(static_cast<pybind11::handle>(obj)))
		{
			instance.Fill(ObjectToVector2D(obj));
		}
		else if (pybind11::isinstance<pybind11::function>(static_cast<pybind11::handle>(obj)))
		{
			auto func = obj.cast<pybind11::function>();
			instance.Fill([func](const Vector2D& pt)
			{
				return ObjectToVector2D(func(pt));
			}, ExecutionPolicy::Serial);
		}
		else
		{
			throw std::invalid_argument("Input type must be Vector2D or function object -> Vector2D");
		}
	},
		R"pbdoc(
			Fills the grid with given value or function.
		)pbdoc")
	.def("Set", &CellCenteredVectorGrid2::Set,
		R"pbdoc(
			Sets the contents with the given `other` grid.

			Parameters
			----------
			- other : Other grid to copy from.
		)pbdoc",
		pybind11::arg("other"));
}

void AddCellCenteredVectorGrid3(pybind11::module& m)
{
	pybind11::class_<CellCenteredVectorGrid3, CellCenteredVectorGrid3Ptr, CollocatedVectorGrid3>(static_cast<pybind11::handle>(m), "CellCenteredVectorGrid3",
		R"pbdoc(
			3-D Cell-centered vector grid structure.

			This class represents 3-D cell-centered vector grid which extends
			CollocatedVectorGrid3. As its name suggests, the class defines the data
			point at the center of a grid cell. Thus, the dimension of data points are
			equal to the dimension of the cells.
		)pbdoc")
	.def("__init__", [](CellCenteredVectorGrid3& instance, pybind11::args args, pybind11::kwargs kwargs)
	{
		Size3 resolution{ 1, 1, 1 };
		Vector3D gridSpacing{ 1, 1, 1 };
		Vector3D gridOrigin{ 0, 0, 0 };

		ParseGridResizeParams(args, kwargs, resolution, gridSpacing, gridOrigin);
		new (&instance) CellCenteredVectorGrid3(resolution, gridSpacing, gridOrigin);
	},
		R"pbdoc(
			Constructs grid.

			Parameters
			----------
			- `*args` : resolution, gridSpacing, and gridOrigin arguments.
			- `**kwargs`
				- resolution : Grid resolution.
				- gridSpacing : Grid spacing.
				- gridOrigin : Origin point at the grid.
				- domainSizeX : Domain size in x-direction.
		)pbdoc")
	.def_property_readonly("dataSize", &CellCenteredVectorGrid3::GetDataSize,
		R"pbdoc(
			Returns the actual data point size.
		)pbdoc")
	.def_property_readonly("dataOrigin", &CellCenteredVectorGrid3::GetDataOrigin,
		R"pbdoc(
			Returns data position for the grid point at (0, 0, 0).

			Note that this is different from origin() since origin() returns
			the lower corner point of the bounding box.
		)pbdoc")
	.def("Fill", [](CellCenteredVectorGrid3& instance, pybind11::object obj)
	{
		if (pybind11::isinstance<Vector3D>(static_cast<pybind11::handle>(obj)))
		{
			instance.Fill(obj.cast<Vector3D>());
		}
		else if (pybind11::isinstance<pybind11::tuple>(static_cast<pybind11::handle>(obj)))
		{
			instance.Fill(ObjectToVector3D(obj));
		}
		else if (pybind11::isinstance<pybind11::function>(static_cast<pybind11::handle>(obj)))
		{
			auto func = obj.cast<pybind11::function>();
			instance.Fill(
				[func](const Vector3D& pt)
			{
				return ObjectToVector3D(func(pt));
			}, ExecutionPolicy::Serial);
		}
		else
		{
			throw std::invalid_argument("Input type must be Vector3D or function object -> Vector3D");
		}
	},
		R"pbdoc(
			Fills the grid with given value or function.
		)pbdoc")
	.def("Set", &CellCenteredVectorGrid3::Set,
		R"pbdoc(
			Sets the contents with the given `other` grid.

			Parameters
			----------
			- other : Other grid to copy from.
		)pbdoc",
		pybind11::arg("other"));
}