/*************************************************************************
> File Name: VertexCenteredVectorGrid.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: VertexCenteredVectorGrid functions for CubbyFlow Python API.
> Created Time: 2018/02/07
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Grid/VertexCenteredVectorGrid.h>
#include <API/Python/Utils/pybind11Utils.h>
#include <Core/Grid/VertexCenteredVectorGrid2.hpp>
#include <Core/Grid/VertexCenteredVectorGrid3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddVertexCenteredVectorGrid2(pybind11::module& m)
{
	pybind11::class_<VertexCenteredVectorGrid2, VertexCenteredVectorGrid2Ptr, CollocatedVectorGrid2>(m, "VertexCenteredVectorGrid2",
		R"pbdoc(
			2-D Vertex-centered vector grid structure.

			This class represents 2-D vertex-centered vector grid which extends
			CollocatedVectorGrid2. As its name suggests, the class defines the data
			point at the center of a grid vertex. Thus, the dimension of data points are
			equal to the dimension of the vertices.
		)pbdoc")
	.def("__init__", [](VertexCenteredVectorGrid2& instance, pybind11::args args, pybind11::kwargs kwargs)
	{
		Size2 resolution{ 1, 1 };
		Vector2D gridSpacing{ 1, 1 };
		Vector2D gridOrigin{ 0, 0 };
		
		ParseGridResizeParams(args, kwargs, resolution, gridSpacing, gridOrigin);
		new (&instance) VertexCenteredVectorGrid2(resolution, gridSpacing, gridOrigin);
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
	.def_property_readonly("dataSize", &VertexCenteredVectorGrid2::GetDataSize,
		R"pbdoc(
			Returns the actual data point size.
		)pbdoc")
	.def_property_readonly("dataOrigin", &VertexCenteredVectorGrid2::GetDataOrigin,
		R"pbdoc(
			Returns data position for the grid point at (0, 0).

			Note that this is different from origin() since origin() returns
			the lower corner point of the bounding box.
		)pbdoc")
	.def("Fill", [](VertexCenteredVectorGrid2& instance, pybind11::object obj)
	{
		if (pybind11::isinstance<Vector2D>(obj))
		{
			instance.Fill(obj.cast<Vector2D>());
		}
		else if (pybind11::isinstance<pybind11::tuple>(obj))
		{
			instance.Fill(ObjectToVector2D(obj));
		}
		else if (pybind11::isinstance<pybind11::function>(obj))
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
	.def("Set", &VertexCenteredVectorGrid2::Set,
		R"pbdoc(
			Sets the contents with the given `other` grid.

			Parameters
			----------
			- other : Other grid to copy from.
		)pbdoc",
		pybind11::arg("other"));
}

void AddVertexCenteredVectorGrid3(pybind11::module& m)
{
	pybind11::class_<VertexCenteredVectorGrid3, VertexCenteredVectorGrid3Ptr, CollocatedVectorGrid3>(m, "VertexCenteredVectorGrid3",
		R"pbdoc(
			3-D Vertex-centered vector grid structure.

			This class represents 3-D vertex-centered vector grid which extends
			CollocatedVectorGrid3. As its name suggests, the class defines the data
			point at the center of a grid vertex. Thus, the dimension of data points are
			equal to the dimension of the vertices.
		)pbdoc")
	.def("__init__", [](VertexCenteredVectorGrid3& instance, pybind11::args args, pybind11::kwargs kwargs)
	{
		Size3 resolution{ 1, 1, 1 };
		Vector3D gridSpacing{ 1, 1, 1 };
		Vector3D gridOrigin{ 0, 0, 0 };
		
		ParseGridResizeParams(args, kwargs, resolution, gridSpacing, gridOrigin);
		new (&instance) VertexCenteredVectorGrid3(resolution, gridSpacing, gridOrigin);
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
	.def_property_readonly("dataSize", &VertexCenteredVectorGrid3::GetDataSize,
		R"pbdoc(
			Returns the actual data point size.
		)pbdoc")
	.def_property_readonly("dataOrigin", &VertexCenteredVectorGrid3::GetDataOrigin,
		R"pbdoc(
			Returns data position for the grid point at (0, 0, 0).

			Note that this is different from origin() since origin() returns
			the lower corner point of the bounding box.
		)pbdoc")
	.def("Fill", [](VertexCenteredVectorGrid3& instance, pybind11::object obj)
	{
		if (pybind11::isinstance<Vector3D>(obj))
		{
			instance.Fill(obj.cast<Vector3D>());
		}
		else if (pybind11::isinstance<pybind11::tuple>(obj))
		{
			instance.Fill(ObjectToVector3D(obj));
		}
		else if (pybind11::isinstance<pybind11::function>(obj))
		{
			auto func = obj.cast<pybind11::function>();
			instance.Fill([func](const Vector3D& pt)
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
	.def("Set", &VertexCenteredVectorGrid3::Set,
		R"pbdoc(
			Sets the contents with the given `other` grid.

			Parameters
			----------
			- other : Other grid to copy from.
		)pbdoc",
		pybind11::arg("other"));
}