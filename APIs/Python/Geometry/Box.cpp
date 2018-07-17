/*************************************************************************
> File Name: Box.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Box functions for CubbyFlow Python API.
> Created Time: 2018/01/28
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Python/Geometry/Box.h>
#include <Python/Utils/pybind11Utils.h>
#include <Geometry/Box2.h>
#include <Geometry/Box3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddBox2(pybind11::module& m)
{
	pybind11::class_<Box2, Box2Ptr, Surface2>(static_cast<pybind11::handle>(m), "Box2",
		R"pbdoc(
			2-D box geometry.

			This class represents 2-D box geometry which extends Surface2 by overriding
			surface-related queries. This box implementation is an axis-aligned box
			that wraps lower-level primitive type, BoundingBox2D.
		)pbdoc")
	.def("__init__", [](Box2& instance, pybind11::object lowerCorner, pybind11::object upperCorner, const Transform2& transform, bool isNormalFlipped)
	{
		new (&instance) Box2(ObjectToVector2D(lowerCorner), ObjectToVector2D(upperCorner), transform, isNormalFlipped);
	},
			R"pbdoc(
				Constructs Box2

				This method constructs Box2 with center, radius, height,
				transform, and normal direction (isNormalFlipped).
			)pbdoc",
		pybind11::arg("lowerCorner") = Vector2D(0, 0),
		pybind11::arg("upperCorner") = Vector2D(1, 1),
		pybind11::arg("transform") = Transform2(),
		pybind11::arg("isNormalFlipped") = false)
	.def_readwrite("bound", &Box2::bound);
}

void AddBox3(pybind11::module& m)
{
	pybind11::class_<Box3, Box3Ptr, Surface3>(static_cast<pybind11::handle>(m), "Box3",
		R"pbdoc(
			3-D box geometry.

			This class represents 3-D box geometry which extends Surface3 by overriding
			surface-related queries. This box implementation is an axis-aligned box
			that wraps lower-level primitive type, BoundingBox3D.
		)pbdoc")
	.def("__init__",
			[](Box3& instance, pybind11::object lowerCorner, pybind11::object upperCorner, const Transform3& transform, bool isNormalFlipped)
	{
		new (&instance) Box3(ObjectToVector3D(lowerCorner), ObjectToVector3D(upperCorner), transform, isNormalFlipped);
	},
			R"pbdoc(
				Constructs Box3

				This method constructs Box3 with center, radius, height,
				transform, and normal direction (isNormalFlipped).
			)pbdoc",
		pybind11::arg("lowerCorner") = Vector3D(0, 0, 0),
		pybind11::arg("upperCorner") = Vector3D(1, 1, 1),
		pybind11::arg("transform") = Transform3(),
		pybind11::arg("isNormalFlipped") = false)
	.def_readwrite("bound", &Box3::bound);
}