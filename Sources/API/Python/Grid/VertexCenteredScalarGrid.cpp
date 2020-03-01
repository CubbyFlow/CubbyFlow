/*************************************************************************
> File Name: VertexCenteredScalarGrid.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: VertexCenteredScalarGrid functions for CubbyFlow Python API.
> Created Time: 2018/02/07
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Grid/VertexCenteredScalarGrid.h>
#include <API/Python/Utils/pybind11Utils.h>
#include <Core/Grid/VertexCenteredScalarGrid2.hpp>
#include <Core/Grid/VertexCenteredScalarGrid3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddVertexCenteredScalarGrid2(pybind11::module& m)
{
	pybind11::class_<VertexCenteredScalarGrid2, VertexCenteredScalarGrid2Ptr>(m, "VertexCenteredScalarGrid2",
		R"pbdoc(
			2-D Cell-centered scalar grid structure.

			This class represents 2-D cell-centered scalar grid which extends
			ScalarGrid2. As its name suggests, the class defines the data point at the
			center of a grid cell. Thus, the dimension of data points are equal to the
			dimension of the cells.
		)pbdoc")
	.def("__init__", [](VertexCenteredScalarGrid2& instance, pybind11::args args, pybind11::kwargs kwargs)
	{
		Size2 resolution{ 1, 1 };
		Vector2D gridSpacing{ 1, 1 };
		Vector2D gridOrigin{ 0, 0 };

		ParseGridResizeParams(args, kwargs, resolution, gridSpacing, gridOrigin);
		new (&instance) VertexCenteredScalarGrid2( resolution, gridSpacing, gridOrigin);
	},
		R"pbdoc(
			Constructs grid.

			Parameters
			----------
			- `*args` : resolution, gridSpacing, and gridOrigin arguments.
			- `**kwargs`
				- resolution : Grid resolution.
				- gridSpacing : Grid spacing.
				- gridOrigin : Origin point ot the grid.
				- domainSizeX : Domain size in x-direction.
		)pbdoc")
	.def_property_readonly("dataSize", &VertexCenteredScalarGrid2::GetDataSize,
		R"pbdoc(
			Returns the actual data point size.
		)pbdoc")
	.def_property_readonly("dataOrigin", &VertexCenteredScalarGrid2::GetDataOrigin,
		R"pbdoc(
			Returns data position for the grid point at (0, 0).

			Note that this is different from origin() since origin() returns
			the lower corner point of the bounding box.
		)pbdoc")
	.def("Set", &VertexCenteredScalarGrid2::Set,
		R"pbdoc(
			Sets the contents with the given `other` grid.

			Parameters
			----------
			- other : Other grid to copy from.
		)pbdoc",
		pybind11::arg("other"));
}

void AddVertexCenteredScalarGrid3(pybind11::module& m)
{
	pybind11::class_<VertexCenteredScalarGrid3, VertexCenteredScalarGrid3Ptr>(m, "VertexCenteredScalarGrid3",
		R"pbdoc(
			3-D Cell-centered scalar grid structure.

			This class represents 3-D cell-centered scalar grid which extends
			ScalarGrid3. As its name suggests, the class defines the data point at the
			center of a grid cell. Thus, the dimension of data points are equal to the
			dimension of the cells.
		)pbdoc")
	.def("__init__", [](VertexCenteredScalarGrid3& instance, pybind11::args args, pybind11::kwargs kwargs)
	{
		Size3 resolution{ 1, 1, 1 };
		Vector3D gridSpacing{ 1, 1, 1 };
		Vector3D gridOrigin{ 0, 0, 0 };
		
		ParseGridResizeParams(args, kwargs, resolution, gridSpacing, gridOrigin);
		new (&instance) VertexCenteredScalarGrid3(
			resolution, gridSpacing, gridOrigin);
	},
		R"pbdoc(
			Constructs grid.

			Parameters
			----------
			- `*args` : resolution, gridSpacing, and gridOrigin arguments.
			- `**kwargs`
				- resolution : Grid resolution.
				- gridSpacing : Grid spacing.
				- gridOrigin : Origin point ot the grid.
				- domainSizeX : Domain size in x-direction.
		)pbdoc")
	.def_property_readonly("dataSize", &VertexCenteredScalarGrid3::GetDataSize,
		R"pbdoc(
			Returns the actual data point size.
		)pbdoc")
	.def_property_readonly("dataOrigin", &VertexCenteredScalarGrid3::GetDataOrigin,
		R"pbdoc(
			Returns data position for the grid point at (0, 0, 0).

			Note that this is different from origin() since origin() returns
			the lower corner point of the bounding box.
		)pbdoc")
	.def("Set", &VertexCenteredScalarGrid3::Set,
		R"pbdoc(
			Sets the contents with the given `other` grid.

			Parameters
			----------
			- other : Other grid to copy from.
		)pbdoc",
		pybind11::arg("other"));
}