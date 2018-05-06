/*************************************************************************
> File Name: Cylinder.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Cylinder functions for CubbyFlow Python API.
> Created Time: 2018/01/28
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Geometry/Cylinder.h>
#include <API/Python/Utils/pybind11Utils.h>
#include <Core/Geometry/Cylinder3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddCylinder3(pybind11::module& m)
{
	pybind11::class_<Cylinder3, Cylinder3Ptr, Surface3>(static_cast<pybind11::handle>(m), "Cylinder3")
	.def("__init__", [](Cylinder3& instance, pybind11::object center, double radius, double height, const Transform3& transform, bool isNormalFlipped)
	{
		new (&instance) Cylinder3(ObjectToVector3D(center), radius, height, transform, isNormalFlipped);
	},
			R"pbdoc(
				Constructs Cylinder3

				This method constructs Cylinder3 with center, radius, height,
				transform, and normal direction (isNormalFlipped).
			)pbdoc",
		pybind11::arg("center") = Vector3D(0, 0, 0),
		pybind11::arg("radius") = 1.0,
		pybind11::arg("height") = 1.0,
		pybind11::arg("transform") = Transform3(),
		pybind11::arg("isNormalFlipped") = false)
	.def_readwrite("center", &Cylinder3::center)
	.def_readwrite("radius", &Cylinder3::radius)
	.def_readwrite("height", &Cylinder3::height);
}