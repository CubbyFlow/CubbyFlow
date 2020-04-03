// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Geometry/Cylinder.hpp>
#include <API/Python/Utils/pybind11Utils.hpp>
#include <Core/Geometry/Cylinder3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddCylinder3(pybind11::module& m)
{
    pybind11::class_<Cylinder3, Cylinder3Ptr, Surface3>(
        static_cast<pybind11::handle>(m), "Cylinder3")
        .def(
            "__init__",
            [](Cylinder3& instance, pybind11::object center, double radius,
               double height, const Transform3& transform,
               bool isNormalFlipped) {
                new (&instance) Cylinder3(ObjectToVector3D(center), radius,
                                          height, transform, isNormalFlipped);
            },
            R"pbdoc(
				Constructs Cylinder3

				This method constructs Cylinder3 with center, radius, height,
				transform, and normal direction (isNormalFlipped).
			)pbdoc",
            pybind11::arg("center") = Vector3D(0, 0, 0),
            pybind11::arg("radius") = 1.0, pybind11::arg("height") = 1.0,
            pybind11::arg("transform") = Transform3(),
            pybind11::arg("isNormalFlipped") = false)
        .def_readwrite("center", &Cylinder3::center)
        .def_readwrite("radius", &Cylinder3::radius)
        .def_readwrite("height", &Cylinder3::height);
}