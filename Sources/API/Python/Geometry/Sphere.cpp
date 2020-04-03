// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Geometry/Sphere.hpp>
#include <API/Python/Utils/pybind11Utils.hpp>
#include <Core/Geometry/Sphere2.hpp>
#include <Core/Geometry/Sphere3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddSphere2(pybind11::module& m)
{
    pybind11::class_<Sphere2, Sphere2Ptr, Surface2>(
        static_cast<pybind11::handle>(m), "Sphere2")
        .def(
            "__init__",
            [](Sphere2& instance, pybind11::object center, double radius,
               const Transform2& transform, bool isNormalFlipped) {
                new (&instance) Sphere2(ObjectToVector2D(center), radius,
                                        transform, isNormalFlipped);
            },
            R"pbdoc(
			Constructs Sphere2.

			This method constructs Sphere2 with center, radius, transform,
			and normal direction (isNormalFlipped).
		)pbdoc",
            pybind11::arg("center") = Vector2D{}, pybind11::arg("radius") = 1.0,
            pybind11::arg("transform") = Transform2(),
            pybind11::arg("isNormalFlipped") = false)
        .def_readwrite("center", &Sphere2::center)
        .def_readwrite("radius", &Sphere2::radius);
}

void AddSphere3(pybind11::module& m)
{
    pybind11::class_<Sphere3, Sphere3Ptr, Surface3>(
        static_cast<pybind11::handle>(m), "Sphere3")
        .def(
            "__init__",
            [](Sphere3& instance, pybind11::object center, double radius,
               const Transform3& transform, bool isNormalFlipped) {
                new (&instance) Sphere3(ObjectToVector3D(center), radius,
                                        transform, isNormalFlipped);
            },
            R"pbdoc(
			Constructs Sphere3.

			This method constructs Sphere3 with center, radius, transform,
			and normal direction (isNormalFlipped).
		)pbdoc",
            pybind11::arg("center") = Vector3D{}, pybind11::arg("radius") = 1.0,
            pybind11::arg("transform") = Transform3(),
            pybind11::arg("isNormalFlipped") = false)
        .def_readwrite("center", &Sphere3::center)
        .def_readwrite("radius", &Sphere3::radius);
}