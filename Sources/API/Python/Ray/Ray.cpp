/*************************************************************************
> File Name: Ray.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Ray functions for CubbyFlow Python API.
> Created Time: 2018/01/29
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Ray/Ray.h>
#include <API/Python/Utils/pybind11Utils.h>
#include <Core/Ray/Ray2.h>
#include <Core/Ray/Ray3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddRay2F(pybind11::module& m)
{
	pybind11::class_<Ray2F>(m, "Ray2F")
	.def("__init__", [](Ray2F& instance, pybind11::object origin, pybind11::object direction)
	{
		new (&instance) Ray2F(ObjectToVector2F(origin), ObjectToVector2F(direction));
	},
		R"pbdoc(
			Constructs Ray2F

			This method constructs 2D float ray with origin and direction.
		)pbdoc",
		pybind11::arg("origin") = Vector2F{},
		pybind11::arg("direction") = Vector2F{ 1, 0 })
	.def_readwrite("origin", &Ray2F::origin,
		R"pbdoc(
			Origin of the ray.
		)pbdoc")
	.def_readwrite("direction", &Ray2F::direction,
		R"pbdoc(
			Direction of the ray.
		)pbdoc");
}

void AddRay2D(pybind11::module& m)
{
	pybind11::class_<Ray2D>(m, "Ray2D")
	.def("__init__", [](Ray2D& instance, pybind11::object origin, pybind11::object direction)
	{
		new (&instance) Ray2D(ObjectToVector2D(origin), ObjectToVector2D(direction));
	},
		R"pbdoc(
			Constructs Ray2D

			This method constructs 2D double ray with origin and direction.
		)pbdoc",
		pybind11::arg("origin") = Vector2D{},
		pybind11::arg("direction") = Vector2D{ 1, 0 })
	.def_readwrite("origin", &Ray2D::origin,
		R"pbdoc(
			Origin of the ray.
		)pbdoc")
	.def_readwrite("direction", &Ray2D::direction,
		R"pbdoc(
			Direction of the ray.
		)pbdoc");
}

void AddRay3F(pybind11::module& m)
{
	pybind11::class_<Ray3F>(m, "Ray3F")
	.def("__init__", [](Ray3F& instance, pybind11::object origin, pybind11::object direction)
	{
		new (&instance) Ray3F(ObjectToVector3F(origin), ObjectToVector3F(direction));
	},
		R"pbdoc(
			Constructs Ray3F

			This method constructs 3D float ray with origin and direction.
		)pbdoc",
		pybind11::arg("origin") = Vector3F{},
		pybind11::arg("direction") = Vector3F{ 1, 0, 0 })
	.def_readwrite("origin", &Ray3F::origin,
		R"pbdoc(
			Origin of the ray.
		)pbdoc")
	.def_readwrite("direction", &Ray3F::direction,
		R"pbdoc(
			Direction of the ray.
		)pbdoc");
}

void AddRay3D(pybind11::module& m)
{
	pybind11::class_<Ray3D>(m, "Ray3D")
	.def("__init__", [](Ray3D& instance, pybind11::object origin, pybind11::object direction)
	{
		new (&instance) Ray3D(ObjectToVector3D(origin), ObjectToVector3D(direction));
	},
		R"pbdoc(
			Constructs Ray3D

			This method constructs 3D double ray with origin and direction.
		)pbdoc",
		pybind11::arg("origin") = Vector3D{},
		pybind11::arg("direction") = Vector3D{ 1, 0, 0 })
	.def_readwrite("origin", &Ray3D::origin,
		R"pbdoc(
			Origin of the ray.
		)pbdoc")
	.def_readwrite("direction", &Ray3D::direction,
		R"pbdoc(
			Direction of the ray.
		)pbdoc");
}